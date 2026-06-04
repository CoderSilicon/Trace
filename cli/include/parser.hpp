#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "network.hpp"

struct PageInfo
{
    std::string url;
    long statusCode;
    size_t sizeBytes;

    // Raw Data
    std::string rawHtml;
    std::string rawHeaders;

    // HTML Metadata
    std::string title;
    std::string description;
    std::string logoUrl;

    // HTTP Header Data
    std::string server;
    std::string contentType;

    // Telemetry
    std::string remoteIp;
    long remotePort;
    double totalTime;
    double dnsTime;
    double downloadSpeed;
    long redirectCount;

    std::vector<std::string> links;
};

PageInfo parseData(const NetworkResponse &netData, const std::string &url);

void printInfoMode(const PageInfo &info);
void printJsonMode(const PageInfo &info);
void printDetailedMode(const PageInfo &info);
void printLinksMode(const PageInfo &info);
void printSecurityMode(const PageInfo &info);
void printSelectMode(const PageInfo& info, const std::string& selector);
void printXPathMode(const PageInfo& info, const std::string& xpathExpr);

#endif