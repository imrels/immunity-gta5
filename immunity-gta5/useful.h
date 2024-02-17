#pragma once
#include "imports.h"

using namespace std;



#include <shlobj.h>
string GetFolderLocation(int csidl) {


	char buffer[_MAX_PATH];
	LPITEMIDLIST pidl = 0;
	HRESULT result = SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	*buffer = 0;

	if (result == 0) {
		SHGetPathFromIDList(pidl, buffer);
		CoTaskMemFree(pidl);
		return string(buffer);
	}
	return string(buffer);
}



namespace logs {
	string logFile = "";
	inline bool exists() {
		string appdata = GetFolderLocation(CSIDL_LOCAL_APPDATA);
		logFile = appdata + "/immunity/logs/log.txt";
		cout << "[!] appdata " << appdata << endl;

		if (CreateDirectory((appdata + "/immunity/logs/").c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError()) {

			return true;
		}
		return false;
	}

	inline string getCurrentDateTime(string s) {
		time_t now = time(0);
		struct tm  tstruct;
		char  buf[80];
		tstruct = *localtime(&now);
		if (s == "now")
			strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		else if (s == "date")
			strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		return string(buf);
	};



	inline void clear() {
		if (exists()) {
			std::ofstream ofs;
			ofs.open(logFile.c_str(), std::ofstream::out | std::ofstream::trunc);
			ofs.close();
		}
	}

	inline void addLog(string logMsg) {
		if (exists()) {
			string now = getCurrentDateTime("now");
			ofstream ofs(logFile.c_str(), std::ios_base::out | std::ios_base::app);
			ofs << now << '\t' << logMsg << '\n';
			ofs.close();
		}
	}
	inline void add(string logMsg) {
		addLog(logMsg);
	}
}



namespace utils {

	using _RtlCreateUserThread = NTSTATUS(NTAPI*)(
		HANDLE ProcessHandle,
		PSECURITY_DESCRIPTOR SecurityDescriptor,
		BOOLEAN CreateSuspend,
		ULONG StackZeroBits,
		PULONG StackReserved,
		PULONG StackCommit,
		void* StartAddress,
		void* StartParameter,
		PHANDLE ThreadHandle,
		void* ClientID
	);




	HANDLE spoof_thread(void* Thread, HMODULE& hModule) {
		HMODULE NT_DLL = LoadLibrary(xorstr_("ntdll"));
		uintptr_t SpoofedAddress = NULL;
		int DefaultThreadSize = 1000;
		srand(time(NULL)); // see random nums

		for (int i = 1; i < 4; i++) {
			SpoofedAddress |= (rand() & 0xFF) << i * 8; // we store it in the lowest bytes
			SpoofedAddress |= (rand() & 0xFF) << i * 8;
			SpoofedAddress |= (rand() & 0xFF) << i * 8;
			//returns spoofed address
		}
		while (SpoofedAddress > 0x7FFFFFFF) {
			SpoofedAddress -= 0x1000;
		}
		VirtualProtect((void*)SpoofedAddress, DefaultThreadSize, PAGE_EXECUTE_READWRITE, NULL);

		CONTEXT tContext;
		HANDLE  pHandle = nullptr;

		_RtlCreateUserThread KeThread = (_RtlCreateUserThread)(GetProcAddress(GetModuleHandle(xorstr_("ntdll")), xorstr_("RtlCreateUserThread")));
		KeThread(GetCurrentProcess(), nullptr, TRUE, NULL, NULL, NULL, (PTHREAD_START_ROUTINE)SpoofedAddress, hModule, &pHandle, NULL); //create a thread & stop init everything

		tContext.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
		GetThreadContext(pHandle, &tContext);

#ifndef _WIN64
		tContext.Eax = (ULONG32)Thread;
#else
		tContext.Rcx = (ULONG64)Thread;
#endif

		tContext.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;

		SetThreadContext(pHandle, &tContext);

		ResumeThread(pHandle);


		return pHandle;
	}
}






#define PI 3.14159265

float EaseOut(float time, float start, float end, float current) {
	if (time == 0) return start;  if ((time /= current) == 1) return start + end;
	float p = current * .3f;
	float a = end;
	float s = p / 4;
	return (a * pow(2, -10 * time) * sin((time * current - s) * (2 * PI) / p) + end + start);
}

double easeOutCubic(double t) {
	return 1 + (--t) * t * t;
}

double easeInSine(double t) {
	return sin(1.5707963 * t);
}




