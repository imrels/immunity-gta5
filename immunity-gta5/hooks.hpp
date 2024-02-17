#pragma once
#include "imports.h"
#include <dwmapi.h>


using namespace std;
IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* D3D11ResizeBuffersHook) (IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
typedef void(__stdcall* D3D11DrawHook) (ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11DrawIndexedInstancedHook) (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);



D3D11PresentHook phookD3D11Present = NULL;
D3D11ResizeBuffersHook phookD3D11ResizeBuffers = NULL;
D3D11PSSetShaderResourcesHook phookD3D11PSSetShaderResources = NULL;
D3D11DrawHook phookD3D11Draw = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11DrawIndexedInstancedHook phookD3D11DrawIndexedInstanced = NULL;
D3D11CreateQueryHook phookD3D11CreateQuery = NULL;


using namespace std::chrono_literals;
namespace hook {

	string renderMethod = "-1";
	//==========================================================================================================================

	std::chrono::time_point<std::chrono::high_resolution_clock> resize_counter = std::chrono::high_resolution_clock::now();
	bool present_setup = false;


	//=========================================================================================================================//
	typedef LRESULT(__stdcall* WINPROC) (_In_ HWND hWnd,
		_In_ UINT Msg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam);

	WINPROC OriginalDefWindowProc = nullptr;
	WINPROC OriginalWindowProc = nullptr;


	LRESULT CALLBACK hook_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		static bool callOnce = false;
		if (!callOnce) {
			add_log("hook WndProc called");
			callOnce = true;
		}


		if (uMsg == WM_KEYUP) {
			if (wParam == config::get("menu", "menu_key", 35)) {
				Game.menuOpen = !Game.menuOpen;
			}
			if (wParam == config::get("menu", "unload_key", VK_PAUSE)) {
				Game.running = false;
			}
		}

		if (present_setup && Game.window) {
			ImGuiIO& io = ImGui::GetIO();
			POINT mPos;
			GetCursorPos(&mPos);
			ScreenToClient(Game.window, &mPos);
			ImGui::GetIO().MousePos.x = mPos.x;
			ImGui::GetIO().MousePos.y = mPos.y;





			if (uMsg == WM_KEYUP) {
				Game.keyStates[(int)wParam] = false;
			}
			else if (uMsg == WM_KEYDOWN) {
				Game.keyStates[(int)wParam] = true;
			}


			if (uMsg == WM_MOUSEMOVE) {
				//logs::add("WM_MOUSEMOVE");
				aimbot::tick();
			}

			if (Game.menuOpen) {
				if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
					return true;
				}
				return 1l;
			}
		}

		return CallWindowProc(Game.originalWndProc, hWnd, uMsg, wParam, lParam);
	}




	HRESULT __stdcall hookedD3D11ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {

		if (present_setup && Game.window) {
			Game.renderReady = false;
			renderer.release();
			add_log("reloading d3d11 device");
			std::this_thread::sleep_for(100ms);


			if (Game.originalWndProc) {
				SetWindowLongPtr(Game.window, GWLP_WNDPROC, (LONG_PTR)Game.originalWndProc);
			}

			//TODO crash when resizing + unloading
			return phookD3D11ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		}

		return phookD3D11ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	}
	int64_t reload_time = 2000;
	HRESULT __stdcall hookedD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
		if (!present_setup) {

			auto now = std::chrono::high_resolution_clock::now();


			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - resize_counter).count();
			if (dur > reload_time) {
				present_setup = true;
				cout << "present" << endl;
				add_log("initiated d3d11");
				//get device


				DXGI_SWAP_CHAIN_DESC desc;

				if (SUCCEEDED(pSwapChain->GetDesc(&desc))) {
					Game.window = desc.OutputWindow;
					add_log("get output window");


					renderer.SetResourceLoad(load_image_assets);
					renderer.Initialize(Game.window, pSwapChain);

					//ui::pre_init();
					//ui::create_styles();

					Game.renderReady = true;
				}

				resize_counter = std::chrono::high_resolution_clock::now();
			}
		}
		if (!Game.renderReady && present_setup) {
			resize_counter = std::chrono::high_resolution_clock::now();
			reload_time = 5000;
			add_log("waiting for init");
			present_setup = false;
		}
		if (!Game.renderReady) return phookD3D11Present(pSwapChain, SyncInterval, Flags);


		//if ((WNDPROC)GetWindowLongPtr(Game.window, GWLP_WNDPROC) != Game.originalWndProc) {
		//	Game.originalWndProc = (WNDPROC)SetWindowLongPtr(Game.window, GWLP_WNDPROC, (LONG_PTR)hook_WndProc);
		//}


		WINDOWINFO info;
		GetWindowInfo(Game.window, &info);
		Game.screen.x = ((info.rcClient.right) - (info.rcClient.left));
		Game.screen.y = ((info.rcClient.bottom) - (info.rcClient.top));


		if (Game.menuOpen) {
			//WantCaptureMouse 

			ImGui::GetIO().WantCaptureMouse = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else {
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui::GetIO().MouseDrawCursor = Game.menuOpen;



		renderer.BeginScene();

		game::render();

		renderer.EndScene();
		renderer.Render();

		return phookD3D11Present(pSwapChain, SyncInterval, Flags);
	}

	//==========================================================================================================================

	LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	uintptr_t discordoverlay_presentd3d11 = 0;
	uintptr_t discordoverlay_resized3d11 = 0;




	bool check_for_nvidiaoverlay() {
		//return false;
		HWND h = FindWindow("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay");

		return (h != nullptr);
	}
	bool check_for_discordoverlay() {
		return ((uintptr_t)GetModuleHandleA("DiscordHook64.dll") != 0) &&
			(discordoverlay_presentd3d11 != 0) &&
			(discordoverlay_resized3d11 != 0);
	}

	uintptr_t gameoverlay_presentd3d11 = 0;
	uintptr_t gameoverlay_resized3d11 = 0;
	bool check_for_gameoverlay64() {
		return ((uintptr_t)GetModuleHandleA("GameOverlayRenderer64.dll") != 0) &&
			(gameoverlay_presentd3d11 != 0) &&
			(gameoverlay_resized3d11 != 0);
	}
	bool hook_discordoverlay() {

		std::cout << "discordoverlay_presentd3d1111 " << discordoverlay_presentd3d11 << std::endl;
		if (discordoverlay_presentd3d11 != 0 && discordoverlay_resized3d11 != 0) {
			if (MH_CreateHook((DWORD_PTR*)discordoverlay_presentd3d11, hookedD3D11Present, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) {
				add_log("failed creating discord present hook");
				return false;
			}
			if (MH_EnableHook((DWORD_PTR*)discordoverlay_presentd3d11) != MH_OK) {
				add_log("failed enabling discord present hook");
				return false;
			}

			if (MH_CreateHook((DWORD_PTR*)discordoverlay_resized3d11, hookedD3D11ResizeBuffers, reinterpret_cast<void**>(&phookD3D11ResizeBuffers)) != MH_OK) {
				add_log("failed creating discord resize hook");
				return false;
			}
			if (MH_EnableHook((DWORD_PTR*)discordoverlay_resized3d11) != MH_OK) {
				add_log("failed enabling discord resize hook");
				return false;
			}
			add_log("discord overlay hooked");
			return true;

		}
		add_log("couldn´t find discord address");
		return false;
	}
	bool hook_gameoverlay64() {
		if (gameoverlay_presentd3d11 != 0 && gameoverlay_resized3d11 != 0) {

			if (MH_CreateHook((DWORD_PTR*)gameoverlay_presentd3d11, hookedD3D11Present, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) {
				add_log("failed creating steam present hook");
				std::cout << "MH_CreateHook failed" << std::endl;
				return false;
			}

			if (MH_EnableHook((DWORD_PTR*)gameoverlay_presentd3d11) != MH_OK) {
				add_log("failed enabling steam present hook");
				std::cout << "MH_EnableHook failed" << std::endl;
				return false;
			}

			if (MH_CreateHook((DWORD_PTR*)gameoverlay_resized3d11, hookedD3D11ResizeBuffers, reinterpret_cast<void**>(&phookD3D11ResizeBuffers)) != MH_OK) {
				add_log("failed enabling steam resize hook");
				std::cout << "gameoverlay_resized3d11 MH_CreateHook failed" << std::endl;
				return false;
			}
			if (MH_EnableHook((DWORD_PTR*)gameoverlay_resized3d11) != MH_OK) {
				add_log("failed enabling steam resize hook");
				std::cout << "gameoverlay_resized3d11 MH_EnableHook failed" << std::endl;
				return false;
			}


			std::cout << "gameoverlay_presentd3d11 created" << std::endl;
			std::cout << "gameoverlay_resized3d11 created" << std::endl;
			add_log("steam overlay hooked");
			return true;
		}
		return false;
	}



	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	IDXGISwapChain* pSwapChain;
	HWND pWindow;
	WNDCLASSEXA wc;

	DWORD_PTR* pSwapChainVtable = NULL;
	DWORD_PTR* pContextVTable = NULL;
	DWORD_PTR* pDeviceVTable = NULL;

	bool cleanupOverlay = false;

	bool hook_d3d11(HWND hWnd, bool destroyAfter) {

		D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL obtainedLevel;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));
		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.OutputWindow = pWindow;
		scd.SampleDesc.Count = 1;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed = ((GetWindowLongPtr(pWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		//scd.Windowed = true;

		scd.BufferDesc.Width = 1;
		scd.BufferDesc.Height = 1;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 1;

		UINT createFlags = 0;
		IDXGISwapChain* d3dSwapChain = 0;

		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags, requestedLevels, sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, &obtainedLevel, &pContext))) {
			//MessageBox(Game.window, "Failed to create directX device and swapchain!", "Error", MB_ICONERROR);
			add_log("Failed to create directX device and swapchain!");
			return false;
		}

		pSwapChainVtable = (DWORD_PTR*)pSwapChain;
		pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

		pContextVTable = (DWORD_PTR*)pContext;
		pContextVTable = (DWORD_PTR*)pContextVTable[0];

		pDeviceVTable = (DWORD_PTR*)pDevice;
		pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

		if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[8], hookedD3D11Present, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) {
			add_log("failed creating present hook");
			return false;
		}
		if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) {
			add_log("failed enabling present hook");
			return false;
		}

		if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[13], hookedD3D11ResizeBuffers, reinterpret_cast<void**>(&phookD3D11ResizeBuffers)) != MH_OK) {
			add_log("failed creating ResizeBuffers hook");
			return false;
		}
		if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[13]) != MH_OK) {
			add_log("failed enabling ResizeBuffers hook");
			return false;
		}


		DWORD dwOld;
		if (!VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld)) {
			add_log("failed vp");
			return false;
		}
		if (!destroyAfter) {
			add_log("overlay renderer created");
			return true;
		}
		// remove stuff

		pSwapChain->Release();
		pSwapChain = NULL;

		pDevice->Release();
		pDevice = NULL;

		pContext->Release();
		pContext = NULL;

		::DestroyWindow(hWnd);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);

		add_log("hwnd renderer created");
		return true;
	}

	bool hook_d3d11() {
		wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "Valve001", NULL };
		RegisterClassExA(&wc);
		pWindow = CreateWindowA("Valve001", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

		return hook_d3d11(pWindow, true);
	}
	bool hook_nvidia_overlay() {
		pWindow = FindWindow("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay");
		if (!pWindow) return false;
		int i = (int)GetWindowLong(pWindow, -20);

		SetWindowLongPtr(pWindow, -20, (LONG_PTR)(i | 0x20));


		MARGINS margin;
		UINT opacity_flag, color_key_flag, color_key = 0;
		UINT opacity = 0;

		//margin.cyBottomHeight = -1;
		//margin.cxLeftWidth = -1;
		//margin.cxRightWidth = -1;
		//margin.cyTopHeight = -1;

		DwmExtendFrameIntoClientArea(pWindow, &margin);

		opacity_flag = 0x02;
		color_key_flag = 0x01;
		color_key = 0x000000;
		opacity = 0xFF;

		SetLayeredWindowAttributes(pWindow, color_key, opacity, opacity_flag);


		//SetWindowPos(pWindow, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);

		ShowWindow(pWindow, SW_SHOW);


		return hook_d3d11(pWindow, true);
	}
	void shutdown_d3d11() {

		pSwapChain->Release();
		pSwapChain = NULL;

		pDevice->Release();
		pDevice = NULL;

		pContext->Release();
		pContext = NULL;
		if (pWindow) {

			::DestroyWindow(pWindow);

			::UnregisterClass(wc.lpszClassName, wc.hInstance);

		}
	}



	//game thread hook

	//https://github.com/citizenfx/fivem/blob/cbe56f78f86bebb68d7960a38c3cdc31c7d76790/code/components/rage-scripting-five/src/scrThread.cpp#L58
	void call_thread(game_thread* this_ptr, int ops_to_execute) {
		if ((
			/*Ragemp*/(this_ptr->m_context.m_script_hash == 0x26FB4AB9 || this_ptr->m_context.m_script_hash == 1104607124 || this_ptr->m_context.m_script_hash == 3381724246) ||
			/*AltV*/(this_ptr->m_context.m_script_hash == 2003913879 || this_ptr->m_context.m_script_hash == 3522812357/* || this_ptr->m_context.m_script_hash == 20 || this_ptr->m_context.m_script_hash == 21*/)
			)) {
			auto& thread = game_thread::get();
			auto orig_thread = thread;
			thread = this_ptr;
			game_fiber->on_tick();
			thread = orig_thread;
		}
	}

	void on_native_thread() {

		static uint64_t    last = 0;
		uint64_t        cur = *(uint64_t*)(FrameCount);
		if (last != cur) {
			last = cur;

			game::tick();

			tick::pnative.on_tick();
		}

		game_fiber->wait(0);
	}

	void call_fiber() {
		//Log::Warning(_xor_("[+] Fiber >> Initializing fiber..."));
		add_log("init fiber");
		game_fiber = std::make_unique<fiber::fiber_task>(nullptr, on_native_thread);
	}

	uintptr_t hooked_native_thread(game_thread* this_ptr, int ops_to_execute) {
		call_thread(this_ptr, ops_to_execute);
		return original_native_thread(this_ptr, ops_to_execute);
	}


	uintptr_t original_set_shoot_at_coords = 0;
	void on_shoot_at_coords(native::nsdk::NativeContext& context) {
		add_log("on_shoot_at_coords");
		reinterpret_cast<decltype(&on_shoot_at_coords)>(original_set_shoot_at_coords)(context);
	}
	uintptr_t original_task_shoot_at_coord = 0;
	void on_task_shoot_at_coord(native::nsdk::NativeContext& context) {
		add_log("task_shoot_at_coord");
		reinterpret_cast<decltype(&on_task_shoot_at_coord)>(original_task_shoot_at_coord)(context);
	}

	uintptr_t original_get_is_task_active = 0;
	void on_get_is_task_active(native::nsdk::NativeContext& context) {
		int32_t task = context.GetArgument<int32_t>(1);
		reinterpret_cast<decltype(&on_get_is_task_active)>(original_get_is_task_active)(context);
	}

	//set_vehicle_engine_on(type::vehicle vehicle, bool value, bool instantly, bool noautoturnon)
	uintptr_t original_set_vehicle_engine_on = 0;
	void on_set_vehicle_engine_on(native::nsdk::NativeContext& context) {
		if (config::get("hacks", "force_engine", 0)) {
			auto ped = native::player::player_ped_id();
			bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(ped, false);
			if (is_in_vehicle) {
				if (context.GetArgument<int32_t>(0) == native::ped::get_vehicle_ped_is_using(ped)) {
					context.SetArgument<bool>(1, true);
				}
			}
		}
		reinterpret_cast<decltype(&on_set_vehicle_engine_on)>(original_set_vehicle_engine_on)(context);
	}
	uintptr_t original_task_leave_vehicle = 0;
	void on_task_leave_vehicle(native::nsdk::NativeContext& context) {
		if (config::get("misc", "no_force_exit", 0)) {
			auto ped = native::player::player_ped_id();
			if (context.GetArgument<int32_t>(0) == ped)
				return;
		}
		reinterpret_cast<decltype(&on_task_leave_vehicle)>(original_task_leave_vehicle)(context);
	}


	uintptr_t original_get_player_invincible = 0;
	void on_get_player_invincible(native::nsdk::NativeContext& context) {
		if (config::get("misc", "spoof_godmode_checks", 0)) {
			auto ped = native::player::player_ped_id();
			if (context.GetArgument<int32_t>(0) == ped) {
				//context.SetArgument<bool>(1, true);
				context.SetResult<bool>(0, false);
			}
		}
		reinterpret_cast<decltype(&on_get_player_invincible)>(original_get_player_invincible)(context);
	}



	uintptr_t original_create_ped = 0;
	void on_create_ped(native::nsdk::NativeContext& context) {
		//int pedtype, type::hash modelhash, float x, float y, float z, float heading, bool isnetwork, bool thisscriptcheck
		if (config::get("misc", "hide_anticheat_npcs", 0)) {
			context.SetArgument<bool>(2, 0.f);//x
			context.SetArgument<bool>(3, 0.f);//y
			context.SetArgument<bool>(4, 0.f);//z
		}
		reinterpret_cast<decltype(&on_create_ped)>(original_create_ped)(context);
	}


//	uintptr_t hooked_gta_weapon_shoot_event(__int64 a1, __int64 a2, __int64 a3, float* a4, char a5) {
//		auto shoot_coord_ptr = (float*)(a3 + 48);
//		auto bullet_impact_ptr = a4;
//
//		auto shoot_coord = Vector3(shoot_coord_ptr[0], shoot_coord_ptr[1], shoot_coord_ptr[2]);
//		auto bullet_impact = Vector3(bullet_impact_ptr[0], bullet_impact_ptr[1], bullet_impact_ptr[2]);
//
//		auto bullet_trace_ptr = new bullet_trace();
//		bullet_trace_ptr->m_tick_count = GetTickCount64();
//		bullet_trace_ptr->m_shoot_coord = shoot_coord;
//		bullet_trace_ptr->m_bullet_impact = bullet_impact;
//
//		c_pools::instance()->bullet_trace_pool.push_back(bullet_trace_ptr);
//
//<<<<<<< Updated upstream
//		return o_gta_event_gun_shot(a1, a2, a3, a4, a5);
//=======
	//uintptr_t hooked_gta_gun_shot(__int64 a1, __int64 a2, __int64 a3, float* a4) {
	uintptr_t hooked_gta_gun_shot(
		__int64 a1,
		__int64 a2,
		float* a3,
		float* a4,
		unsigned int a5,
		unsigned int a6,
		unsigned int a7,
		int a8,
		char a9,
		int a10,
		char a11,
		char a12) {
		//https://i.imgur.com/MgJqzlH.png
		//auto shot_coord_ptr = (float*)(a3 + 48);
		//auto bullet_impact_ptr = a4;
		//
		//auto shot_coord = Vector3(shot_coord_ptr[0], shot_coord_ptr[1], shot_coord_ptr[2]);
		//auto bullet_impact = Vector3(bullet_impact_ptr[0], bullet_impact_ptr[1], bullet_impact_ptr[2]);
		logs::add("hooked_gta_gun_shot");
		logs::add(to_string(a4[0]));
		logs::add(to_string(a4[1]));
		logs::add(to_string(a4[2]));
		logs::add("---");
		logs::add(to_string(a3[0]));
		logs::add(to_string(a3[1]));
		logs::add(to_string(a3[2]));
		//
		//logs::add("-");
		//
		//logs::add(to_string(a4[0]));
		//logs::add(to_string(a4[1]));
		//logs::add(to_string(a4[2]));
		//logs::add("hooked_gta_gun_shot");
		return original_gta_event_gun_shot(a1, a2, a3, a4,a5,a6,a7,a8,a9,a10,a11,a12);
	}

	uintptr_t original_clear_ped_tasks_immediately = 0;
	void on_clear_ped_tasks_immediately(native::nsdk::NativeContext& context) {
		if (config::get("misc", "spoof_clear_ped_tasks", 0)) {
			auto ped = native::player::player_ped_id();
			if (context.GetArgument<int32_t>(0) == ped) {
				if (Game.allowClearTasks) {
					Game.allowClearTasks = false;
				}
				else {
					return;
				}
			}
		}
		reinterpret_cast<decltype(&on_clear_ped_tasks_immediately)>(original_clear_ped_tasks_immediately)(context);
	}


	bool hook_game() {
		//	MH_STATUS hkEventStatus = MH_CreateHook((BYTE* const)GetEventData, (BYTE*)hookGetEventData, reinterpret_cast<void**>( &pGetEventData ));
		//	MH_STATUS hkEntityDeadStatus = MH_CreateHook((BYTE* const)IsEntityDead, (BYTE*)hookIsEntityDead, reinterpret_cast<void**>( &pIsEntityDead ));
		/*if (!Is_Dlc_Present) {
			logs::add("game hook addr not found");
			return false;
		}
		auto hk = MH_CreateHook((BYTE* const)Is_Dlc_Present, (BYTE*)hookIs_Dlc_Present, reinterpret_cast<void**>(&pIs_Dlc_Present));
		if (hk != MH_OK) {
			logs::add("failed creating game hook");
			cout << "Failed Game HK" << MH_StatusToString(hk) << endl;
			return false;

		}
		if (MH_EnableHook((BYTE* const)Is_Dlc_Present) != MH_OK) {
			logs::add("failed enabling game hook");
			cout << "failed enable Is_DLC_present hook..." << endl;
			return false;
		}*/

		//if(does_cam_exists) {
		//	auto hk1 = MH_CreateHook((BYTE* const)does_cam_exists, (BYTE*)hk_does_cam_exists, reinterpret_cast<void**>(&pdoes_cam_exists));
		//	if(hk1 != MH_OK) {
		//		logs::add("failed creating game hook 2");
		//		cout << "Failed Game does_cam_exists " << MH_StatusToString(hk1) << endl;
		//		return false;
		//	}
		//	if(MH_EnableHook((BYTE* const)does_cam_exists) != MH_OK) {
		//		logs::add("failed enabling game hook 2");
		//		cout << "failed enable does_cam_exists hook..." << endl;
		//		return false;
		//	}
		//}




		//Hook tick / thread

		call_fiber(); //set game_fiber
		MH_CreateHook(gta_script_thread_tick, hooked_native_thread, reinterpret_cast<void**>(&original_native_thread));
		if (gta_script_thread_tick) {
			if (MH_EnableHook(gta_script_thread_tick) == MH_OK) {
				logs::add("hijack thread success");
			}
		}
		//hook coords
		auto set_vehicle_engine_on = native::invoker::find_native_handler(0x2497c4717c8b881e);
		if (set_vehicle_engine_on) {
			MH_CreateHook(set_vehicle_engine_on, on_set_vehicle_engine_on, reinterpret_cast<void**>(&original_set_vehicle_engine_on));
			if (MH_EnableHook(set_vehicle_engine_on) == MH_OK) {
				logs::add("set_vehicle_engine_on ok");
			}
		}

		auto task_leave_vehicle = native::invoker::find_native_handler(0xd3dbce61a490be02);
		if (task_leave_vehicle) {
			MH_CreateHook(task_leave_vehicle, on_task_leave_vehicle, reinterpret_cast<void**>(&original_task_leave_vehicle));
			if (MH_EnableHook(task_leave_vehicle) == MH_OK) {
				add_log("task_leave_vehicle ok");
			}
		}
		//godmode stuff

		//config::get("misc", "spoof_godmode_checks", 0)


		auto get_player_invincible = native::invoker::find_native_handler(0xb721981b2b939e07);
		if (get_player_invincible) {
			MH_CreateHook(get_player_invincible, on_get_player_invincible, reinterpret_cast<void**>(&original_get_player_invincible));
			if (MH_EnableHook(get_player_invincible) == MH_OK) {
				add_log("get_player_invincible ok");
			}
		}


		//static bool get_ped_last_weapon_impact_coord(type::ped ped, PVector3 * coords) {
		//	return invoker::invoke<bool>(0x6c4d0409ba1a2bc2, ped, coords);
		//} // 0x6c4d0409ba1a2bc2 0x9b266079


		auto create_ped = native::invoker::find_native_handler(0xd49f9b0955c367de);
		if (create_ped) {
			MH_CreateHook(create_ped, on_create_ped, reinterpret_cast<void**>(&original_create_ped));
			if (MH_EnableHook(create_ped) == MH_OK) {
				add_log("create_ped ok");
			}
		}


		//if (gta_event_gun_shot) {
		//	MH_CreateHook(gta_event_gun_shot, hooked_gta_gun_shot, reinterpret_cast<void**>(&original_gta_event_gun_shot));
		//	if (MH_EnableHook(gta_event_gun_shot) == MH_OK) {
		//		add_log("gta_event_gun_shot ok");
		//	}
		//}

		//static PVector3 get_entity_coords(type::entity entity, bool alive) {
		//	return invoker::invoke<PVector3>(0x3fef770d40960d5a, entity, alive);
		//} // 0x3fef770d40960d5a 0x1647f1cb

		//if (gta_event_gun_shot) {
		//	MH_CreateHook(gta_event_gun_shot, hooked_gta_gun_shot, reinterpret_cast<void**>(&original_gta_event_gun_shot));
		//	if (MH_EnableHook(gta_event_gun_shot) == MH_OK) {
		//		add_log("gta_event_gun_shot ok");
		//	}
		//}






		//auto play_entity_anim = native::invoker::find_native_handler(0x7fb218262b810701);
		//if(play_entity_anim) {
		//	MH_CreateHook(play_entity_anim, on_play_entity_anim, reinterpret_cast<void**>(&original_play_entity_anim));
		//	if(MH_EnableHook(play_entity_anim) == MH_OK) {
		//		add_log("play_entity_anim ok");
		//	}
		//}



		// //auto set_ped_shoots_at_coord = native::invoker::find_native_handler(0x96a05e4fb321b1ba);
		//MH_CreateHook(set_ped_shoots_at_coord, on_shoot_at_coords, reinterpret_cast<void**>(&original_set_shoot_at_coords));
		//if (MH_EnableHook(set_ped_shoots_at_coord) == MH_OK) {
		//	add_log("set_ped_shoots_at_coord ok");
		//}
		//
		//auto shoot_single_bullet_between_coords = native::invoker::find_native_handler(0x867654cbc7606f2c);
		//MH_CreateHook(shoot_single_bullet_between_coords, on_shoot_single_bullet_between_coords, reinterpret_cast<void**>(&original_shoot_single_bullet_between_coords));
		//if (MH_EnableHook(shoot_single_bullet_between_coords) == MH_OK) {
		//	add_log("shoot_single_bullet_between_coords ok");
		//}

		//auto get_is_task_active = native::invoker::find_native_handler(0xB0760331C7AA4155);
		//MH_CreateHook(get_is_task_active, on_get_is_task_active, reinterpret_cast<void**>(&original_get_is_task_active));
		//if(MH_EnableHook(get_is_task_active) == MH_OK) {
		//	add_log("get_is_task_active ok");
		//}


		//auto get_ped_last_weapon_impact_coord = native::invoker::find_native_handler(0x6c4d0409ba1a2bc2);
		//MH_CreateHook(get_ped_last_weapon_impact_coord, on_get_ped_last_weapon_impact_coord, reinterpret_cast<void**>(&original_get_ped_last_weapon_impact_coord));
		//if (MH_EnableHook(get_ped_last_weapon_impact_coord) == MH_OK) {
		//	add_log("get_ped_last_weapon_impact_coord ok");
		//}
		//hook fire


		/*
		using task_shoot_at_coords_t = void(*)(uint64_t ped, float x, float y, float z, int duration, uint64_t firingPattern);
		task_shoot_at_coords_t org_task_shoot_at_coords;

		auto ptr_task_shoot_at_coords = (task_shoot_at_coords_t)native::invoker::find_native_handler(0x96a05e4fb321b1ba);

		MH_CreateHook(ptr_task_shoot_at_coords, hooked_task_shoot_at_coords, reinterpret_cast<void**>(&org_task_shoot_at_coords));
		if (MH_EnableHook(ptr_task_shoot_at_coords) == MH_OK) {
			//Log::Debug(_xor_("[+] Hooks >>"), _xor_("Game-Thread"), _xor_("enabled."));
			add_log("ptr_task_shoot_at_coords");
		}*/


		//auto lptrWndProc = reinterpret_cast<WINPROC>(GetWindowLongPtr(Game.window, GWLP_WNDPROC));
		//
		//add_log("lptrWndProc " + to_string((uintptr_t)lptrWndProc));
		//if (MH_CreateHook(lptrWndProc, &hook_WndProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK) {
		//	add_log("Couldn't create hook for GWLP_WNDPROC " );
		//}
		//if (MH_EnableHook(lptrWndProc) != MH_OK) {
		//	add_log("Couldn't enable GWLP_WNDPROC hook");
		//}



		//Game.originalWndProc = (WNDPROC)SetWindowLongPtr(Game.window, GWLP_WNDPROC, (LONG_PTR)hook_WndProc);
		//add_log("set wnd proc");
		//add_log("WndLogHK HK " + (string)to_string((LONG_PTR)hook_WndProc));
		//add_log("WndLogHK " + (string)to_string((GetWindowLongPtr(Game.window, GWLP_WNDPROC))));
		//add_log("WndLogHK cmp" + (string)to_string((GetWindowLongPtr(Game.window, GWLP_WNDPROC) == (LONG_PTR)hook_WndProc)));
		/*if (!get_gameplay_cam_zoom) {
			return false;
		}
		auto hk3 = MH_CreateHook((BYTE* const)get_gameplay_cam_zoom, (BYTE*)hk_get_gameplay_cam_zoom, reinterpret_cast<void**>(&pget_gameplay_cam_zoom));
		if (hk3 != MH_OK) {
			cout << "Failed Game get_gameplay_cam_zoom " << MH_StatusToString(hk3) << endl;
			return false;

		}
		if (MH_EnableHook((BYTE* const)get_gameplay_cam_zoom) != MH_OK) {
			cout << "failed enable get_gameplay_cam_zoom hook..." << endl;
			return false;
		}*/





		/*

		if (!sub_7FF766264E90) {
			return false;
		}
		auto h1k = MH_CreateHook((BYTE* const)sub_7FF766264E90, (BYTE*)hooksub_7FF766264E90, reinterpret_cast<void**>(&psub_7FF766264E90));
		if (h1k != MH_OK) {
			cout << "Failed Game sub_7FF766264E90"<< MH_StatusToString(h1k) << endl;
			return false;

		}
		if (MH_EnableHook((BYTE* const)sub_7FF766264E90) != MH_OK) {
			cout << "failed enable sub_7FF766264E90 hook..." << endl;
			return false;
		}*/

		add_log("game hooks done");
		return true;

	}
	void patch_wndproc() {
		if (!present_setup) return;
		if ((GetWindowLongPtr(Game.window, GWLP_WNDPROC) != (LONG_PTR)hook_WndProc)) {
			add_log("wndproc needs rehook");
			Game.originalWndProc = (WNDPROC)SetWindowLongPtr(Game.window, GWLP_WNDPROC, (LONG_PTR)hook_WndProc);
		}

		return;
	}



	bool hook_renderer() {
		//cout << "[!] render -> " << (check_for_gameoverlay64() ? (check_for_discordoverlay() ? "discord" : "steam") : check_for_nvidiaoverlay() ? "nvidia" : "own") << endl;

		int force_renderer = config::get("hack", "renderer", -1);
		if (force_renderer != -1) {
			logs::add(string("forcing renderer to " + (string)(force_renderer == 0 ? "steam" : force_renderer == 1 ? "discord" : force_renderer == 2 ? "nvidia" : "own")));
		}


		if ((check_for_gameoverlay64() && (force_renderer == -1)) || (force_renderer == 0)) {
			add_log("using steam overlay");
			Game.renderMethod = 0;
			return hook_gameoverlay64();
		}
		else if ((check_for_discordoverlay() && (force_renderer == -1)) || (force_renderer == 1)) {
			add_log("using discord overlay");
			Game.renderMethod = 1;
			return hook_discordoverlay();
			//} else if((check_for_nvidiaoverlay() && (force_renderer == -1)) || (force_renderer == 3)) {
			//
			//	logs::add("using nvidia renderer");
			//	Game.renderMethod = 3;
			//	cleanupOverlay = true;
			//	return hook_nvidia_overlay();
		}
		else {
			add_log("using own renderer");
			Game.renderMethod = 2;
			return hook_d3d11();
		}
		return false;
	}

	bool disable() {
		if (Game.originalWndProc) {
			SetWindowLongPtr(Game.window, GWLP_WNDPROC, (LONG_PTR)Game.originalWndProc);
		}
		add_log("Unloading");

		MH_DisableHook(MH_ALL_HOOKS);

		if (cleanupOverlay) {
			shutdown_d3d11();
		}



		FreeConsole();

		return true;
	}
	bool Ready() {
		if ((GetModuleHandleA("d3d11.dll") != NULL) || (GetModuleHandleA("dxgi.dll") != NULL)) return true;
		return false;
	}

	bool enable() {
		while (Game.running == false) {
			if (Ready()) {
				if (MH_Initialize() != MH_OK) {
					add_log("init failed");
					return false;
				}
				do {
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					add_log("waiting for game start");
				} while (*Game.gta_game_state != game_state_t::playing);

				bool renderer = hook_renderer();

				bool game = hook_game();

				Game.tick = game::tick;

				Game.menuOpen = config::get("menu", "openOnLoad", 1);

				return renderer && game;
			}
			std::this_thread::sleep_for(1000ms);
		}
		return false;
	}
}
