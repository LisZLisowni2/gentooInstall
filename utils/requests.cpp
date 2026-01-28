#include "requests.hpp"
#include <cpr/cpr.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <regex>

std::vector<std::string> getFoldersFromServer(const std::string& url) {
    std::cout << "Download list of folders\n";
    cpr::Response response = cpr::Get(cpr::Url{url}, cpr::VerifySsl(false)); 
    if (response.status_code != 200) { 
        std::cerr << "Error: Failed to download list of folders. HTTP Status: " << response.status_code << "\n";
        std::cerr << "Response text: " << response.text << "\n";
        return {};
    }

    std::cout << "List of folders has been downloaded\n";
    std::vector<std::string> folders;

    std::cout << "Select folders with datetime\n";
    std::regex folderRegex(R"((\d{8}T\d{6}Z))");
    std::smatch match;
    std::string content = response.text;

    auto begin = std::sregex_iterator(content.begin(), content.end(), folderRegex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; it++) {
        folders.push_back(it->str());
    }

    std::cout << "Return datetimes\n";
    return folders;
}

std::tm parseDate(const std::string& folderName) {
    std::cout << "Parse the datetime of " << folderName << "\n";
    std::regex datePatern(R"((\d{4})(\d{2})(\d{2})T(\d{2})(\d{2})(\d{2})Z)");
    std::smatch match;
    std::tm tmDate = {};
    if (std::regex_search(folderName, match, datePatern)) {
        if (match.size() == 7) {
            tmDate.tm_year = std::stoi(match[1]) - 1900;
            tmDate.tm_mon = std::stoi(match[2]) - 1;
            tmDate.tm_mday = std::stoi(match[3]);
            tmDate.tm_hour = std::stoi(match[4]);
            tmDate.tm_min = std::stoi(match[5]);
            tmDate.tm_sec = std::stoi(match[6]);
        } else {
            std::cerr << "Match size unexcepted: " << match.size() << "\n";
        }
    } else {
        std::cerr << "Failed to parse folder date: " << folderName << "\n";
    }

    return tmDate;
}

bool isNewer(const std::string& folderName1, const std::string& folderName2) {
    std::cout << "Compare " << folderName1 << " and " << folderName2 << "\n";
    std::tm dataA = parseDate(folderName1);
    std::tm* dataAptr = &dataA; 
    std::tm dataB = parseDate(folderName2);
    std::tm* dataBptr = &dataB; 
    return std::mktime(dataAptr) > std::mktime(dataBptr);
}

std::string latestVersion(const bool& isARM) {
    std::vector<std::string> folders;
    if (!isARM) folders = getFoldersFromServer("https://gentoo.osuosl.org/releases/amd64/autobuilds/");
    else folders = getFoldersFromServer("https://gentoo.osuosl.org/releases/arm64/autobuilds/");
    if (folders.empty()) return "";

    std::cout << "Find the latest datetime\n";
    std::string latest = folders[0];
    for (const auto& folder : folders) {
        if (isNewer(folder, latest)) {
            latest = folder;
        }
    }

    std::cout << "Latest: " << latest << "\n";
    return latest;
}