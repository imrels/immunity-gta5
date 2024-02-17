#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>

#include <dwmapi.h> 
#include <TlHelp32.h>
#include <stdio.h>

#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdint>
#include <string>
#include <Windows.h>
#include <vector>

#include <sstream>
#include <string_view>
#include <algorithm>
#include <xmmintrin.h>
#include <map>
#include <directxmath.h>
#include <mutex>

#include <nlohmann/json.hpp>

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

#ifndef IsValidPtr
#define IsValidPtr(x) (x && !IsBadReadPtr(&x, sizeof(void*) && x != nullptr && x > nullptr))
#endif


// for convenience
using json = nlohmann::json;
bool debug = false;

using namespace std;

#include "MinHook.h"
#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#include "../includes/SimpleMath.h"

using namespace DirectX::SimpleMath;


#include "../includes/renderer/Renderer.h"
#include "xor.h"
#include "useful.h"
#include <D3DX11tex.h>

#include "image.h"

#define add_log(str) logs::add(enc(str))

imgui_render renderer;

ID3D11ShaderResourceView* immunity_image = nullptr;

void load_image_assets(ID3D11Device* device)
{
	//All your other InitImGuiCode ...



	logs::add("load_image");
	D3DX11_IMAGE_LOAD_INFO info;
	ID3DX11ThreadPump* pump{nullptr};


	D3DX11CreateShaderResourceViewFromMemory(device, rawData, sizeof(rawData), &info,
		pump, &immunity_image, 0);

	return;
};


#include "structs.h"

typedef void(__fastcall* Game_tick) ();

namespace fiber
{
	using script_func_t = void(*)();

	class fiber_task
	{
	public:
		explicit fiber_task(HMODULE hmod, script_func_t func) :
			m_hmodule(hmod),
			m_function(func)
		{ }

		~fiber_task() noexcept
		{
			if (m_script_fiber)
			{
				DeleteFiber(m_script_fiber);
			}
		}

		HMODULE get_hmodule()
		{
			return m_hmodule;
		}

		script_func_t get_function()
		{
			return m_function;
		}

		void on_tick()
		{
			if (GetTickCount64() < m_wake_at)
				return;

			if (!m_main_fiber)
			{
				m_main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);
			}

			if (m_script_fiber)
			{
				current_fiber_script = this;
				SwitchToFiber(m_script_fiber);
				current_fiber_script = nullptr;
			}
			else
			{

				m_script_fiber = CreateFiber(0, [](PVOID param)
					{
						auto this_script = static_cast<fiber_task*>(param);
						while (true)
						{
							this_script->m_function();
						}

					}, this);
			}
		}
		void wait(std::uint32_t time)
		{
			m_wake_at = GetTickCount64() + time;
			SwitchToFiber(m_main_fiber);
		}

		inline static fiber_task* get_current_script()
		{
			return current_fiber_script;
		}
	private:
		HMODULE m_hmodule{};
		script_func_t m_function{};

		std::uint32_t m_wake_at{};
		void* m_script_fiber{};
		void* m_main_fiber{};

		inline static fiber_task* current_fiber_script{};
	};
}
std::unique_ptr<fiber::fiber_task> game_fiber;


struct HackBase {
	map<int, bool> keyStates;
	int renderMethod = -1;
	bool renderReady = false;
	bool running = false;

	bool menuOpen = true;

	ImVec2 screen = ImVec2(0, 0);
	HWND window = 0;
	HMODULE hModule = 0;
	WNDPROC originalWndProc = nullptr;

	uintptr_t base = 0;
	CReplayInterface* ReplayInterface = nullptr;
	CWorld* World = nullptr;

	CViewPort* viewPort = nullptr;
	game_state_t* gta_game_state = 0;

	uintptr_t CamAddr = 0;

	Game_tick tick;


	int game_mod_base = 0;

	vector<string> messages;

	CPlayerAngles* getCam() {
		if (CamAddr){
			return *(CPlayerAngles**)(CamAddr + 0x0);
		}
		return 0;
	}
	
	CGameCameraAngles* CGameCamera;

	bool allowClearTasks = false;
} Game;

//local player etc
struct LocalData {
	CObject* player = nullptr;
} local;




#include "functions.h"

#include "database.hpp"

#include "config.hpp"
#include "tick.hpp"

#include "features/aimbot.hpp"
#include "features/ui.hpp"
#include "features/esp.hpp"
#include "features/hacks.hpp"

#include "game.hpp"

#include "hooks.hpp"

#include "pointers.hpp"
