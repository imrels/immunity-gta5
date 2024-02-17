#pragma once
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>


#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
#include "./Renderer.h"
//#include "../font.h"

#define SAFE_RELEASE(pObject) { if(pObject) { (pObject)->Release(); (pObject) = NULL; } }



bool imgui_render::ready() {
	return pDevice && pContext;
}


void imgui_render::SetResourceLoad(vResourceLoadCall funct2) {
	
	loadCall = funct2;
}




void imgui_render::Initialize(HWND targetWindow, IDXGISwapChain* pSwapchain) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	pSwapChain = pSwapchain;


	io.IniFilename = NULL;
	ImGui::StyleColorsDark();



	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(8, 8);
	//style->WindowPadding = ImVec2(15, 15);
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->WindowRounding = 1.0f;
	style->FramePadding = ImVec2(4, 4);
	style->FrameRounding = 1.0f;
	style->ItemSpacing = ImVec2(2, 2);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->PopupRounding = 1.0f;



	ImColor mainColor = ImColor(4, 188, 0, 200);

	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);

	style->Colors[ImGuiCol_TitleBg] = mainColor;
	style->Colors[ImGuiCol_TitleBgActive] = mainColor;
	style->Colors[ImGuiCol_TitleBgCollapsed] = mainColor;


	style->Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 220);
	//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBg] = ImColor(21, 21, 21, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(59, 59, 59, 200);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(59, 59, 59, 200);

	style->Colors[ImGuiCol_Button] = ImColor(51, 51, 51, 150);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(64, 62, 64, 180);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(3, 159, 0, 200);

	style->Colors[ImGuiCol_CheckMark] = ImColor(4, 158, 0, 155);




	style->Colors[ImGuiCol_SliderGrab] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(4, 168, 0, 155);


	style->Colors[ImGuiCol_Tab] = ImColor(51, 51, 51, 255);
	style->Colors[ImGuiCol_TabHovered] = ImColor(4, 168, 0, 155);
	style->Colors[ImGuiCol_TabActive] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_TabUnfocused] = ImColor(51, 51, 51, 255);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImColor(4, 158, 0, 155);





	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);

	style->Colors[ImGuiCol_ChildWindowBg] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);

	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(25, 25, 25, 220);

	style->Colors[ImGuiCol_Header] = ImColor(51, 51, 51, 220);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(51, 100, 51, 220);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(51, 61, 51, 220);




	style->Colors[ImGuiCol_ScrollbarBg] = ImColor(30, 30, 30, 220);

	style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(30, 110, 30, 220);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(30, 110, 30, 220);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(30, 110, 30, 220);



	style->Colors[ImGuiCol_PopupBg] = ImColor(25, 25, 25, 220);


	style->Colors[ImGuiCol_Border] = ImColor(255, 255, 255, 0);
	style->Colors[ImGuiCol_BorderShadow] = ImColor(255, 255, 255, 0);

	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);


	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);




/*
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);



	*/


	if (SUCCEEDED(pSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
		pSwapchain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);
	}


	loadCall(pDevice);

	ImGui_ImplWin32_Init(targetWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);

	//ImGui_ImplDX11_CreateDeviceObjects();
	//imFont = io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 14.f);
	//imFont = io.Fonts->AddFontDefault();
	// Arial C:\Windows\Fonts\arial.ttf
	imFont = io.Fonts->AddFontDefault();

	return;
}
void imgui_render::release() {



	pContext->OMSetRenderTargets(0, 0, 0);
	RenderTargetView->Release();


	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	pDevice = nullptr;
	pContext = nullptr;
	RenderTargetView = nullptr;
	pSwapChain = nullptr;


	finishedInit = false;

}
void imgui_render::reset(UINT Width, UINT Height) {
	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBuffer);
	pDevice->CreateRenderTargetView(pBuffer, NULL,
		&RenderTargetView);
	// Perform error handling here!
	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

}

void imgui_render::BeginScene() {
	ImGuiIO& io = ImGui::GetIO();






	if (RenderTargetView == nullptr) {
		ID3D11Texture2D* backbuffer = NULL;
		//hres = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);

		hres = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);//pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));

		if (FAILED(hres)) {
			pContext->OMGetRenderTargets(1, &RenderTargetView, NULL);
			return;
		}


		hres = pSwapChain->GetDevice(IID_PPV_ARGS(&pDevice));

		if (FAILED(hres)) {
			pContext->OMGetRenderTargets(1, &RenderTargetView, NULL);
			
			return;
		}


		hres = pDevice->CreateRenderTargetView(backbuffer, NULL, &RenderTargetView);
		backbuffer->Release();
		if (FAILED(hres)) {
			return;
		}
	} 
	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	//else {
	//	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	//}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
}

void imgui_render::Render() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
void imgui_render::EndScene() {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(3);

	ImGui::EndFrame();
}

bool inScreen(ImVec2 pos) {
	if (pos.x < 0 || pos.y < 0 || pos.x > ImGui::GetIO().DisplaySize.x || pos.y > ImGui::GetIO().DisplaySize.y) {
		return false;
	}

	return true;
}

float imgui_render::RenderText(const std::string& text, const ImVec2& position, float size, RGBA color, bool center, bool outine) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();


	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = imFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());

		if (center) {
			if (outine) {
				window->DrawList->AddText(imFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
			}
			window->DrawList->AddText(imFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, IM_COL32(color.r, color.g, color.b, color.a), text.c_str());
		} else {
			if (outine) {
				window->DrawList->AddText(imFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(imFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
			}
			window->DrawList->AddText(imFont, size, { position.x, position.y + textSize.y * i }, IM_COL32(color.r, color.g, color.b, color.a), text.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void imgui_render::RenderDot(const ImVec2& from, const ImVec2& to, RGBA color, float thickness) {

	//DrawRect(x - 1.0f, y - 1.0f, width + 2.0f, height + 2.0f, D3DCOLOR_RGBA(0, 0, 0, 1));
	//DrawRect(x, y, width, height, dColor);

	//imgui_render::RenderRect(ImVec2(from.x - 1, from.y - 1), ImVec2(to.x + 2, to.y + 2), RGBA(0, 0, 0, 255), 1.0f, 0, thickness);
	if (inScreen(from) && inScreen(to))
		imgui_render::RenderRect(from, to, color, 5.0f, 0, thickness);
}

void imgui_render::RenderLine(const ImVec2& from, const ImVec2& to, RGBA color, float thickness) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (inScreen(from) && inScreen(to))
		window->DrawList->AddLine(from, to, IM_COL32(color.r, color.g, color.b, color.a), thickness);
}

void imgui_render::RenderCircle(const ImVec2& position, float radius, RGBA color, float thickness, uint32_t segments) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();


	if (inScreen(position))
		window->DrawList->AddCircle(position, radius, IM_COL32(color.r, color.g, color.b, color.a), segments, thickness);
}

void imgui_render::RenderCircleFilled(const ImVec2& position, float radius, RGBA color, uint32_t segments) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (inScreen(position))
		window->DrawList->AddCircleFilled(position, radius, IM_COL32(color.r, color.g, color.b, color.a), segments);
}

void imgui_render::RenderRect(const ImVec2& from, const ImVec2& to, RGBA color, float rounding, uint32_t roundingCornersFlags, float thickness) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (inScreen(from) && inScreen(to))
		window->DrawList->AddRect(from, to, IM_COL32(color.r, color.g, color.b, color.a), rounding, roundingCornersFlags, thickness);
}

void imgui_render::RenderRectFilled(const ImVec2& from, const ImVec2& to, RGBA color, float rounding, uint32_t roundingCornersFlags) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (inScreen(from) && inScreen(to))
		window->DrawList->AddRectFilled(from, to, IM_COL32(color.r, color.g, color.b, color.a), rounding, roundingCornersFlags);
}


