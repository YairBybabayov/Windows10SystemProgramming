// ToolHelp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include <tlhelp32.h>
#include <iostream>

int main()
{
	/*
	typedef struct tagPROCESSENTRY32 {
		DWORD dwSize; // size of structure
		DWORD cntUsage; // unused
		DWORD th32ProcessID; // PID
		ULONG_PTR th32DefaultHeapID; // unused
		DWORD th32ModuleID; // unused
		DWORD cntThreads; // # threads
		DWORD th32ParentProcessID; // parent PID
		LONG pcPriClassBase; // Base priority
		DWORD dwFlags; // unused
		TCHAR szExeFile[MAX_PATH]; // Path
	} PROCESSENTRY32;
	*/
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		// handle error
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	if (!::Process32First(hSnapshot, &pe)) {
		// unlikely - handle error
	}
	do {
		printf("PID:%6d (PPID:%6d): %ws (Threads=%d) (Priority=%d)\n",
			pe.th32ProcessID, pe.th32ParentProcessID, pe.szExeFile,
			pe.cntThreads, pe.pcPriClassBase);
	} while (::Process32Next(hSnapshot, &pe));
	::CloseHandle(hSnapshot);

	return ERROR_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
