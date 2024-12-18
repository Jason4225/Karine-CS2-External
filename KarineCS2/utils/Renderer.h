#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <imgui/imgui.h>

class CRenderer
{
private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;

	bool CreateDevice();
	void CleanupDevice();
	void CreateRenderTarget();
	void CleanupRenderTarget();
public:
	HWND targetWindow = nullptr;

	bool Initialize();
	void Destroy();

	void Render();
	void ChangeClickability(bool canclick);
};