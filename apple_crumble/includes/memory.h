#pragma once

#include <cstdio>
#include <windows.h>
#include <memory>
#include <tlhelp32.h>

class Memory
{
public:
	// Constructors, Destructors
	Memory();
	~Memory();

	// Member functions
	bool AttachProcess(const char* ProcessName);
	MODULEENTRY32 GetModule(const char* ModuleName);

	template<typename T>
	T Read(DWORD dwAddress) const;

	template<typename T>
	BOOL Write(DWORD dwAddress, T ValueToWrite) const;

	// Getters
	inline DWORD GetProcID() const { return this->dwPID; }
	inline HANDLE GetProcHandle() const { return this->hProcess; }

	// Public variables
	MODULEENTRY32 ClientDLL;
	MODULEENTRY32 EngineDLL;
	DWORD Client, ClientDLL_Size;
	DWORD Engine, EngineDLL_Size;

private:
	HANDLE hProcess;
	DWORD dwPID;
};

template<typename T>
inline T Memory::Read(DWORD dwAddress) const
{
	T val;
	ReadProcessMemory(this->hProcess, (LPVOID)dwAddress, &val, sizeof(T), NULL);
	return val;
}

template<typename T>
inline BOOL Memory::Write(DWORD dwAddress, T ValueToWrite) const
{
	return WriteProcessMemory(hProcess, (LPVOID)dwAddress, &ValueToWrite, sizeof(T), NULL);
}
