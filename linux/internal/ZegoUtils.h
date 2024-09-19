#pragma once

#include <limits.h>
#ifdef _WIN32
#include <flutter/plugin_registrar_windows.h>
#elif defined(__linux__)
#include <flutter_linux/flutter_linux.h>
#endif

std::string GetFlutterAssetsPath() {
    std::string assetPath = "";
#ifdef _WIN32
    wchar_t exePath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exePathStrW{exePath};
    std::string exePathStr(exePathStrW.begin(), exePathStrW.end());
    exePathStr = std::string(exePathStr, 0, exePathStr.find_last_of("\\"));
    if (!exePathStr.empty()) {
        assetPath = exePathStr + "\\data\\flutter_assets\\";
    }
#elif defined(__linux__)
    char proc_exe[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", proc_exe, sizeof(proc_exe) - 1);
    if (len != -1) {
        proc_exe[len] = '\0';
        std::string exePathStr(proc_exe);
        if (!exePathStr.empty()) {
            assetPath = exePathStr + "/data/flutter_assets/";
        }
    }
#endif
    return assetPath;
}