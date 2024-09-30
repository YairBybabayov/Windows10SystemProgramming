// MiniProcExp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "wtsapi32")
#include<Windows.h>
#include<wtsapi32.h>
#include <stdio.h>
#include <atlstr.h>
#include <atltime.h>

CString GetUserNameFromSid(PSID sid) {
    if (sid == nullptr)
        return L"";
    WCHAR name[128], domain[64];
    DWORD len = _countof(name);
    DWORD domainLen = _countof(domain);
    SID_NAME_USE use;
    if (!::LookupAccountSid(nullptr, sid, name, &len, domain, &domainLen, &use))
        return L"";
    return CString(domain) + L"\\" + name;
}

CString GetCpuTime(PWTS_PROCESS_INFO_EX pi) {
    auto totalTime = pi->KernelTime.QuadPart + pi->UserTime.QuadPart;
    return CTimeSpan(totalTime / 10000000LL).Format(L"%D:%H:%M:%S");
}

bool EnumerateProcessesEX() {
    PWTS_PROCESS_INFO_EX info;
    DWORD count;
    DWORD level = 1; // extended info
    if (!::WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &level,
        WTS_ANY_SESSION, (PWSTR*)&info, &count))
        return false;
    for (DWORD i = 0; i < count; i++) {
        auto pi = info + i;
        printf("\nPID: %5d (S: %d) (Number Of Threads: %3d) (Handle Count: %4d) (CPU: %ws) (User: %ws) %ws",
            pi->ProcessId, pi->SessionId, pi->NumberOfThreads, pi->HandleCount,
            (PCWSTR)GetCpuTime(pi),
            (PCWSTR)GetUserNameFromSid(pi->pUserSid), pi->pProcessName);
    }
    ::WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, info, count);
    return true;
}

BOOL FindProcessesEX(LPCWSTR pProcessName) {
    PWTS_PROCESS_INFO_EX info;
    DWORD count;
    DWORD level = 1; // extended info
    BOOL found = false;
    if (!::WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &level,
        WTS_ANY_SESSION, (PWSTR*)&info, &count))
        return false;
    for (DWORD i = 0; i < count; i++) {
        auto pi = info + i;
        if (wcscmp(pProcessName, pi->pProcessName) == 0) {
            found = true;
            printf("\nPID: %5d (S: %d) (Number Of Threads: %3d) (Handle Count: %4d) (CPU: %ws) (User: %ws) %ws",
                pi->ProcessId, pi->SessionId, pi->NumberOfThreads, pi->HandleCount,
                (PCWSTR)GetCpuTime(pi),
                (PCWSTR)GetUserNameFromSid(pi->pUserSid), pi->pProcessName);
        }
    }
    if (found == false) {
        printf("Process %ws was not found\n", pProcessName);
        return false;
    }
    ::WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, info, count);
    return true;
}
VOID CreatNewProcess() {
    WCHAR name[] = L"notepad";
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL success = ::CreateProcess(nullptr, name, nullptr, nullptr, FALSE,
        0, nullptr, nullptr, &si, &pi);
    if (!success) {
        printf("Error creating process: %d\n", ::GetLastError());
    }
    else {
        printf("Process created. PID=%d\n", pi.dwProcessId);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
}
VOID ProcessTerminator(DWORD PID) {
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, PID);
    DWORD exitCode;
    GetExitCodeProcess(hProcess, &exitCode);
    TerminateProcess(
        hProcess,
        exitCode);
}

int main()
{
    char choice = 'x';
    printf("Welcome to MiniProcExp\n");
    printf("Press a to watch the current running processes\n");
    printf("Press b to search a specific process\n");
    printf("Press c to create a new process\n");
    printf("Press d to terminate an existing process\n");
    printf("Press z to exit\n");


    printf("Your choice: ");
    fflush(stdout);
    scanf_s("%c", &choice, sizeof(choice));

    printf("You chose: %c\n", choice);

    //LPWSTR lpwPN = nullptr;
    wchar_t lpwPN[32];
    //LPWSTR otherVariable = L"notepad.exe";
    DWORD PID = NULL;
    while (choice != 'z') {
        switch (choice)
        {
        case 'a':
            EnumerateProcessesEX();
            break;
        case 'b':
        {
            wprintf(L"Enter the process name: ");
            wscanf_s(L"%31ls", lpwPN, 31); // Read wide string input from the user
            size_t len = wcslen(lpwPN);
            if (lpwPN[len - 1] == L'\n') {
                lpwPN[len - 1] = L'\0';  // Replace newline with null terminator
            }
            FindProcessesEX(lpwPN);
        }
        break;
        case 'c':
            CreatNewProcess();
            break;
        case 'd':
            printf("Enter PID:\n");
            scanf_s("%d", &PID, sizeof(DWORD)); // Read wide string input from the user
            ProcessTerminator(PID);
            break;
        default:
            printf("Unidentified Choice\n");
            break;
        }
        
        printf("\n\nPress a to watch the current running processes\n");
        printf("Press b to search a specific process\n");
        printf("Press c to create a new process\n");
        printf("Press d to terminate an existing process\n");
        printf("Press z to exit\n");

        printf("Your choice: ");
        fflush(stdout);
        // Add a space before %c to discard any trailing newline from previous input!!!
        scanf_s(" %c", &choice, sizeof(choice));

        printf("You chose: %c\n", choice);
        
    }

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
