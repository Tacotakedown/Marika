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

std::wstring GetSteamPathW() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::wcerr << L"Unable to open registry key" << std::endl;
        return L"";
    }

    wchar_t steamPath[MAX_PATH];
    DWORD bufferSize = sizeof(steamPath);
    if (RegQueryValueExW(hKey, L"InstallPath", NULL, NULL, reinterpret_cast<LPBYTE>(steamPath), &bufferSize) != ERROR_SUCCESS) {
        std::wcerr << L"Unable to read reg value" << std::endl;
        RegCloseKey(hKey);
        return L"";
    }

    RegCloseKey(hKey);
    return std::wstring(steamPath);
}

std::wstring FindEldenRingPath() {
    const std::wstring steamPath = GetSteamPathW(); // Assuming GetSteamPathW() returns std::wstring
    std::wstring configFilePath = steamPath + L"\\steamapps\\libraryfolders.vdf";
    std::wifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        std::wcerr << L"Unable to open libraryfolder.vdf" << std::endl;
        return L"";
    }

    std::wstring line;
    std::wregex pathRegex(L"\"path\"\\s+\"(.+?)\"");
    std::wregex appRegex(L"\"12345620\"");
    std::wsmatch match;
    std::wstring currentPath;

    while (std::getline(configFile, line)) {
        if (std::regex_search(line, match, pathRegex)) {
            currentPath = match[1].str();
        } else if (std::regex_search(line, appRegex) && !currentPath.empty()) {
            configFile.close();
            return currentPath + L"\\steamapps\\common\\ELDEN RING\\Game";
        }
    }
    configFile.close();
    return L"";
}

#endif //STEAMUTILS_H
