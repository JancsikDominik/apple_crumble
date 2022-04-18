#include "../includes/memory.h"
#include <iostream>

Memory::Memory()
{
	this->hProcess = NULL;
	this->dwPID = NULL;
	try 
	{
		// trying to find csgo.exe
		if (!AttachProcess("csgo.exe")) 
			throw 1;

		// getting the modules we need
		this->ClientDLL = GetModule("client.dll");
		this->EngineDLL = GetModule("engine.dll");
		this->Client = (DWORD)this->ClientDLL.modBaseAddr;
		this->Engine = (DWORD)this->EngineDLL.modBaseAddr;

		if (this->Client == 0x0) 
			throw 2;

		if (this->Engine == 0x0) 
			throw 3;

		// setting module sizes
		this->ClientDLL_Size = this->ClientDLL.dwSize;
		this->EngineDLL_Size = this->EngineDLL.dwSize;
	}
	// basic error handling
	catch (int iEx) 
	{
		switch (iEx)
		{
			case 1: 
				MessageBoxA(NULL, "CS:GO must be running", "ERROR", MB_ICONSTOP | MB_OK); exit(0); 
				break;
			case 2: 
				MessageBoxA(NULL, "Couldn't find Client.dll", "ERROR", MB_ICONSTOP | MB_OK); exit(0); 
				break;
			case 3: 
				MessageBoxA(NULL, "Couldn't find Engine.dll", "ERROR", MB_ICONSTOP | MB_OK); exit(0); 
				break;
			default: 
				throw;
		}
	}
	catch (...) 
	{
		MessageBoxA(NULL, "Unhandled exception thrown", "ERROR", MB_ICONSTOP | MB_OK);
		exit(0);
	}
}

Memory::~Memory()
{
	// checking if handle is valid
	if (this->hProcess != NULL)
	{
		// always close handles after using them
		CloseHandle(this->hProcess);
	}
}

bool Memory::AttachProcess(const char* ProcessName)
{
	// creating snapshot
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	// converting process nem from char* to WCHAR*
	const WCHAR* wchProcName;
	int nChars = MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, NULL, 0);
	wchProcName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, (LPWSTR)wchProcName, nChars);

	// looping through processes
	while (Process32Next(hSnapshot, &procEntry))
	{
		// checking if we found the process we are looking for (wcscmp ~ strcmp)
		if (!wcscmp(procEntry.szExeFile, wchProcName))
		{
			this->dwPID = procEntry.th32ProcessID;
			CloseHandle(hSnapshot);
			this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->dwPID);
			delete[] wchProcName;
			return true;
		}
	}

	CloseHandle(hSnapshot);
	delete[] wchProcName;
	return false;
}

MODULEENTRY32 Memory::GetModule(const char* ModuleName)
{
	// creating snapshot
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	// converting module name from char* to WCHAR*
	const WCHAR* wchModuleName;
	int nChars = MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, NULL, 0);
	wchModuleName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, (LPWSTR)wchModuleName, nChars);

	// looping through modules
	while (Module32Next(hSnapshot, &mEntry))
	{
		// checking if we found the module we are looking for
		if (!wcscmp(mEntry.szModule, wchModuleName))
		{
			CloseHandle(hSnapshot);
			delete[] wchModuleName;
			return mEntry;
		}
	}

	CloseHandle(hSnapshot);
	mEntry.modBaseAddr = 0x0;
	delete[] wchModuleName;
	return mEntry;
}
