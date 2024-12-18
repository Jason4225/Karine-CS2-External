#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

typedef struct _SYSTEM_HANDLE {
	ULONG ProcessId;
	UCHAR ObjectTypeNumber;
	UCHAR Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef NTSTATUS(WINAPI* ZwQuerySystemInformation_t)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

class CMemory
{
private:
	HANDLE targetHandle = NULL;
	DWORD processId = NULL;

	DWORD FindProcessId(const char* procName) noexcept
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnap)
			return NULL;

		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		while (Process32Next(hSnap, &procEntry))
		{
			if (!strcmp(procEntry.szExeFile, procName))
			{
				CloseHandle(hSnap);
				return procEntry.th32ProcessID;
			}
		}

		CloseHandle(hSnap);
		return NULL;
	}
public:
	bool Initialize(const char* procName) noexcept
	{
		processId = FindProcessId(procName);
		if (processId == NULL)
			return false;

		ZwQuerySystemInformation_t ZwQuerySystemInformation = (ZwQuerySystemInformation_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwQuerySystemInformation");
		if (ZwQuerySystemInformation == nullptr)
			return false;

		std::vector<BYTE> buffer(0x10000);
		
		ULONG returnLength = 0;
		NTSTATUS status = ZwQuerySystemInformation(16, buffer.data(), buffer.size(), &returnLength);

		if (status == STATUS_INFO_LENGTH_MISMATCH)
		{
			buffer.resize(returnLength);
			status = ZwQuerySystemInformation(16, buffer.data(), buffer.size(), &returnLength);
		}

		PSYSTEM_HANDLE_INFORMATION handleInfo = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(buffer.data());
		DWORD currentProcessId = GetCurrentProcessId();

		for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
			const SYSTEM_HANDLE& handle = handleInfo->Handles[i];

			if (handle.ProcessId == currentProcessId) {
				if ((int)handle.ObjectTypeNumber == 0x7)
				{
					if (GetProcessId((HANDLE)handle.Handle) == processId)
					{
						targetHandle = (HANDLE)handle.Handle;
						return true;
					}
				}
			}
		}

		return false;
	}

	uintptr_t FindModule(const char* modName) noexcept
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
		if (!hSnap)
			return NULL;

		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		while (Module32Next(hSnap, &modEntry))
		{
			if (!strcmp(modEntry.szModule, modName))
			{
				CloseHandle(hSnap);
				return reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
			}
		}

		CloseHandle(hSnap);
		return NULL;
	}

	template<typename T = void*>
	T Read(uintptr_t address)
	{
		T buffer{ };
		ReadProcessMemory(targetHandle, (void*)address, &buffer, sizeof(T), 0);
		return buffer;
	}

	bool ReadRaw(uintptr_t address, void* buffer, size_t size)
	{
		return ReadProcessMemory(targetHandle, (void*)address, buffer, size, 0);
	}

	constexpr const DWORD GetPid() noexcept
	{
		return processId;
	}
};