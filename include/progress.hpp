#ifndef SGET_PROGRESS_HPP
#define SGET_PROGRESS_HPP

#include <atomic>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>

enum DownloadState
{
    DL_QUEUED = 0,
    DL_ACTIVE = 1,
    DL_DONE = 2,
    DL_ERROR = 3
};

// One in-flight transfer. Shared between the worker thread that performs the
// transfer (task->state, task->done, task->expected via the curl progress
// callback) and the renderer thread that draws the progress panel.
struct DownloadTask
{
    std::string url;
    std::string path;
    std::string error;

    std::atomic<long long> expected{0};
    std::atomic<long long> done{0};
    std::atomic<double> speed{0.0};
    std::atomic<int> state{DL_QUEUED};

    long long bytes = 0;    // final size (set on completion)
    double elapsed = 0.0;   // wall time of the transfer
    bool printed = false;   // final line already emitted (non-TTY)

    // internals owned by the worker thread
    long long resumeOffset = 0;
    long long lastDone = 0;
    std::chrono::steady_clock::time_point lastTick;
    std::ofstream out;
};

// Draws the animation until every task reaches a terminal state.
void renderDownloads(std::vector<DownloadTask> &tasks, bool terminal, int parallel);

#endif // SGET_PROGRESS_HPP