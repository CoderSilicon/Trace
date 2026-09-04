#ifndef SGET_DOWNLOAD_HPP
#define SGET_DOWNLOAD_HPP

#include <string>
#include <vector>

struct DownloadOptions
{
    std::vector<std::string> urls;
    std::string directory;     // -d <dir>: save into directory
    std::string filename;      // -o <name>: save as (single URL only)
    std::string userAgent = "sget/1.1";
    bool followRedirects = true;
    bool insecure = false;     // -k: skip TLS verification
    bool resume = false;       // -C: resume partial downloads
    int parallel = 4;          // -P <n>: max simultaneous downloads
};

int downloadAll(const DownloadOptions &opts);

#endif // SGET_DOWNLOAD_HPP