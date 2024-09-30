// HelloWil.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include <iostream>

int main(int argc, const char* argv[], char* env[]) {
	for (int i = 0; ; i++) {
		if (env[i] == nullptr)
			break;
		auto equals = strchr(env[i], '=');
		// change the equals to NULL
		*equals = '\0';
		printf("%s: %s\n", env[i], equals + 1);
		// for consistency, revert the equals sign
		*equals = '=';
	}

	WCHAR path[MAX_PATH];
	::ExpandEnvironmentStrings(L"%windir%\\explorer.exe", path, MAX_PATH);
	printf("%ws\n", path); // c:\windows\explorer.exe
	printf("%ws\n", L"%windir%\\explorer.exe"); // %windir%\\explorer.exe

	::GetFullPathName(L"c:mydata.txt", MAX_PATH, path, nullptr);
	printf("Process PATH. =%s\n", path);

	MessageBoxW(
		NULL,
		L"Notepad",
		L"test",
		MB_OKCANCEL | MB_ICONEXCLAMATION
	);

	WCHAR name[] = L"notepad";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL success = ::CreateProcess(nullptr, name, nullptr, nullptr, FALSE,
		0, nullptr, nullptr, &si, &pi);
	if (!success) {
		printf("Error creating process: %d\n", ::GetLastError());
	}
	else {
		// process creation succeeded
		printf("Process created. PID=%d\n", pi.dwProcessId);
		DWORD rv = ::WaitForSingleObject(pi.hProcess, 10000); //10 seconds - FOR SOME REASON SEEMS TO JUST TERMINATE
			if (rv == WAIT_TIMEOUT)
				printf("Notepad still running...\n");
			else if (rv == WAIT_OBJECT_0)
				printf("Notepad terminated.\n");
			else // WAIT_ERROR (unlikely in this case)
				printf("Error! %d\n", ::GetLastError());
		
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);

	}



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
