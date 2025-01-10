#pragma once
#include <cstddef>

namespace offsets
{
	inline std::ptrdiff_t m_hPawn;
	inline std::ptrdiff_t m_iHealth;
	inline std::ptrdiff_t m_bPawnIsAlive;
	inline std::ptrdiff_t m_entitySpottedState;
	inline std::ptrdiff_t m_vOldOrigin;
	inline std::ptrdiff_t m_iTeamNum;

	inline std::ptrdiff_t dwGlobalVars;
	inline std::ptrdiff_t dwGameEntitySystem;
	inline std::ptrdiff_t dwViewMatrix;
	inline std::ptrdiff_t dwLocalPlayerController;
	inline std::ptrdiff_t schemaSystemInterface;
}