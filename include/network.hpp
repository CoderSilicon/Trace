#ifndef SGET_NETWORK_HPP
#define SGET_NETWORK_HPP

#include <string>

struct NetworkResponse
{
    std::string html;
    std::string headers;
    long statusCode = 0;

    // Transfer outcome: ok is true only when the HTTP transfer completed
    // successfully; error carries the libcurl message when it did not.
    bool ok = false;
    std::string error;

    // Low-level network stats
    std::string remoteIp;
    long remotePort = 0;
    double totalTime = 0.0;
    double dnsTime = 0.0;
    double downloadSpeed = 0.0;
    long redirectCount = 0;
};

NetworkResponse fetchWebpage(const std::string &url);

#endif // SGET_NETWORK_HPP