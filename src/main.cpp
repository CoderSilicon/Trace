#ifndef SGET_VERSION
#define SGET_VERSION "2.0.1"
#endif

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <curl/curl.h>
#include "network.hpp"
#include "download.hpp"
#include "parser.hpp"
#include "colors.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void enableWindowsColors()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

enum class RunMode
{
    INFO,
    BINFO,
    JSON,
    SECURITY,
    SELECT,
    XPATH,
    LINKS,
    DOWNLOAD,
    INVALID
};

// 2. Configuration State
struct AppConfig
{
    std::vector<std::string> urls;
    std::string url = "";
    std::string outputFile = "";     // -o: save-as (download) or stdout redirect (inspect)
    std::string directory = "";      // -d/--dir: download directory
    std::string queryExpression = "";
    std::string userAgent = "sget/1.1";
    RunMode mode = RunMode::DOWNLOAD; // bare `sget <url>` downloads, curl-style
    bool exitEarly = false;
    bool resume = false;             // -C/--resume
    bool insecure = false;           // -k/--insecure
    int parallel = 4;                // -P/--parallel
};

// --- Terminal Commands ---

void printHelp()
{
    std::cout << "\n"
              << Color::CYAN << Color::BOLD << "sget" << Color::RESET
              << " - Meta Extractor & MOdern Frontend of Curl\n\n";

    std::cout << Color::YELLOW << "USAGE:" << Color::RESET << "\n";
    std::cout << "  sget [options] <URL> [<URL2> ...]\n\n";

    std::cout << Color::YELLOW << "DOWNLOAD (default):" << Color::RESET << "\n";

    std::cout << "  " << Color::GREEN << "sget https://.../file.zip" << Color::RESET << "\n";

    std::cout << "  " << Color::GREEN << "-o <name>" << Color::RESET << "     Save as <name> (single URL only)\n";
    std::cout << "  " << Color::GREEN << "-d <dir>" << Color::RESET << "     Save files into <dir>\n";
    std::cout << "  " << Color::GREEN << "-C" << Color::RESET << "           Resume a partial download\n";
    std::cout << "  " << Color::GREEN << "-k" << Color::RESET << "           Skip TLS certificate verification\n";
    std::cout << "  " << Color::GREEN << "-A <ua>" << Color::RESET << "     Custom User-Agent\n";
    std::cout << "  " << Color::GREEN << "-P <n>" << Color::RESET << "      Download <n> files in parallel (default 4)\n";
    std::cout << "  " << Color::GREEN << "--download" << Color::RESET << "  Download explicitly (same as default)\n\n";

    std::cout << Color::YELLOW << "INSPECT (one URL):" << Color::RESET << "\n";
    std::cout << "  " << Color::GREEN << "-i / --info" << Color::RESET << "       Page metadata, status, size\n";
    std::cout << "  " << Color::GREEN << "-bi / --binfo" << Color::RESET << "    Deep header + socket audit\n";
    std::cout << "  " << Color::GREEN << "-j / --json" << Color::RESET << "      Raw JSON output\n";
    std::cout << "  " << Color::GREEN << "-l / --links" << Color::RESET << "     All hyperlinks on the page\n";
    std::cout << "  " << Color::GREEN << "-s / --security" << Color::RESET << "  Security header scan\n";
    std::cout << "  " << Color::GREEN << "-sel <css>" << Color::RESET << "      Extract via CSS selector\n";
    std::cout << "  " << Color::GREEN << "-xp <expr>" << Color::RESET << "      Extract via XPath expression\n\n";

    std::cout << Color::YELLOW << "OTHER:" << Color::RESET << "\n";
    std::cout << "  " << Color::GREEN << "-o <file>" << Color::RESET << "   In inspect modes: write stdout to <file>\n";
    std::cout << "  " << Color::GREEN << "-h / --help" << Color::RESET << "   Show this manual\n";
    std::cout << "  " << Color::GREEN << "-v / --version" << Color::RESET << " Show version\n";
    std::cout << "  " << Color::GREEN << "--author" << Color::RESET << "     Display the dev\n\n";

    std::cout << Color::GRAY << "Examples:\n"
              << "  sget https://example.com/file.zip\n"
              << "  sget -d ~/downloads -P 8 url1 url2 url3\n"
              << "  sget -C -o resume.zip https://example.com/big.iso\n"
              << "  sget -i https://google.com\n"
              << Color::RESET << "\n";
}

void printVersion()
{
    std::cout << Color::CYAN << "sget " << Color::RESET
              << "version " << Color::BOLD << SGET_VERSION << Color::RESET << "\n";
}

void printAuthor()
{
    std::cout << "Engineered by " << Color::GREEN << Color::BOLD << "@coderSilicon" << Color::RESET
              << "\n"
              << Color::GRAY << "> It is always better to differ from others." << Color::RESET << "\n";
}

// Engine
AppConfig parseCLI(const std::vector<std::string> &args)
{
    AppConfig config;

    for (size_t i = 0; i < args.size(); ++i)
    {
        const std::string &arg = args[i];
        // Non-network modes
        if (arg == "--help" || arg == "-h")
        {
            printHelp();
            config.exitEarly = true;
            return config;
        }
        else if (arg == "--version" || arg == "-v")
        {
            printVersion();
            config.exitEarly = true;
            return config;
        }
        else if (arg == "--author" || arg == "-aCs")
        {
            printAuthor();
            config.exitEarly = true;
            return config;
        }
        if (arg == "--select" || arg == "-sel")
        {
            config.mode = RunMode::SELECT;
            if (i + 1 < args.size())
            {
                config.queryExpression = args[++i];
            }
            else
            {
                std::cerr << Color::RED << "Error: --select requires a CSS selector query.\n" << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        else if (arg == "--x-path" || arg == "-xp")
        {
            config.mode = RunMode::XPATH;
            if (i + 1 < args.size())
            {
                config.queryExpression = args[++i];
            }
            else
            {
                std::cerr << Color::RED << "Error: --x-path requires an XPath expression.\n" << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        else if (arg == "--info" || arg == "-i")
        {
            config.mode = RunMode::INFO;
        }
        else if (arg == "--binfo" || arg == "-bi")
        {
            config.mode = RunMode::BINFO;
        }
        else if (arg == "--json" || arg == "-j")
        {
            config.mode = RunMode::JSON;
        }
        else if (arg == "--links" || arg == "-l")
        {
            config.mode = RunMode::LINKS;
        }
        else if (arg == "--security" || arg == "-s")
        {
            config.mode = RunMode::SECURITY;
        }
        else if (arg == "--download")
        {
            config.mode = RunMode::DOWNLOAD;
        }
        else if (arg == "--dir" || arg == "-d")
        {
            if (i + 1 < args.size() && args[i + 1].rfind("-", 0) != 0)
            {
                config.directory = args[++i];
            }
            else
            {
                std::cerr << Color::RED << "Error: --dir requires a directory argument.\n" << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        else if (arg == "--resume" || arg == "-C")
        {
            config.resume = true;
        }
        else if (arg == "--insecure" || arg == "-k")
        {
            config.insecure = true;
        }
        else if (arg == "--user-agent" || arg == "-A")
        {
            if (i + 1 < args.size() && args[i + 1].rfind("-", 0) != 0)
            {
                config.userAgent = args[++i];
            }
            else
            {
                std::cerr << Color::RED << "Error: --user-agent requires a UA string.\n" << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        else if (arg == "--parallel" || arg == "-P")
        {
            if (i + 1 < args.size() && args[i + 1].rfind("-", 0) != 0)
            {
                try
                {
                    config.parallel = std::stoi(args[++i]);
                }
                catch (...)
                {
                    std::cerr << Color::RED << "Error: --parallel requires a number.\n" << Color::RESET;
                    config.mode = RunMode::INVALID;
                    config.exitEarly = true;
                    return config;
                }
                if (config.parallel < 1)
                    config.parallel = 1;
            }
            else
            {
                std::cerr << Color::RED << "Error: --parallel requires a number.\n" << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        else if (arg == "--output" || arg == "-o")
        {
            if (i + 1 < args.size())
            {
                config.outputFile = args[++i]; // Grab the next argument and increment i
            }
            else
            {
                std::cerr << Color::RED << "Error: --output requires a filename argument.\n"
                          << Color::RESET;
                config.mode = RunMode::INVALID;
                config.exitEarly = true;
                return config;
            }
        }
        // Catch unknown flags
        else if (arg.rfind("--", 0) == 0 || arg.rfind("-", 0) == 0)
        {
            std::cerr << Color::RED << "Error: Unknown flag '" << arg << "'\n"
                      << Color::RESET;
            std::cerr << "Run " << Color::GREEN << "sget --help" << Color::RESET << " for valid options.\n";
            config.mode = RunMode::INVALID;
            config.exitEarly = true;
            return config;
        }
        else
        {
            config.urls.push_back(arg);
            if (config.url.empty())
                config.url = arg;
        }
    }
    return config;
}

// Main
int main(int argc, char *argv[])
{
    enableWindowsColors();
    curl_global_init(CURL_GLOBAL_ALL);

    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty())
    {
        std::cerr << Color::RED << "Error: No target specified.\n"
                  << Color::RESET;
        std::cerr << "Usage: sget [flag] <URL>\n";
        return 1;
    }

    AppConfig config = parseCLI(args);

    if (config.exitEarly)
    {
        return 0;
    }

    if (config.urls.empty())
    {
        std::cerr << Color::RED << "Error: Missing target URL.\n"
                  << Color::RESET;
        return 1;
    }

    if (config.mode == RunMode::DOWNLOAD)
    {
        if (!config.outputFile.empty() && config.urls.size() > 1)
        {
            std::cerr << Color::RED << "Error: -o/--output can only be used with a single URL.\n"
                      << Color::RESET;
            return 1;
        }

        DownloadOptions opts;
        opts.urls = config.urls;
        opts.directory = config.directory;
        opts.filename = config.outputFile;
        opts.userAgent = config.userAgent;
        opts.insecure = config.insecure;
        opts.resume = config.resume;
        opts.parallel = config.parallel;
        return downloadAll(opts);
    }

    if (config.urls.size() > 1)
    {
        std::cerr << Color::YELLOW << "Warning: inspect modes use only the first URL.\n" << Color::RESET;
    }

    NetworkResponse response = fetchWebpage(config.urls[0]);

    if (!response.ok)
    {
        std::cerr << Color::RED << "Fatal: Could not fetch " << config.urls[0]
                  << " (" << response.error
                  << "). The rest can't be fetched." << Color::RESET << "\n";
        curl_global_cleanup();
        return 1;
    }

    if (response.html.empty() && response.headers.empty())
    {
        std::cerr << Color::RED << "Fatal: Received empty response from "
                  << config.urls[0] << ". The rest can't be fetched." << Color::RESET << "\n";
        curl_global_cleanup();
        return 1;
    }

    if (response.statusCode >= 400)
    {
        std::cerr << Color::RED << "HTTP error " << response.statusCode
                  << " for " << config.urls[0] << Color::RESET << "\n";
    }

    PageInfo info = parseData(response, config.urls[0]);

    std::ofstream outFile;
    std::streambuf *originalCoutBuffer = std::cout.rdbuf(); // Save the terminal buffer

    if (!config.outputFile.empty())
    {
        outFile.open(config.outputFile, std::ios::out | std::ios::trunc);
        if (outFile.is_open())
        {
            // Redirect all standard std::cout calls directly to the hard drive
            std::cout.rdbuf(outFile.rdbuf());
        }
        else
        {
            std::cerr << Color::RED << "Error: Could not open " << config.outputFile << " for writing.\n"
                      << Color::RESET;
            curl_global_cleanup();
            return 1;
        }
    }

    switch (config.mode)
    {
    case RunMode::INFO:
        printInfoMode(info);
        break;
    case RunMode::BINFO:
        printDetailedMode(info);
        break;
    case RunMode::JSON:
        printJsonMode(info);
        break;
    case RunMode::SECURITY:
        printSecurityMode(info);
        break;
    case RunMode::SELECT:
        printSelectMode(info, config.queryExpression); // <-- Route to Parser
        break;
    case RunMode::XPATH:
        printXPathMode(info, config.queryExpression);  // <-- Route to Parser
        break;
    case RunMode::LINKS:
        printLinksMode(info);
        break;
    case RunMode::INVALID:
    default:
        return 1;
    }

    if (outFile.is_open())
    {
        // Give cout back to the terminal so the program exits cleanly
        std::cout.rdbuf(originalCoutBuffer);
        outFile.close();
        std::cout << Color::GREEN << "Successfully wrote output to: " << config.outputFile << Color::RESET << "\n";
    }

    return (response.statusCode >= 400) ? 1 : 0;
}