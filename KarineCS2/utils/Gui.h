#pragma once

namespace gui
{
	inline bool open = false;

	inline int currentTab = 0;
	inline int aimTab = 0;
	inline int miscTab = 0;
	inline int visualsTab = 0;

	void Render();

	void DrawMisc();
	void DrawVisuals();
	void DrawAim();
}