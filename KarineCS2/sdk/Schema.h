#pragma once
#include <string>

#include "../Context.h"
#include "classes/ISchemaSystem.h"

namespace schemaSystem
{
	void Initialize();
	uintptr_t GetOffset(const uint64_t uHashedFieldName);
}