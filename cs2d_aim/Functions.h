#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>


//We make Length at the end optional as most jumps will be 5 or less bytes
/*void PlaceJMP(BYTE* Address, DWORD jumpTo, DWORD length = 5)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	//give that address read and write permissions and store the old permissions at oldProtection
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// Calculate the "distance" we're gonna have to jump - the size of the JMP instruction
	dwRelAddr = (DWORD)(jumpTo - (DWORD)Address) - 5;

	// Write the JMP opcode @ our jump position...
	*Address = 0xE9;

	// Write the offset to where we're gonna jump
	*((DWORD*)(Address + 0x1)) = dwRelAddr;

	// Overwrite the rest of the bytes with NOPs
	for (DWORD x = 0x5; x < length; x++)
		*(Address + x) = 0x90;

	// Restore the default permissions
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}*/


MODULEINFO GetModuleInfo(const char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}
/*
DWORD FindPattern(char* module, char* pattern, char* mask)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	//Get length of pattern by strlengthing the mask
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		//if still found, return the memory addy so we can write to it
		if (found)
		{
			return base + i;
		}
	}
	return NULL;
}
*/
DWORD getModuleBase(const char* module)
{
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	return base;
}

bool Hook(void* toHook, void* ourFunct, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD  curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	uintptr_t  relativeAddress = ((uintptr_t)ourFunct - (uintptr_t)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(uintptr_t*)((uintptr_t)toHook + 1) = relativeAddress;

	DWORD  temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}


void* TrampolineHook(void* toHook, void* ourFunct, int len)
{
	// Make sure the length is greater than 5
	if (len < 5) {
		return NULL;
	}

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	void* gateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// Put the bytes that will be overwritten in the gateway
	memcpy(gateway, toHook, len);

	// Get the gateway to destination addy
	uintptr_t  gateJmpAddy = ((uintptr_t)toHook - (uintptr_t)gateway) - 5;

	// Add the jmp opcode to the end of the gateway
	*(BYTE*)((uintptr_t)gateway + len) = 0xE9;

	// Add the address to the jmp
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gateJmpAddy;

	// Place the hook at the destination
	Hook(toHook, ourFunct, len);

	return gateway;
}



#endif