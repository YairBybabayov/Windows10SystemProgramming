// Termination.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <stdio.h>

int main()
{
    WCHAR name[] = L"powershell";
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Create the process
    BOOL success = ::CreateProcess(nullptr, name, nullptr, nullptr, FALSE,
        CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);
    if (!success) {
        printf("Error creating process: %d\n", ::GetLastError());
    }
    else {
        printf("Process created. PID=%d\n", pi.dwProcessId);
    }
    WaitForInputIdle(pi.hProcess, 5000);
    if (IsDebuggerPresent()) {
        Sleep(10000);  // Give debugger time to stabilize
    }

    // Open the process with PROCESS_TERMINATE access
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, pi.dwProcessId);

    if (!hProcess) {
        printf("Error opening process: %d\n", ::GetLastError());
        return EXIT_FAILURE;
    }

    // Check if the process is still active
    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        // Terminate the process
        if (TerminateProcess(hProcess, exitCode)) {
            printf("Process terminated.\n");
        }
        else {
            printf("Failed to terminate process: %d\n", ::GetLastError());
        }
    }
    else {
        printf("Process is not active or already exited.\n");
    }

    // Close handles
    ::CloseHandle(hProcess);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
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
