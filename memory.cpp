#include "memory.h"

void WriteToMemory(uintptr_t addressToWrite, const char* valuetoWrite, int byteNum)
{
	DWORD oldProtection;
	VirtualProtect((LPVOID)addressToWrite, byteNum, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy((void*)addressToWrite, valuetoWrite, byteNum);
	VirtualProtect((LPVOID)addressToWrite, byteNum, oldProtection, &oldProtection);
}

MODULEINFO GetModuleInfo(const char* szModule)
{
	MODULEINFO modInfo{};
	HMODULE hModule{ GetModuleHandle(szModule) };
	if (hModule == 0)
		return modInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

DWORD FindPattern(const char* module,const char* pattern,const char* mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}


DWORD FindPattern(const char* module, const char* pattern, const char* mask, DWORD offset, DWORD extra)
{
	DWORD addr = FindPattern(module, pattern, mask);
	addr += offset;
	return *(DWORD*)addr + extra;
}

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}