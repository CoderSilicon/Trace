#ifndef SGET_NETWORK_HPP
#define SGET_NETWORK_HPP

#include <string>

struct NetworkResponse
{
    std::string html;
    std::string headers;
    long statusCode = 0;

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