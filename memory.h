#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <vector>

void WriteToMemory(uintptr_t addressToWrite, const char* valuetoWrite, int byteNum);
MODULEINFO GetModuleInfo(const char* szModule);
DWORD FindPattern(const char* module, const char* pattern, const char* mask);
DWORD FindPattern(const char* module, const char* pattern, const char* mask, DWORD offset, DWORD extra);
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);