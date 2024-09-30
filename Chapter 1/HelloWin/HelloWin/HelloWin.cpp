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
