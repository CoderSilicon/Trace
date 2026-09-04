#include "download.hpp"
#include "progress.hpp"
#include "colors.hpp"
#include <curl/curl.h>
#include <iostream>
#include <cstdio>
#include <atomic>
#include <thread>
#include <chrono>
#include <set>
#include <algorithm>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

static size_t FileWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    std::ofstream *ofs = static_cast<std::ofstream *>(userp);
    if (ofs && ofs->is_open())
    {
        ofs->write(static_cast<char *>(contents), totalSize);
        return totalSize;
    }
    return 0;
}

// Shared task counter for the worker pool.
static std::atomic<size_t> gNext{0};

// --------------------------------------------------------------- helpers

static long long existingSize(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return static_cast<long long>(st.st_size);
    return 0;
}

static std::string filenameFromUrl(const std::string &url)
{
    std::string u = url;
    u = u.substr(0, u.find('#'));
    u = u.substr(0, u.find('?'));
    size_t pos = u.find_last_of('/');
    std::string name = (pos == std::string::npos || pos + 1 >= u.size()) ? "downloaded_file.out" : u.substr(pos + 1);
    if (name.empty())
        name = "downloaded_file.out";
    return name;
}

static std::string uniquify(const std::string &path, std::set<std::string> &used)
{
    if (used.insert(path).second)
        return path;

    std::string base = path, ext;
    size_t dot = path.find_last_of('.');
    size_t slash = path.find_last_of('/');
    if (dot != std::string::npos && (slash == std::string::npos || dot > slash))
    {
        base = path.substr(0, dot);
        ext = path.substr(dot);
    }
    for (int i = 1;; ++i)
    {
        std::string cand = base + " (" + std::to_string(i) + ")" + ext;
        if (used.insert(cand).second)
            return cand;
    }
}

static std::vector<DownloadTask> buildTasks(const DownloadOptions &opts)
{
    std::vector<DownloadTask> tasks(opts.urls.size());
    std::set<std::string> used;

    for (size_t i = 0; i < opts.urls.size(); ++i)
    {
        std::string name = (i == 0 && !opts.filename.empty()) ? opts.filename : filenameFromUrl(opts.urls[i]);

        while (name.find('/') != std::string::npos && !opts.directory.empty())
            name = name.substr(name.find('/') + 1);

        std::string dir = opts.directory;
        if (!dir.empty() && dir.back() != '/' && dir.back() != '\\')
            dir += '/';

        tasks[i].url = opts.urls[i];
        tasks[i].path = uniquify(dir + name, used);
    }
    return tasks;
}

// ------------------------------------------------------------- transfer

// Called by libcurl many times per second; feeds the renderer. Only the few
// atomics are shared with the renderer, everything else is worker-local.
static int xferInfoCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                            [[maybe_unused]] curl_off_t ultotal,
                            [[maybe_unused]] curl_off_t ulnow)
{
    DownloadTask *t = static_cast<DownloadTask *>(clientp);
    if (t->state.load() == DL_ACTIVE)
    {
        long long base = t->resumeOffset;
        if (dltotal > 0)
            t->expected.store(base + static_cast<long long>(dltotal));
        t->done.store(base + static_cast<long long>(dlnow));

        auto now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration<double>(now - t->lastTick).count();
        if (dt >= 0.25) // sample ~4x/sec, then smooth with an EMA
        {
            double inst = static_cast<double>(dlnow - t->lastDone) / dt;
            if (inst >= 0.0)
            {
                double prev = t->speed.load();
                t->speed.store(prev <= 0.0 ? inst : prev * 0.7 + inst * 0.3);
            }
            t->lastDone = dlnow;
            t->lastTick = now;
        }
    }
    return 0;
}

static void runDownload(DownloadTask &t, const DownloadOptions &opts)
{
    t.state.store(DL_ACTIVE);
    t.lastTick = std::chrono::steady_clock::now();

    if (opts.resume)
        t.resumeOffset = existingSize(t.path);

    t.done.store(t.resumeOffset);
    t.expected.store(t.resumeOffset);

    bool havePartial = (t.resumeOffset > 0);
    t.out.open(t.path, std::ios::binary | (havePartial ? std::ios::app : std::ios::trunc));
    if (!t.out.is_open())
    {
        t.state.store(DL_ERROR);
        t.error = "cannot open file for writing";
        return;
    }

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        t.out.close();
        if (!opts.resume)
            std::remove(t.path.c_str());
        t.state.store(DL_ERROR);
        t.error = "curl_easy_init failed";
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, t.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &t.out);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L); // 404/500 => hard error, no fake HTML file
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, opts.followRedirects ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, opts.userAgent.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferInfoCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &t);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    if (opts.insecure)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    if (havePartial)
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, static_cast<curl_off_t>(t.resumeOffset));

    auto t0 = std::chrono::steady_clock::now();
    CURLcode res = curl_easy_perform(curl);
    t.elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();

    t.out.close();
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        if (!opts.resume)
            std::remove(t.path.c_str()); // no garbage on disk
        t.state.store(DL_ERROR);
        t.error = curl_easy_strerror(res);
        return;
    }

    t.bytes = t.done.load();
    t.state.store(DL_DONE);
}

// ------------------------------------------------------------ public API

int downloadAll(const DownloadOptions &opts)
{
    if (opts.urls.empty())
    {
        std::cerr << Color::RED << "Error: nothing to download.\n" << Color::RESET;
        return 1;
    }

    if (!opts.directory.empty())
    {
        struct stat st;
        if (stat(opts.directory.c_str(), &st) != 0 || !S_ISDIR(st.st_mode))
        {
            std::cerr << Color::RED << "Error: directory '" << opts.directory
                      << "' does not exist.\n" << Color::RESET;
            return 1;
        }
    }

    if (!opts.directory.empty() && opts.filename.find('/') != std::string::npos)
    {
        std::cerr << Color::RED << "Error: cannot combine -d with an -o path.\n" << Color::RESET;
        return 1;
    }

    bool tty = false;
#ifdef _WIN32
    tty = _isatty(_fileno(stdout)) != 0;
#else
    tty = isatty(STDOUT_FILENO) != 0;
#endif

    std::vector<DownloadTask> tasks = buildTasks(opts);
    size_t nThreads = std::min(static_cast<size_t>(opts.parallel), tasks.size());
    if (nThreads < 1)
        nThreads = 1;

    curl_global_init(CURL_GLOBAL_ALL);
    gNext.store(0);

    std::vector<std::thread> threads;
    for (size_t i = 0; i < nThreads; ++i)
    {
        threads.emplace_back([&tasks, &opts]() {
            while (true)
            {
                size_t idx = gNext.fetch_add(1);
                if (idx >= tasks.size())
                    break;
                runDownload(tasks[idx], opts);
            }
        });
    }

    renderDownloads(tasks, tty, opts.parallel);

    for (auto &th : threads)
        th.join();
    curl_global_cleanup();

    int failures = 0;
    for (const auto &t : tasks)
        if (t.state.load() == DL_ERROR)
            ++failures;

    if (failures > 0)
    {
        std::cout << Color::RED << Color::BOLD << failures << " of " << tasks.size()
                  << " downloads failed." << Color::RESET << "\n";
    }
    else if (tasks.size() > 1)
    {
        std::cout << Color::GREEN << Color::BOLD << "All " << tasks.size()
                  << " downloads complete." << Color::RESET << "\n";
    }
    return (failures > 0) ? 1 : 0;
}