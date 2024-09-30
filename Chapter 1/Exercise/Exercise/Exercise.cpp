// HelloWin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define BUILD_WINDOWS
#include <windows.h>
#include <stdio.h>
#include <versionhelpers.h>

int main() {

	/* GetNativeSystemInfo() function */
	SYSTEM_INFO si;
	::GetNativeSystemInfo(&si);
	printf("Number of Logical Processors: %d\n", si.dwNumberOfProcessors);
	printf("Page size: %d Bytes\n", si.dwPageSize);
#ifdef _WIN64
	printf("Processor Mask: 0x%016llX\n", si.dwActiveProcessorMask);
#else
	printf("Processor Mask: 0x%08X\n", si.dwActiveProcessorMask);
#endif
	printf("Minimum process address: 0x%p\n", si.lpMinimumApplicationAddress);
	printf("Maximum process address: 0x%p\n", si.lpMaximumApplicationAddress);
	printf("Processor type: %d\n", si.dwProcessorType);
	printf("Starting address at which virtual memory can be allocated: %d\n", si.dwAllocationGranularity);
	printf("Processor level: %d\n", si.wProcessorLevel);
	printf("Processor revision: %d\n", si.wProcessorRevision);
	printf("Processor Architecture: %d\n", si.wProcessorArchitecture);

	/* GetSystemDirectory() function */
	WCHAR path[MAX_PATH];
	::GetSystemDirectory(path, MAX_PATH);
	printf("System directory: %ws\n", path);

	/* GetVersionEx() function - not updated from windows 8 and above */
	OSVERSIONINFOEX vi = { sizeof(vi) };
	::GetVersionEx((OSVERSIONINFO*)&vi);
	printf("Version: %d.%d.%d\n",
		vi.dwMajorVersion, vi.dwMinorVersion, vi.dwBuildNumber);

	/* IsWindows8Point1OrGreater() function - not working versionhelpers.h */
	if (IsWindows8Point1OrGreater())
		printf("Nice\n");

	/* KUSER_SHARED_DATA cheat function */
	auto sharedUserData = (BYTE*)0x7FFE0000;
	printf("Version: %d.%d.%d\n",
		*(ULONG*)(sharedUserData + 0x26c), // major version offset
		*(ULONG*)(sharedUserData + 0x270), // minor version offset
		*(ULONG*)(sharedUserData + 0x260)); // build number offset (Windows 10)

	/* GetComputerNameW() function */
	wchar_t cn[MAX_COMPUTERNAME_LENGTH + 1]; // Buffer to store the computer name
	DWORD s = MAX_COMPUTERNAME_LENGTH + 1;   // Size of the buffer

	if (::GetComputerNameW(cn, &s)) { // Get the computer name (wide char version)
		wprintf(L"Computer Name: %ws\n", cn);
	}
	else {
		wprintf(L"Failed to get computer name. Error code: %ld\n", GetLastError());
	}

	/* GetWindowsDirectory() function */
	::GetWindowsDirectoryW(path, MAX_PATH);
	printf("Windows directory: %ws\n", path);

	/* QueryPerformanceCounter() function */
	LARGE_INTEGER lpPerformanceCount;
	::QueryPerformanceCounter(&lpPerformanceCount);
	printf("Query Performance Counter: %d\n", lpPerformanceCount);

	/* GetProductInfo() function */
	DWORD pdwReturnedProductType;
	DWORD dwSpMajorVersion = vi.wServicePackMajor;
	DWORD dwSpMinorVersion = vi.wServicePackMinor;
	::GetProductInfo(
		vi.dwMajorVersion,
		vi.dwMinorVersion,
		dwSpMajorVersion,
		dwSpMinorVersion,
		&pdwReturnedProductType
	);
	printf("Product Type (pro): %d\n", pdwReturnedProductType);


	/* GetComputerObjectNameW() function - NOT WRKING */
	//wchar_t cn[MAX_COMPUTERNAME_LENGTH + 1]; // Buffer to store the computer name
	//PULONG s = (PULONG)MAX_COMPUTERNAME_LENGTH + 1;   // Size of the buffer
	//::GetComputerObjectNameW(1,cn,s);
	//printf("Windows directory: %ws\n", path);

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
