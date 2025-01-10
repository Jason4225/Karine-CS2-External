#include "Renderer.h"

#include <d3d11.h>
#include <tchar.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include <dwmapi.h>
#include <string>

#include "Utils.h"
#include "Gui.h"

#include <SDK.h>

#include <types/Vector.h>
#include <types/ViewMatrix.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// TODO: No support for fullscren, maybe later z-order? -> https://blog.adeltax.com/window-z-order-in-windows-10/

bool CRenderer::Initialize()
{
    std::string name = utils::RandomString(15);
    WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, name.c_str(), nullptr };
    RegisterClassExA(&wc);

    targetWindow = ::CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, name.c_str(), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(targetWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(targetWindow, &margin);

    if (!CreateDevice())
    {
        CleanupDevice();
        return false;
    }

    CreateRenderTarget();

    ShowWindow(targetWindow, SW_SHOWDEFAULT);
    UpdateWindow(targetWindow);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    ImGui_ImplWin32_Init(targetWindow);
    ImGui_ImplDX11_Init(device, deviceContext);

    ImGui::GetIO().Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    ImGui::GetIO().IniFilename = nullptr;
    ChangeClickability(false);
    return true;
}

void CRenderer::Destroy()
{
    swapChain->Present(0, 0);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDevice();
}

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

Vector3_t w2s(ViewMatrix_t vm, Vector3_t& in) {

    Vector3_t out;

    out.x = vm[0][0] * in.x + vm[0][1] * in.y + vm[0][2] * in.z + vm[0][3];
    out.y = vm[1][0] * in.x + vm[1][1] * in.y + vm[1][2] * in.z + vm[1][3];

    float width = vm[3][0] * in.x + vm[3][1] * in.y + vm[3][2] * in.z + vm[3][3];

    if (width < 0.01f) {
        return out;
    }

    float inverseWidth = 1.f / width;

    out.x *= inverseWidth;
    out.y *= inverseWidth;

    float x = screenWidth / 2;
    float y = screenHeight / 2;

    x += 0.5f * out.x * screenWidth + 0.5f;
    y -= 0.5f * out.y * screenHeight + 0.5f;

    out.x = x;
    out.y = y;

    return out;
}

void CRenderer::Render()
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (gui::open)
    {
        ChangeClickability(true);
        gui::Render();
    }
    else
    {
        ChangeClickability(false);
    }

    ImGui::GetBackgroundDrawList()->AddText({ 10, 10 }, ImColor(255, 0, 0), std::string("Shitware 1337 | FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());

    ImDrawList* pBackgroundDrawList = ImGui::GetBackgroundDrawList();
    
    // P100 esp
    if (gui::esp)
    {
        for (CEntity entity : sdk::playerList)
        {
            if (entity.m_iTeamNum == sdk::localEntity.m_iTeamNum) // Team check
                continue;

            Vector3_t www = w2s(sdk::viewMatrix, entity.m_vOldOrigin);
            pBackgroundDrawList->AddText({ www.x, www.y }, ImColor(255, 0, 0), "Player");
        }
    }

    ImGui::Render();
    const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    deviceContext->ClearRenderTargetView(renderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    swapChain->Present(0, 0);
}

bool CRenderer::CreateDevice()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = targetWindow;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &deviceContext);
    
    if (result == DXGI_ERROR_UNSUPPORTED)
        result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &deviceContext);
    
    if (result != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CRenderer::CleanupDevice()
{
    CleanupRenderTarget();

    if (swapChain) 
    { 
        swapChain->Release(); 
        swapChain = nullptr; 
    }

    if (deviceContext) 
    { 
        deviceContext->Release(); 
        deviceContext = nullptr; 
    }

    if (device) 
    { 
        device->Release(); 
        device = nullptr; 
    }
}

void CRenderer::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();
}

void CRenderer::CleanupRenderTarget()
{
    if (renderTargetView) 
    { 
        renderTargetView->Release();
        renderTargetView = nullptr; 
    }
}

void CRenderer::ChangeClickability(bool canclick)
{
    if (canclick)
        SetWindowLongPtrW(targetWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
    else
        SetWindowLongPtrW(targetWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
        return true;

    return ::DefWindowProcA(hWnd, Msg, wParam, lParam);
}