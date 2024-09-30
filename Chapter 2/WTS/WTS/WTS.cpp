// WTS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "wtsapi32")
#include<Windows.h>
#include<wtsapi32.h>
#include <iostream>
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

bool EnumerateProcesses1() {
    PWTS_PROCESS_INFO info;
    DWORD count;
    if (!::WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &info, &count\
    ))
        return false;
    for (DWORD i = 0; i < count; i++) {
        auto pi = info + i;
        printf("\nPID: %5d (S: %d) (User: %ws) %ws",
            pi->ProcessId, pi->SessionId,
            (PCWSTR)GetUserNameFromSid(pi->pUserSid), pi->pProcessName);
    }
    ::WTSFreeMemory(info);

    return true;
}

CString GetCpuTime(PWTS_PROCESS_INFO_EX pi) {
    auto totalTime = pi->KernelTime.QuadPart + pi->UserTime.QuadPart;
    return CTimeSpan(totalTime / 10000000LL).Format(L"%D:%H:%M:%S");
}

bool EnumerateProcesses2() {
    PWTS_PROCESS_INFO_EX info;
    DWORD count;
    DWORD level = 1; // extended info
    if (!::WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &level,
        WTS_ANY_SESSION, (PWSTR*)&info, &count))
        return false;
    for (DWORD i = 0; i < count; i++) {
        auto pi = info + i;
        printf("\nPID: %5d (S: %d) (T: %3d) (H: %4d) (CPU: %ws) (U: %ws) %ws",
            pi->ProcessId, pi->SessionId, pi->NumberOfThreads, pi->HandleCount,
            (PCWSTR)GetCpuTime(pi),
            (PCWSTR)GetUserNameFromSid(pi->pUserSid), pi->pProcessName);
    }
    ::WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, info, count);
    return true;
}
int main()
{
    //EnumerateProcesses1();
    EnumerateProcesses2();


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
