#pragma once
#include <cstdint>
#include <assert.h>

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15
#define ENT_MAX_NETWORKED_ENTRY 16384

class CBaseHandle
{
private:
	std::uint32_t index;
public:
	CBaseHandle() noexcept : index(INVALID_EHANDLE_INDEX) {}

	CBaseHandle(const int nEntry, const int nSerial) noexcept
	{
		assert(nEntry >= 0 && (nEntry & ENT_ENTRY_MASK) == nEntry);
		assert(nSerial >= 0 && nSerial < (1 << NUM_SERIAL_NUM_SHIFT_BITS));

		index = nEntry | (nSerial << NUM_SERIAL_NUM_SHIFT_BITS);
	}

	constexpr const bool operator!=(const CBaseHandle& other) const noexcept
	{
		return index != other.index;
	}

	constexpr const bool operator==(const CBaseHandle& other) const noexcept
	{
		return index == other.index;
	}

	constexpr const bool operator<(const CBaseHandle& other) const noexcept
	{
		return index < other.index;
	}

	constexpr const bool IsValid() const noexcept
	{
		return index != INVALID_EHANDLE_INDEX;
	}

	constexpr const int GetEntryIndex() const noexcept
	{
		return static_cast<int>(index & ENT_ENTRY_MASK);
	}

	constexpr const int GetSerialNumber() const noexcept
	{
		return static_cast<int>(index >> NUM_SERIAL_NUM_SHIFT_BITS);
	}
};