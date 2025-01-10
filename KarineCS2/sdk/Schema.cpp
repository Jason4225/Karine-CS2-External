#include "Schema.h"
#include <memory>
#include <FNV1A.h>
#include <SDK.h>

// @credits: https://www.unknowncheats.me/forum/counter-strike-2-a/598055-netvars-parser-external.html
// @credits: https://github.com/maecry/asphyxia-cs2

struct SchemaData_t
{
	uint64_t m_uHashedFieldName = 0x0ULL;
	std::uintptr_t m_uOffset = 0x0U;
};

static std::vector<SchemaData_t> schemaData;

void schemaSystem::Initialize()
{
	uintptr_t schemaSystemInterface = sdk::schemaSystemDLL + offsets::schemaSystemInterface;

	// @ ida: CSchemaSystem -> 13th Index
	/*
	.text:000000018000E62D                 mov     rax, [rbp+190h] <-- Offset to array
	.text:000000018000E634                 mov     rbx, [rsp+38h+arg_8]
	.text:000000018000E639                 movzx   ecx, di
	.text:000000018000E63C                 mov     rax, [rax+rcx*8]
	.text:000000018000E640                 add     rsp, 20h
	.text:000000018000E644                 pop     rdi
	.text:000000018000E645                 pop     rsi
	.text:000000018000E646                 pop     rbp
	.text:000000018000E647                 retn
	*/
	uintptr_t listElement = ctx::memory.Read<std::uintptr_t>(schemaSystemInterface + 0x190);

	CSchemaSystemTypeScope* typeScope = ctx::memory.Read<CSchemaSystemTypeScope*>(listElement + (0x10 * 8));
	CUtlTSHash<CSchemaClassBindingBase*, 256, unsigned int> pTable = typeScope->GetBindingsTable();

	std::unique_ptr<UtlTSHashHandle_t[]> elements = std::make_unique_for_overwrite< UtlTSHashHandle_t[] >(pTable.Count());
	const int elementCount = pTable.GetElements(0, pTable.Count(), elements.get());

	for (int i = 0; i < elementCount; i++)
	{
		const UtlTSHashHandle_t element = elements[i];
		if (!element)
			continue;

		CSchemaClassBindingBase* const pClassBinding = pTable.Element(element);
		if (!pClassBinding)
			continue;

		if (!pClassBinding->GetNumFields())
			continue;

		CSchemaField* pSchemaField = &pClassBinding->GetFields()[0];
		for (int nFieldIndex = 0; nFieldIndex < pClassBinding->GetNumFields(); nFieldIndex++)
		{
			if (!pSchemaField)
				continue;

			std::string name = pSchemaField->GetName();

			std::string szFieldClassBuffer = pClassBinding->GetName() + "->" + pSchemaField->GetName();

			schemaData.emplace_back(fnv1a::Hash(szFieldClassBuffer.c_str()), pSchemaField->GetOffset());

			pSchemaField = reinterpret_cast<CSchemaField*>(std::uintptr_t(pSchemaField) + 0x20);
		}
	}
}

std::uintptr_t schemaSystem::GetOffset(const uint64_t uHashedFieldName)
{
	for (SchemaData_t data : schemaData)
	{
		if (data.m_uHashedFieldName != uHashedFieldName)
			continue;

		return data.m_uOffset;
	}

	return 0U;
}