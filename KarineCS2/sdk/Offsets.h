#pragma once
#include <cstddef>

namespace offsets
{
	// Gonna leave it like that not gonna do Schema
	inline constexpr std::ptrdiff_t m_hPawn = 0x62C;
	inline constexpr std::ptrdiff_t m_iHealth = 0x344;
	inline constexpr std::ptrdiff_t m_bPawnIsAlive = 0x814;
	inline constexpr std::ptrdiff_t m_entitySpottedState = 0x23D0;
	inline constexpr std::ptrdiff_t m_vOldOrigin = 0x1324;
	inline constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
	inline std::ptrdiff_t dwGlobalVars;
	inline std::ptrdiff_t dwGameEntitySystem;
	inline std::ptrdiff_t dwViewMatrix;
}