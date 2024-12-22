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

typedef NTSTATUS(WINAPI* fZwQuerySystemInformation)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS(NTAPI* fRtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

struct Signature_t
{
	const char* sig;
	const char* mask;
};

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

		/*HMODULE ntDll = GetModuleHandleA("ntdll.dll");

		fZwQuerySystemInformation ZwQuerySystemInformation = (fZwQuerySystemInformation)GetProcAddress(ntDll, "ZwQuerySystemInformation");
		if (ZwQuerySystemInformation == nullptr)
			return false;

		fRtlAdjustPrivilege RtlAdjustPrivilege = (fRtlAdjustPrivilege)GetProcAddress(ntDll, "RtlAdjustPrivilege");
		if (RtlAdjustPrivilege == nullptr)
			return false;

		BOOLEAN oldPriv;
		RtlAdjustPrivilege(20, TRUE, FALSE, &oldPriv); // Debug Privilege

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
			const SYSTEM_HANDLE& sysHandle = handleInfo->Handles[i];

			if (sysHandle.ProcessId == currentProcessId) {
				if ((int)sysHandle.ObjectTypeNumber == 0x7)
				{
					if (GetProcessId((HANDLE)sysHandle.Handle) == processId)
					{
						targetHandle = (HANDLE)sysHandle.Handle;
						return true;
					}
				}
			}
		}*/

		targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

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

	uintptr_t ResolveRVA(uintptr_t address, uint32_t rvaOffset, uint32_t ripOffset) 
	{
		const uintptr_t rva = *reinterpret_cast<PLONG>(address + rvaOffset);
		const uintptr_t rip = address + ripOffset;

		return rva + rip;
	}

	uintptr_t FindSig(Signature_t signature, uintptr_t base, size_t size)
	{
		for (BYTE* region = reinterpret_cast<BYTE*>(base); region < (reinterpret_cast<BYTE*>(base) + size); ++region)
		{
			if (*region == *signature.sig)
			{
				const unsigned char* patternIt = reinterpret_cast<const BYTE*>(signature.sig), * maskIt = reinterpret_cast<const BYTE*>(signature.mask), * memoryIt = region;

				bool found = true;

				for (; *maskIt && (memoryIt < (reinterpret_cast<BYTE*>(base) + size)); ++maskIt, ++patternIt, ++memoryIt)
				{
					if (*maskIt != 'x') continue;
					if (*memoryIt != *patternIt)
					{
						found = false;
						break;
					}
				}

				if (found) 
					return reinterpret_cast<std::uintptr_t>(region);
			}
		}

		return NULL;
	}

	constexpr const DWORD GetPid() noexcept
	{
		return processId;
	}

	constexpr const HANDLE GetHandle() noexcept
	{
		return targetHandle;
	}
};