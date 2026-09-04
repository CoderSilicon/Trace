#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <curl/curl.h>
#include "network.hpp"
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
    std::string url = "";
    std::string outputFile = "";
    std::string queryExpression = "";
    RunMode mode = RunMode::INFO;
    bool exitEarly = false;
};

// --- Terminal Commands ---

void printHelp()
{
    std::cout << "\n"
              << Color::CYAN << Color::BOLD << "sget" << Color::RESET
              << " - Web Data Extractor\n\n";

    std::cout << Color::YELLOW << "USAGE:" << Color::RESET << "\n";
    std::cout << "  sget [options] <URL>\n\n";

    std::cout << Color::YELLOW << "Working flags:" << Color::RESET << "\n";
    std::cout << "  " << Color::GREEN << "--info" << Color::RESET << "      Standard metadata extraction (Default)\n";
    std::cout << "  " << Color::GREEN << "--binfo" << Color::RESET << "     Deep and detailed header extraction\n";
    std::cout << "  " << Color::GREEN << "--json" << Color::RESET << "      Raw JSONextraction\n";
    std::cout << "  " << Color::GREEN << "--links" << Color::RESET << "     Extract all hyperlinks from the page\n\n";
    std::cout << "  " << Color::GREEN << "--download" << Color::RESET << "  Download the target asset with visual progress\n\n";
    std::cout << "  " << Color::GREEN << "--select <query>" << Color::RESET << "  Extract raw data via CSS selectors\n";
    std::cout << "  " << Color::GREEN << "--x-path <expr>" << Color::RESET << "  Extract raw data via XPath expressions\n\n";

    std::cout << Color::YELLOW << "Default Flags:" << Color::RESET << "\n";
    std::cout << "  " << Color::GREEN << "--help" << Color::RESET << "      Manual\n";
    std::cout << "  " << Color::GREEN << "--version" << Color::RESET << "   Display core compiler version\n";
    std::cout << "  " << Color::GREEN << "--author" << Color::RESET << "    Display the dev\n\n";

    std::cout << Color::GRAY << "Example: sget --json https://google.com" << Color::RESET << "\n\n";
}

void printVersion()
{
    std::cout << Color::CYAN << "sget " << Color::RESET
              << "version " << Color::BOLD << "1.0.0-core" << Color::RESET << "\n";
}

void printAuthor()
{
    std::cout << "Engineered by " << Color::GREEN << Color::BOLD << "@coderSilicon" << Color::RESET
              << "\n"
              << Color::GRAY << "> It is always better to differ from others." << Color::RESET << "\n";
}

void triggerMatrix()
{
    std::cout << Color::GREEN << "01101001 01101110 01110100 01101001 01100001 01101100 01101001 01111010 01101001 01101110 01100111 00100000 01101101 01100001 01110100 01110010 01101001 01111000." << Color::RESET << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    uintptr_t baseAddr = 0x7fff5fbff000;

    for (int i = 0; i < 120; ++i)
    {
        std::cout << Color::GREEN << "0x" << std::hex << baseAddr << "  " << Color::RESET;
        baseAddr += 16;

        for (int j = 0; j < 8; ++j)
        {
            int rawByte = rand() % 256;
            if (rawByte < 16)
                std::cout << "0";
            std::cout << std::hex << rawByte << " ";
        }

        std::cout << Color::GRAY << "| " << Color::RESET;

        for (int j = 0; j < 12; ++j)
        {

            char printableChar = (rand() % 94) + 33;
            if (printableChar == '[' || printableChar == ']' || printableChar == '%')
            {
                printableChar = '.';
            }
            std::cout << Color::GREEN << printableChar << Color::RESET;
        }

        std::cout << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }

    std::cout << Color::GREEN << Color::BOLD << "\n[SUCCESS] FATAL CORE DUMP COMPLETED. STAY IN THE SIMULATION.\n\n"
              << Color::RESET;
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
        //else if (arg == "--matrix" || "-mmen")
        //{
        //    config.mode = RunMode::MATRIX;
        //}
        // True Modes
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
        else if (arg == "--download" || arg == "-d")
        { // <-- Add this block
            config.mode = RunMode::DOWNLOAD;
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

    if (config.url.empty())
    {
        std::cerr << Color::RED << "Error: Missing target URL.\n"
                  << Color::RESET;
        return 1;
    }

    if (config.mode == RunMode::DOWNLOAD)
    {
        // Extract a basic filename from the URL, or default to "downloaded_file.out"
        std::string filename = "downloaded_file.out";
        size_t pos = config.url.find_last_of('/');
        if (pos != std::string::npos && pos + 1 < config.url.length())
        {
            filename = config.url.substr(pos + 1);
        }

        std::cout << Color::CYAN << "Initializing download for: " << Color::RESET << filename << "\n\n";

        bool success = downloadFile(config.url, filename);
        if (success)
        {
            std::cout << "\n\n"
                      << Color::GREEN << Color::BOLD << "Download Complete: "
                      << Color::RESET << filename << "\n";
        }
        return success ? 0 : 1;
    }


    NetworkResponse response = fetchWebpage(config.url);

    if (response.html.empty() && response.headers.empty())
    {
        std::cerr << Color::RED << "Fatal: Failed to fetch or received empty response." << Color::RESET << "\n";
        return 1;
    }

    PageInfo info = parseData(response, config.url);

    std::ofstream outFile;
    std::streambuf *originalCoutBuffer = std::cout.rdbuf(); // Save the terminal buffer

    if (!config.outputFile.empty())
    {
        outFile.open(config.outputFile, std::ios::out | std::ios::app);
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
        return 1;
    }

    if (outFile.is_open())
    {
        // Give cout back to the terminal so the program exits cleanly
        std::cout.rdbuf(originalCoutBuffer);
        outFile.close();
        std::cout << Color::GREEN << "Successfully wrote output to: " << config.outputFile << Color::RESET << "\n";
    }

    return 0;
}