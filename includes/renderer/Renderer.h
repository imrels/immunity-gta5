#pragma once
#include <fstream>
#include <iostream>
#include <ctime>

#include <dwmapi.h> 
#include <TlHelp32.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")


//#include <D3DX11.h>
//#pragma comment(lib, "d3dx11.lib")

	


#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"


// Color Struct Storing rgba value
struct RGBA {
	RGBA(int r, int g, int b, int a) :
		r(r), g(g), b(b), a(a) { }

	int r ;
	int g ;
	int b ;
	int a ;
};

class imgui_render {
public:
	ImGuiIO io;

	HRESULT hres;
	D3D11_VIEWPORT viewport;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	typedef void (*vResourceLoadCall)(ID3D11Device*);


	vResourceLoadCall loadCall = nullptr;

	void SetResourceLoad(vResourceLoadCall funct2);

	void Initialize(HWND targetWindow, IDXGISwapChain* pSwapchain);
	
	void BeginScene();
	void EndScene();

	void Render();

	bool ready();
	void release();
	void reset(UINT Width, UINT Height);


	float RenderText(const std::string& text, const ImVec2& position, float size, RGBA color, bool center,bool outine = false);
	void RenderLine(const ImVec2& from, const ImVec2& to,  RGBA color, float thickness = 1.0f);
	void RenderCircle(const ImVec2& position, float radius, RGBA color, float thickness = 1.0f, uint32_t segments = 16);
	void RenderCircleFilled(const ImVec2& position, float radius, RGBA color, uint32_t segments = 16);
	void RenderRect(const ImVec2& from, const ImVec2& to, RGBA color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	void RenderDot(const ImVec2& from, const ImVec2& to, RGBA color, float thickness = 1.0f);
	void RenderRectFilled(const ImVec2& from, const ImVec2& to,RGBA color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);



	ImVec2 RenderMenuRow(const ImVec2& from,float width);
	void RenderMenuSwitch(const ImVec2& from,std::string value);
	void RenderMenuValue(const ImVec2& from,float value);

private:

	bool finishedInit = false;
	ImFont* imFont;
};