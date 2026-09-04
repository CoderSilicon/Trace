#include "progress.hpp"
#include "colors.hpp"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

// UTF-8 box-drawing characters
#define BOX_TL "\xE2\x94\x8C" // ┌
#define BOX_TR "\xE2\x94\x90" // ┐
#define BOX_BL "\xE2\x94\x94" // └
#define BOX_BR "\xE2\x94\x98" // ┘
#define BOX_H  "\xE2\x94\x80" // ─
#define BOX_V  "\xE2\x94\x82" // │
#define BOX_TM "\xE2\x94\x9C" // ├
#define BOX_BM "\xE2\x94\xA4" // ┤

#define BAR_FILL "\xE2\x96\x88"  // █
#define BAR_EMPTY "\xE2\x96\x91" // ░
#define DOT  "\xC2\xB7"          // ·
#define ELLIPSIS "\xE2\x80\xA6"  // …

// ------------------------------------------------------------- helpers

static std::string humanSize(double bytes)
{
    static const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int u = 0;
    double v = bytes;
    while (v >= 1024.0 && u < 4)
    {
        v /= 1024.0;
        ++u;
    }
    char buf[32];
    if (u == 0)
        std::snprintf(buf, sizeof buf, "%.0f %s", v, units[u]);
    else
        std::snprintf(buf, sizeof buf, "%.1f %s", v, units[u]);
    return std::string(buf);
}

// Visible character columns, ignoring ANSI escapes and counting one UTF-8
// lead byte as a single cell (all glyphs we use are single-width).
static size_t visibleWidth(const std::string &s)
{
    size_t w = 0;
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == '\x1b')
        {
            ++i; // '['
            while (i < s.size() && s[i] != 'm')
                ++i;
            continue;
        }
        if ((c & 0xC0) != 0x80)
            ++w; // count lead byte only
    }
    return w;
}

static std::string rpad(const std::string &s, int width)
{
    int pad = width - static_cast<int>(visibleWidth(s));
    if (pad < 0)
        pad = 0;
    return s + std::string(static_cast<size_t>(pad), ' ');
}

static std::string truncVisible(const std::string &s, int maxW)
{
    if (maxW < 1)
        return "";
    if (static_cast<int>(visibleWidth(s)) <= maxW)
        return s;

    std::string out;
    int w = 0;
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == '\x1b')
        {
            out += static_cast<char>(c);
            out += '['; // '[' is the next byte
            ++i;
            while (i < s.size() && s[i] != 'm')
                out += static_cast<char>(s[i++]);
            if (i < s.size())
                out += 'm';
            continue; // color codes don't consume width
        }
        if ((c & 0xC0) == 0x80)
        {
            out += static_cast<char>(c); // keep continuation bytes
            continue;
        }
        ++w;
        if (w > maxW)
        {
            out += ELLIPSIS;
            out += Color::RESET; // close any dangling color code
            return out;
        }
        out += static_cast<char>(c);
    }
    return out;
}

static std::string repeat(const std::string &s, int n)
{
    if (n <= 0)
        return "";
    std::string out;
    out.reserve(s.size() * static_cast<size_t>(n));
    for (int i = 0; i < n; ++i)
        out += s;
    return out;
}

static int terminalWidth()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(h, &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 100;
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
        return static_cast<int>(ws.ws_col);
    return 100;
#endif
}

// --------------------------------------------------------------- bars

static std::string barDeterminate(double frac, int width)
{
    int filled = static_cast<int>(std::lround(frac * static_cast<double>(width)));
    if (filled < 0)
        filled = 0;
    if (filled > width)
        filled = width;

    std::string out = "[";
    out += Color::GREEN + std::string(static_cast<size_t>(filled), ' ');
    for (int i = 0; i < filled; ++i)
        out += BAR_FILL;
    out += Color::RESET;
    out += Color::GRAY;
    for (int i = filled; i < width; ++i)
        out += BAR_EMPTY;
    out += Color::RESET + "]";
    return out;
}

static std::string barStream(int step, int width)
{
    if (step >= width)
        step = width - 1;
    if (step < 0)
        step = 0;

    std::string out = "[";
    for (int i = 0; i < width; ++i)
    {
        if (i == step)
            out += Color::YELLOW + std::string((i % 2 == 0) ? "C" : "c") + Color::RESET;
        else
            out += Color::GRAY + std::string(DOT) + Color::RESET;
    }
    out += "]";
    return out;
}

// --------------------------------------------------------- task rows

static char stateIcon(const DownloadTask &t)
{
    switch (t.state.load())
    {
    case DL_QUEUED:
        return 'o';
    case DL_ACTIVE:
        return '>';
    case DL_DONE:
        return '*';
    default:
        return 'x';
    }
}

static std::string stateIconColor(const DownloadTask &t)
{
    switch (t.state.load())
    {
    case DL_QUEUED:
        return Color::GRAY;
    case DL_DONE:
        return Color::GREEN;
    case DL_ERROR:
        return Color::RED;
    default:
        return Color::YELLOW;
    }
}

// One row of the panel, colourised, pre-truncated so it can never wrap.
static std::string taskRow(const DownloadTask &t, int nameW, int barW, int infoW)
{
    std::string name = truncVisible(t.path, nameW);
    int state = t.state.load();
    std::string infoPlain;

    std::string bar;
    if (state == DL_QUEUED)
    {
        bar = barDeterminate(0.0, barW);
        infoPlain = "queued";
    }
    else if (state == DL_ACTIVE)
    {
        long long expected = t.expected.load();
        long long done = t.done.load();
        double speed = t.speed.load();

        if (expected > 0)
        {
            double frac = static_cast<double>(done) / static_cast<double>(expected);
            int pct = static_cast<int>(frac * 100.0);
            if (pct > 100)
                pct = 100;
            bar = barDeterminate(frac, barW);

            char pctbuf[16];
            std::snprintf(pctbuf, sizeof pctbuf, "%3d%%", pct);
            infoPlain = std::string(pctbuf) + "  " + humanSize(static_cast<double>(done)) +
                        " / " + humanSize(static_cast<double>(expected)) +
                        "  " + humanSize(speed) + "/s";
        }
        else
        {
            int step = static_cast<int>((done / 8192) % barW);
            bar = barStream(step, barW);
            infoPlain = humanSize(static_cast<double>(done)) + "  " + humanSize(speed) + "/s";
        }
    }
    else if (state == DL_DONE)
    {
        bar = barDeterminate(1.0, barW);
        char buf[32];
        std::snprintf(buf, sizeof buf, "done %.1fs", t.elapsed);
        infoPlain = std::string("complete  ") + humanSize(static_cast<double>(t.bytes)) + "  " + buf;
    }
    else
    {
        bar = barDeterminate(0.0, barW);
        infoPlain = t.error;
    }

    std::string info = Color::GRAY + truncVisible(infoPlain, infoW) + Color::RESET;
    std::string content = stateIconColor(t) + std::string(1, stateIcon(t)) + Color::RESET +
                          " " + Color::BLUE + name + Color::RESET + "  " + bar + "  " + info;

    if (visibleWidth(content) > static_cast<size_t>(infoW + barW + nameW + 8))
        content = truncVisible(content, infoW + barW + nameW + 8);
    return content;
}

// ----------------------------------------------------------- frame

struct FrameStats
{
    size_t done = 0;
    size_t failed = 0;
    size_t active = 0;
    long long bytes = 0;
    double aggSpeed = 0.0;
};

static std::string lineIn(const std::string &content, int innerW)
{
    if (static_cast<int>(visibleWidth(content)) > innerW)
        return std::string(BOX_V) + rpad(truncVisible(content, innerW), innerW) + BOX_V;
    return std::string(BOX_V) + rpad(content, innerW) + BOX_V;
}

static std::string buildFrame(std::vector<DownloadTask> &tasks, FrameStats &stats,
                              const std::chrono::steady_clock::time_point &started, int parallel)
{
    int fullW = terminalWidth();
    if (fullW < 24)
        fullW = 24;
    int innerW = fullW - 2;

    int nameW = std::min(20, std::max(8, innerW / 3));
    int barW = innerW - nameW - 28;
    barW = std::max(6, std::min(32, barW));
    int infoW = innerW - nameW - barW - 8;
    if (infoW < 6)
        infoW = 6;

    stats = FrameStats{};
    for (const auto &t : tasks)
    {
        int s = t.state.load();
        if (s == DL_DONE)
        {
            ++stats.done;
            stats.bytes += t.done.load();
        }
        else if (s == DL_ERROR)
            ++stats.failed;
        else if (s == DL_ACTIVE)
        {
            ++stats.active;
            stats.bytes += t.done.load();
            stats.aggSpeed += t.speed.load();
        }
    }

    double elapsed = std::chrono::duration<double>(
                         std::chrono::steady_clock::now() - started)
                         .count();

    std::string frame;

    // top border with title
    std::string title = Color::BOLD + Color::CYAN +
                        " sget \xC2\xB7 " + std::to_string(tasks.size()) +
                        " file" + (tasks.size() == 1 ? "" : "s") +
                        " \xC2\xB7 parallel " + std::to_string(parallel) +
                        " " + Color::RESET;
    std::string top = std::string(BOX_TL) + title;
    int fill = innerW - static_cast<int>(visibleWidth(title));
    if (fill < 1)
        fill = 1;
    top += repeat(BOX_H, fill);
    top += Color::CYAN + std::string(BOX_TR);
    frame += top + "\n";

    // task rows
    for (const auto &t : tasks)
        frame += lineIn(taskRow(t, nameW, barW, infoW), innerW) + "\n";

    // divider
    frame += Color::CYAN + std::string(BOX_TM) +
             repeat(BOX_H, innerW) +
             std::string(BOX_BM) + Color::RESET + "\n";

    // summary line
    std::string sum = Color::GRAY + " total: " + Color::RESET +
                      Color::CYAN + humanSize(static_cast<double>(stats.bytes)) + Color::RESET +
                      Color::GRAY + " @ " + Color::RESET +
                      Color::CYAN + humanSize(stats.aggSpeed) + "/s" + Color::RESET +
                      Color::GRAY + "  \xC2\xB7  " + Color::RESET +
                      Color::GREEN + std::to_string(stats.done) + " done" + Color::RESET +
                      Color::GRAY + " \xC2\xB7 " + Color::RESET +
                      Color::YELLOW + std::to_string(stats.active) + " active" + Color::RESET +
                      Color::GRAY + " \xC2\xB7 " + Color::RESET +
                      Color::RED + std::to_string(stats.failed) + " failed" + Color::RESET +
                      Color::GRAY + "  \xC2\xB7  " + Color::RESET +
                      Color::CYAN + std::to_string(elapsed) + "s" + Color::RESET;
    frame += lineIn(sum, innerW) + "\n";

    // bottom border
    frame += Color::CYAN + std::string(BOX_BL) +
             repeat(BOX_H, innerW) +
             std::string(BOX_BR) + Color::RESET;

    return frame;
}

// ------------------------------------------------------- non-TTY line

static std::string compactLine(const DownloadTask &t)
{
    std::string out = stateIconColor(t) + std::string(1, stateIcon(t)) + Color::RESET;
    int s = t.state.load();
    if (s == DL_DONE)
    {
        char buf[48];
        std::snprintf(buf, sizeof buf, " %s  complete  %s in %.1fs",
                      t.path.c_str(), humanSize(static_cast<double>(t.bytes)).c_str(), t.elapsed);
        out += buf;
    }
    else if (s == DL_ERROR)
    {
        out += " " + t.path + "  " + t.error;
    }
    else
    {
        out += " " + t.path + "  " +
               humanSize(static_cast<double>(t.done.load())) + " / " +
               humanSize(static_cast<double>(t.expected.load())) + "  " +
               humanSize(t.speed.load()) + "/s";
    }
    return out;
}

// --------------------------------------------------------- renderer

void renderDownloads(std::vector<DownloadTask> &tasks, bool tty, int parallel)
{
    size_t n = tasks.size();
    auto started = std::chrono::steady_clock::now();
    FrameStats stats;

    if (!tty)
    {
        while (true)
        {
            size_t doneC = 0, failedC = 0, activeC = 0;
            long long bytes = 0;
            double spd = 0.0;
            for (const auto &t : tasks)
            {
                int s = t.state.load();
                if (s == DL_DONE)
                {
                    ++doneC;
                    bytes += t.done.load();
                }
                else if (s == DL_ERROR)
                    ++failedC;
                else if (s == DL_ACTIVE)
                {
                    ++activeC;
                    bytes += t.done.load();
                    spd += t.speed.load();
                }
            }
            if (doneC + failedC == n)
                break;

            std::cout << "\r" << Color::GRAY << "sget: " << doneC << " done, " << activeC
                      << " active, " << failedC << " failed | " << humanSize(static_cast<double>(bytes))
                      << " @ " << humanSize(spd) << "/s" << Color::RESET << "   " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "\n";
        for (const auto &t : tasks)
            std::cout << compactLine(t) << "\n";
        return;
    }

    const size_t frameRows = n + 4; // top + tasks + divider + summary + bottom
    bool first = true;
    while (true)
    {
        buildFrame(tasks, stats, started, parallel);
        if (stats.done + stats.failed == n)
            break;

        if (!first)
            std::cout << "\x1b[" << frameRows << "A";
        first = false;

        std::cout << buildFrame(tasks, stats, started, parallel) << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
    std::cout << "\n";
}