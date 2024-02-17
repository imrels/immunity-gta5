#include "imports.h"
#include "secure.hpp"

DWORD __stdcall InitializeHook(PVOID) {

	config::load();


	if (pointers::findAllPatterns()) {
		if (hook::enable()) {
			logs::add("enabled hooks");
			Game.running = true;


			std::thread aimThread(aimbot::init);
			aimThread.detach();

			std::thread hacksThread(hacks::init);
			hacksThread.detach();
		}
	}

	tools::unlink_module_peb(Game.hModule);
	while (Game.running) {
		hook::patch_wndproc();
		std::this_thread::sleep_for(500ms);
	}
	tools::relink_module_peb(Game.hModule);



	Game.running = false;

	Sleep(500);
	hook::disable();

	FreeLibraryAndExitThread((HMODULE)Game.hModule, 0);
	return 1;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		logs::clear();
		Game.base = (uintptr_t)GetModuleHandleA(0);
		Game.hModule = hModule;
		logs::add("module entry");
		Sleep(100);
		CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {

	}
	return TRUE;
}

