#pragma once
#include "../types/CUtlHash.h"

class CSchemaType
{
public:
	std::string GetName()
	{
		uintptr_t uAddress = ctx::memory.Read<uintptr_t>(uintptr_t(this) + 0x8);
		return ctx::memory.ReadString(uAddress);
	}
};

class CSchemaField
{
public:
	std::string GetName()
	{
		uintptr_t uAddress = ctx::memory.Read<uintptr_t>(uintptr_t(this));
		return ctx::memory.ReadString(uAddress);
	}

	CSchemaType* GetSchemaType()
	{
		return ctx::memory.Read< CSchemaType* >(uintptr_t(this) + 0x8);
	}

	uint32_t GetOffset()
	{
		return ctx::memory.Read< uint32_t >(uintptr_t(this) + 0x10);
	}
};

class CSchemaClassBindingBase
{
public:
	std::string GetName()
	{
		uintptr_t uAddress = ctx::memory.Read<uintptr_t>(uintptr_t(this) + 0x8);
		return ctx::memory.ReadString(uAddress);
	}

	std::string GetProjectName()
	{
		return ctx::memory.ReadString(reinterpret_cast<uintptr_t>(this) + 0x10);
	}

	uint32_t GetSize()
	{
		return ctx::memory.Read< uint32_t >(uintptr_t(this) + 0x18);
	}

	uint16_t GetNumFields()
	{
		return ctx::memory.Read< uint16_t >(uintptr_t(this) + 0x1C);
	}

	CSchemaField* GetFields()
	{
		return ctx::memory.Read< CSchemaField* >(uintptr_t(this) + 0x28);
	}
};

class CSchemaSystemTypeScope
{
public:
	CUtlTSHash< CSchemaClassBindingBase*, 256, unsigned int > GetBindingsTable()
	{
		return ctx::memory.Read< CUtlTSHash< CSchemaClassBindingBase*, 256, unsigned int > >(uintptr_t(this) + 0x500);
	};
};

