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


std::wstring GetProcessName(DWORD processID) {
    std::wstring processName;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess) {
        WCHAR buffer[MAX_PATH];
        if (GetModuleBaseName(hProcess, NULL, buffer, MAX_PATH)) {
            processName = buffer;
        }
        CloseHandle(hProcess);
    }
    return processName;
}

DWORD FindProcessID(const std::wstring &processName) {
    DWORD processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (processName == pe.szExeFile) {
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


#endif //LAUNCHER_H
