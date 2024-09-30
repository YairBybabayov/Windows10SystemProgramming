// MemLimit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include <assert.h>
#include <iostream>

bool SetJobMemoryLimit(HANDLE hJob, SIZE_T max) {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = {};
    info.ProcessMemoryLimit = max;
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;
    return ::SetInformationJobObject(hJob, JobObjectExtendedLimitInformation,
        &info, sizeof(info));
}
VOID SetProcessMemoryLimit(DWORD PID, SIZE_T max) {
    auto hJob = ::CreateJobObject(nullptr, L"Job");
    assert(hJob);
    auto hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, false, PID);
    auto success = ::AssignProcessToJobObject(hJob, hProcess);
    assert(success);
    SetJobMemoryLimit(hJob, max);

    printf("Press any key to terminate parent job...\n");
    ::getchar();
    ::TerminateJobObject(hJob, 0); //Closes the process
}

int main()
{
    DWORD PID;
    SIZE_T max;
    printf("Enter PID:");
    fflush(stdout);
    scanf_s("%d", &PID);

    printf("Enter memory limit:");
    fflush(stdout);
    scanf_s("%d", &max);

    SetProcessMemoryLimit(PID, max);

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
