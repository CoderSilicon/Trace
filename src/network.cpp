// Add this near your other includes at the top
#include "network.hpp"
#include <curl/curl.h>
#include <iostream>
#include "colors.hpp"
#include <fstream>
#include <iomanip>
#include <cstdio>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    ((std::string *)userp)->append((char *)contents, totalSize);
    return totalSize;
}

static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t totalSize = size * nitems;
    ((std::string *)userdata)->append(buffer, totalSize);
    return totalSize;
}

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

static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    if (dlnow == 0)
        return 0;

    const int totalDots = 25;
    static int chompTick = 0;
    chompTick++;

    if (dltotal <= 0)
    {
        int step = static_cast<int>(dlnow / 8192) % totalDots;
        char pacman = (chompTick % 4 < 2) ? 'C' : 'c';

        std::string track = "[";
        for (int i = 0; i < totalDots; ++i)
        {
            if (i == step)
                track += pacman;
            else
                track += "•";
        }
        track += "]";

        std::cout << "\r" << Color::YELLOW << track << Color::RESET
                  << " Streaming: " << (dlnow / 1024) << " KB..." << std::flush;
        return 0;
    }

    double progress = static_cast<double>(dlnow) / static_cast<double>(dltotal);
    int percentage = static_cast<int>(progress * 100);

    int pacmanPos = static_cast<int>(progress * totalDots);
    if (pacmanPos >= totalDots)
        pacmanPos = totalDots - 1;

    char pacmanChar = (percentage == 100) ? 'C' : ((chompTick % 6 < 3) ? 'C' : 'c');

    std::string track = "[";

    for (int i = 0; i < pacmanPos; ++i)
    {
        track += " ";
    }

    track += pacmanChar;

    for (int i = pacmanPos + 1; i < totalDots; ++i)
    {
        track += "•";
    }
    track += "]";

    std::cout << "\r" << Color::YELLOW << track << Color::RESET
              << " " << std::setw(3) << percentage << "%"
              << " [" << (dlnow / 1024 / 1024) << "/" << (dltotal / 1024 / 1024) << " MB]" << std::flush;

    return 0;
}

NetworkResponse fetchWebpage(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    NetworkResponse response;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.html);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "sget/1.0");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "Network error: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            // Extract HTTP Status
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.statusCode);

            char *ip;
            curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip);
            if (ip)
                response.remoteIp = ip;

            curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &response.remotePort);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response.totalTime);
            curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &response.dnsTime);
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &response.downloadSpeed);
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &response.redirectCount);
        }

        curl_easy_cleanup(curl);
    }
    return response;
}

bool downloadFile(const std::string &url, const std::string &outputPath)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return false;

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open())
    {
        std::cerr << Color::RED << "Error: Could not open output file stream." << Color::RESET << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

    // [FIX]: Force libcurl to FAIL instantly on 404/500 errors instead of downloading a fake HTML file!
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "sget/1.0");

    CURLcode res = curl_easy_perform(curl);

    outFile.close();

    if (res != CURLE_OK)
    {
        std::cerr << "\n"
                  << Color::RED << "Download failed: " << curl_easy_strerror(res) << Color::RESET << "\n";
        std::remove(outputPath.c_str()); // Deletes the garbage file from your disk
    }

    curl_easy_cleanup(curl);
    return (res == CURLE_OK);
}
