#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <format>

#pragma comment(lib, "psapi.lib")

class Profiler {
public:
    static SIZE_T getRAM() {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
            return pmc.WorkingSetSize; 
        }
        return 0;
    }

    static SIZE_T getCPU() {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(),
            (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
            return pmc.PrivateUsage; 
        }
        return 0;
    }

   static std::vector<std::string> getMemoryUsage() {
       return { std::format("RAM: {} MB",getRAM() / 1024 / 1024),
       std::format("CPU: {} MB",getCPU() / 1024 / 1024) };
    }
};
