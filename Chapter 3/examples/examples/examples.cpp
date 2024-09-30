// examples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <memory>

std::vector<DWORD> GetJobProcessList(HANDLE hJob) {
	auto size = 256;
	std::vector<DWORD> pids;
	while (true) {
		auto buffer = std::make_unique<BYTE[]>(size);
		auto ok = ::QueryInformationJobObject(hJob, JobObjectBasicProcessIdList
			,
			buffer.get(), size, nullptr);
		if (!ok && ::GetLastError() == ERROR_MORE_DATA) {
			// buffer too small - resize and try again
			size *= 2;
			continue;
		}
		if (!ok)
			break;
		auto info = reinterpret_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST*>(buffer.get());
		pids.reserve(info->NumberOfAssignedProcesses);
		for (DWORD i = 0; i < info->NumberOfAssignedProcesses; i++)
			pids.push_back((DWORD)info->ProcessIdList[i]);
		break;
	}
	return pids;
}

HANDLE CreateSimpleProcess(PCWSTR name) {
	std::wstring sname(name);
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };
	if (!::CreateProcess(nullptr, const_cast<PWSTR>(sname.data()), nullptr, nullptr,
		FALSE, CREATE_BREAKAWAY_FROM_JOB | CREATE_NEW_CONSOLE,
		nullptr, nullptr, &si, &pi))
		return nullptr;
	::CloseHandle(pi.hThread);
	return pi.hProcess;
}
HANDLE CreateJobHierarchy() {
	auto hJob1 = ::CreateJobObject(nullptr, L"Job1");
	assert(hJob1);
	auto hProcess1 = CreateSimpleProcess(L"mspaint");
	auto success = ::AssignProcessToJobObject(hJob1, hProcess1);
	assert(success);
	auto hJob2 = ::CreateJobObject(nullptr, L"Job2");
	assert(hJob2);
	success = ::AssignProcessToJobObject(hJob2, hProcess1);
	assert(success);
	auto hProcess2 = CreateSimpleProcess(L"mstsc");
	success = ::AssignProcessToJobObject(hJob2, hProcess2);
	assert(success);
	auto hProcess3 = CreateSimpleProcess(L"cmd");
	success = ::AssignProcessToJobObject(hJob1, hProcess3);
	assert(success);
	// not bothering to close process and job 2 handles
	return hJob1;
}
int main() {
	auto hJob = CreateJobHierarchy();
	std::vector<DWORD> dwProcessList = GetJobProcessList(hJob);
	
	for (int i = 0; i < dwProcessList.size(); i++) {
		printf("Process at the job: %d\n", dwProcessList.at(i));
	}
	printf("Press any key to terminate parent job...\n");
	::getchar();
	::TerminateJobObject(hJob, 0);
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
