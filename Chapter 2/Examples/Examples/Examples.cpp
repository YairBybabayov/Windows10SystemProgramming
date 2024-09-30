// Examples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include <iostream>
#include<psapi.h>
#include <memory>

int main()
{
	int maxCount = 256;
	std::unique_ptr<DWORD[]> pids;
	int count = 0;
	for (;;) {
		pids = std::make_unique<DWORD[]>(maxCount);
		DWORD actualSize;
		if (!::EnumProcesses(pids.get(), maxCount * sizeof(DWORD), &actualSize))
			break;
		count = actualSize / sizeof(DWORD);
		if (count < maxCount)
			break;
		// need to resize
		maxCount *= 2;
	}
	// count is the number of processes
	for (int i = 0; i < count; i++) {
		DWORD pid = pids[i];
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
		if (!hProcess) {
			printf("Failed to open a handle to process %d (error=%d)\n",
				pid, ::GetLastError());
			continue;
		}
		FILETIME start = { 0 }, dummy;
		::GetProcessTimes(hProcess, &start, &dummy, &dummy, &dummy);
		SYSTEMTIME st;
		::FileTimeToLocalFileTime(&start, &start);
		::FileTimeToSystemTime(&start, &st);
		WCHAR exeName[MAX_PATH];
		DWORD size = MAX_PATH;
		DWORD count = ::QueryFullProcessImageName(hProcess, 0, exeName, &size);
		printf("PID: %5d, Start: %d/%d/%d %02d:%02d:%02d Image: %ws\n",
			pid, st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
			count > 0 ? exeName : L"Unknown");
	}


	/* const int MaxCount = 1024;
	DWORD pids[MaxCount];
	DWORD actualSize;
	if (::EnumProcesses(pids, sizeof(pids), &actualSize)) {
		// assume actualSize < sizeof(pids)
		int count = actualSize / sizeof(DWORD);
		for (int i = 0; i < count; i++) {
			// do something with pids[i]
			printf("%d\n", pids[i]);
		}
		printf("%d Processes are active now\n", count);

	} */
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
