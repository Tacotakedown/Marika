//
// Created by Taco on 7/3/2024.
//

#ifndef STEAMUTILS_H
#define STEAMUTILS_H

#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <iostream>
#include <filesystem>
#include <windows.h>

std::string GetSteamPath() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "Unable to open registry key" << std::endl;
        return "";
    }
    char steamPath[MAX_PATH];
    DWORD bufferSize = sizeof(steamPath);
    if (RegQueryValueEx(hKey, "InstallPath", NULL,NULL, (LPBYTE) steamPath, &bufferSize) != ERROR_SUCCESS) {
        std::cerr << "unable to read reg value" << std::endl;
        RegCloseKey(hKey);
        return "";
    }

    RegCloseKey(hKey);
    return std::string(steamPath);
}

std::string FindEldenRingPath(const std::string &steamPath) {
    std::string configFilePath = steamPath + "\\steamapps\\libraryfolders.vdf";
    std::ifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        std::cerr << "unable to open libraryfolder.vdf" << std::endl;
        return "";
    }

    std::string line;
    std::regex pathRegex("\"path\"\\s+\"(.+?)\"");
    std::regex appRegex("\"12345620\"");
    std::smatch match;
    std::string currentPath;

    while (std::getline(configFile, line)) {
        if (std::regex_search(line, match, pathRegex)) {
            currentPath = match[1].str();
        } else if (std::regex_search(line, appRegex) && !currentPath.empty()) {
            configFile.close();
            return currentPath + "\\steamapps\\common\\ELDEN RING\\Game";
        }
    }
    configFile.close();
    return "";
}

#endif //STEAMUTILS_H
