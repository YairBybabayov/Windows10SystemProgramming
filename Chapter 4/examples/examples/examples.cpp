// examples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include<stdio.h>

DWORD WINAPI DoWork(PVOID) {
	printf("Thread ID running DoWork: %u\n", ::GetCurrentThreadId());
	// simulate some heavy work...
	::Sleep(3000);
	// return a result
	return 42;
}
int main() {
	HANDLE hThread = ::CreateThread(nullptr, 0, DoWork, nullptr, 0, nullptr);
	if (!hThread) {
		printf("Failed to create thread (error=%d)\n", ::GetLastError());
		return 1;
	}
	// print ID of main thread
	printf("Main thread ID: %u\n", ::GetCurrentThreadId());
	// wait for the thread to finish
	::WaitForSingleObject(hThread, INFINITE);
	DWORD result;
	::GetExitCodeThread(hThread, &result);
	printf("Thread done. Result: %u\n", result);
	::CloseHandle(hThread);
	return 0;
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
