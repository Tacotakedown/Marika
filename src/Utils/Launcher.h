//
// Created by Taco on 7/3/2024.
//

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <mutex>
#include <fwpmu.h>
#include <sddl.h>


#include "DeltaTime.h"


// std::wstring GetProcessName(DWORD processID) {
//     std::wstring processName;
//     HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
//     if (hProcess) {
//         WCHAR buffer[MAX_PATH];
//         if (GetModuleBaseName(hProcess, NULL, buffer, MAX_PATH)) {
//             processName = buffer;
//         }
//         CloseHandle(hProcess);
//     }
//     return processName;
// }

std::wstring StringToWString(const char *str) {
    size_t len = strlen(str);
    std::wstring wstr(len, L' ');
    mbstowcs(&wstr[0], str, len);
    return wstr;
}

DWORD FindProcessID(const std::wstring &processName) {
    DWORD processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                std::wstring exeFileName = StringToWString(pe.szExeFile);
                if (processName == exeFileName) {
                    processID = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    return processID;
}

void MonitorProcess(DWORD processID, std::mutex &mutex, bool &flag) {
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processID);
    if (hProcess) {
        WaitForSingleObject(hProcess, INFINITE);
        CloseHandle(hProcess);
    }
    std::lock_guard<std::mutex> lock(mutex);
    flag = true;
}

void blockAllNetworkRequests(DWORD pid) {
    HANDLE engineHandle = NULL;
    DWORD result = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &engineHandle);
    if (result != ERROR_SUCCESS) {
        wprintf(L"FwpmEngineOpen0 Failed with error: %u\n", result);
        return;
    }

    FWPM_FILTER_CONDITION0 Condition = {0};
    Condition.fieldKey = FWPM_CONDITION_ALE_APP_ID;
    Condition.matchType = FWP_MATCH_EQUAL;
    Condition.conditionValue.type = FWP_UINT32;
    Condition.conditionValue.uint32 = pid;

    FWPM_FILTER0 filter = {0};
    filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
    const wchar_t *name = L"Marika Network blocker";
    filter.displayData.name = const_cast<wchar_t *>(name);
    const wchar_t *desc = L"Blocks all network requests from Elden Ring while launching to avoid any checks on old seamless co op versions";
    filter.displayData.description = const_cast<wchar_t *>(desc);
    filter.action.type = FWP_ACTION_BLOCK;
    filter.filterCondition = &Condition;
    filter.numFilterConditions = 1;
    filter.weight.type = FWP_EMPTY;

    UINT64 filterId;
    result = FwpmFilterAdd0(engineHandle, &filter, NULL, &filterId);
    if (result != ERROR_SUCCESS) {
        FwpmEngineClose0(engineHandle);
        return;
    }

    Sleep(20000);

    result = FwpmFilterDeleteById0(engineHandle, filterId);


    FwpmEngineClose0(engineHandle);

}

void FindAndStartMonitor(std::mutex &mtx, bool &gameExited) {
    DWORD gamePID = 0;
    DeltaTime *deltaTime = new DeltaTime();
    float accumTime = 0.0f;
    float startTime = 0.0f;
    while (gamePID == 0) {

        gamePID = FindProcessID(L"eldenring.exe");
        Sleep(1);

    }


    blockAllNetworkRequests(gamePID);

    MonitorProcess(gamePID, mtx, gameExited);
    delete deltaTime;
}

bool CheckIntegrityOfModEngine(const std::wstring &folderPath) {
    const std::vector<std::wstring> modEngineFiles = {
            L"config_eldenring.toml",
            L"modengine2_launcher.exe"


    };

    for (const auto &fileName: modEngineFiles) {
        std::wstring fullPath = folderPath + L"\\" + fileName;
        if (!std::filesystem::exists(fullPath)) {
            return false;
        }
    }
    return true;
}


#endif //LAUNCHER_H
