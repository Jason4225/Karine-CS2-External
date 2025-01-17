#pragma once
#include <SDK.h>

namespace visuals
{
	namespace esp
	{
		void AddName(CEntity& entity);
		void AddBox(CEntity& entity);
		void AddHealth(CEntity& entity);
		void AddHeadDot(CEntity& entity);

		void Render();
	}

	/*
	* Render all of our Visuals
	*/
	inline void RenderVisuals()
	{
		esp::Render();
	}
}

namespace misc
{
	namespace cool
	{
		
	}
}