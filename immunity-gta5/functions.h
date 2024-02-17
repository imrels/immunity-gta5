#pragma once
#include "imports.h"





typedef DWORD(__cdecl* start_shape_test_capsule_t)(Vector3 From, Vector3 To, float radius, IntersectOptions flags, DWORD entity, int p9);
start_shape_test_capsule_t start_shape_test_capsule = nullptr;

typedef int(__cdecl* get_raycast_result_t)(DWORD Handle, bool* hit, Vector3* endCoords, Vector3* surfaceNormal, DWORD* entityHit);
get_raycast_result_t get_raycast_result = nullptr;

typedef void(__cdecl* give_weapon_delayed_t)(int32_t ped, uintptr_t hash, int ammo, bool equip_now);
give_weapon_delayed_t give_weapon_delayed = nullptr;
typedef int32_t(__cdecl* pointer_to_handle_t)(intptr_t pointer);
pointer_to_handle_t pointer_to_handle = nullptr;

typedef void(__cdecl* disable_all_controlls_t)(unsigned int index);
disable_all_controlls_t disable_all_controlls;

typedef bool(__cdecl* clear_ped_task_t) (int32_t ped);
clear_ped_task_t clear_ped_task;
typedef bool(__fastcall* clear_ped_task_immediatly_t) (int32_t ped);
clear_ped_task_immediatly_t clear_ped_task_immediatly;


typedef void(__fastcall* set_ped_infinite_ammo_clip_t)(int32_t ped, BOOL toggle);
set_ped_infinite_ammo_clip_t set_ped_infinite_ammo_clip = nullptr;

float SquareRootFloat(float number) {
	long i;
	float x, y;
	const float f = 1.5F;

	x = number * 0.5F;
	y = number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (f - (x * y * y));
	y = y * (f - (x * y * y));
	return number * y;
}

float get_distance(Vector3 to, Vector3 from) {
	return (SquareRootFloat(
		((to.x - from.x) * (to.x - from.x)) +
		((to.y - from.y) * (to.y - from.y)) +
		((to.z - from.z) * (to.z - from.z))
	));
}


float screen_distance(float Xx, float Yy, float xX, float yY) {
	return SquareRootFloat((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
}

/*
auto screen_distance = [](double a, double b, double c, double d) {
return sqrtf(pow(c - a, 2.0) + pow(d - b, 2.0));
};*/



static auto rage_joaat = [](const char* str) -> std::int32_t {
	static auto to_lowercase = [](char c) -> char {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	};

	std::uint32_t hash = 0;
	while (*str) {
		hash += to_lowercase(*str++);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return (signed)hash;
};



Vector3 PredictMovement(Vector3 TargetPos, Vector3 TargetSpeed, float mul) {
	Vector3 temp_pos = TargetPos;
	float d = screen_distance(temp_pos.x, temp_pos.y, temp_pos.x + TargetSpeed.x, temp_pos.y + TargetSpeed.y);
	Vector3 normalize_target_speed(0, 0, 0);
	if (d > 0) {
		normalize_target_speed = TargetSpeed / d;
		temp_pos.x += (normalize_target_speed.x * mul);
		temp_pos.y += (normalize_target_speed.y * mul);
	}

	return temp_pos;
}



typedef BOOLEAN(__cdecl* WorldToScreen_t)(Vector3* WorldPos, float* x, float* y);
WorldToScreen_t WorldToScreenAddr = nullptr;

bool WorldToScreen(Vector3 WorldPos, ImVec2* screen) {
	if (IsValidPtr(WorldToScreenAddr)) {
		ImVec2 pos;
		if (static_cast<BOOLEAN>(WorldToScreenAddr(&WorldPos, &pos.x, &pos.y))) {
			screen->x = pos.x * ImGui::GetIO().DisplaySize.x; // or Game.screen.x
			screen->y = pos.y * ImGui::GetIO().DisplaySize.y; // or Game.screen.y

			return true;
		}
	}
	return false;
}


bool isW2SValid(ImVec2 coords) {
	return (coords.x > 1.0f && coords.y > 1.0f);
}


bool WorldToScreen2(Vector3 pos, ImVec2* out) {
	if (!IsValidPtr(Game.viewPort)) return false;
	Vector3	tmp;

	tmp.x = (Game.viewPort->fViewMatrix[1] * pos.x) + (Game.viewPort->fViewMatrix[5] * pos.y) + (Game.viewPort->fViewMatrix[9] * pos.z) + Game.viewPort->fViewMatrix[13];		//row 2
	tmp.y = (Game.viewPort->fViewMatrix[2] * pos.x) + (Game.viewPort->fViewMatrix[6] * pos.y) + (Game.viewPort->fViewMatrix[10] * pos.z) + Game.viewPort->fViewMatrix[14];	//row 3
	tmp.z = (Game.viewPort->fViewMatrix[3] * pos.x) + (Game.viewPort->fViewMatrix[7] * pos.y) + (Game.viewPort->fViewMatrix[11] * pos.z) + Game.viewPort->fViewMatrix[15];	//row 4

	if (tmp.z < 0.001f)
		return false;

	tmp.z = 1.0f / tmp.z;

	tmp.x *= tmp.z;
	tmp.y *= tmp.z;

	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	out->x = ((w / 2.f) + (.5f * tmp.x * w + 1.f)); /// ScreenWidth;
	out->y = ((h / 2.f) - (.5f * tmp.y * h + 1.f)); /// ScreenHeight;

	//if (!isW2SValid(*out)) return false;

	return true;
}

uint8_t* GetBone1 = 0;
Vector3 FromM128(__m128 in) {
	return Vector3(in.m128_f32[0], in.m128_f32[1], in.m128_f32[2]);
}






typedef struct D3DXVECTOR4 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;
} D3DXVECTOR4;



typedef void* (__fastcall* GetBoneFromMask2)(CObject* pThis, D3DXVECTOR4& vBonePos, WORD dwMask);
GetBoneFromMask2 GetBoneFunc;
Vector3 GetBonePosition(CObject* pThis, const int32_t wMask) {
	try {
		D3DXVECTOR4 tempVec4;
		GetBoneFunc(pThis, tempVec4, wMask);

		return Vector3(tempVec4.x, tempVec4.y, tempVec4.z);
	} catch (const std::exception& e) {

		printf("ERror BonePos %s\n", e.what());
	}
	return Vector3();
}

Vector3 GetBonePosition(CObject* pThis, Bones bone) {
	return GetBonePosition(pThis, (WORD)bone);
}


ImVec2 GetBoneScreenPosition(CObject* pThis, Bones bone) {
	ImVec2 bone2d;
	Vector3 bone3d = GetBonePosition(pThis, bone);
	if (WorldToScreen(bone3d, &bone2d)) {
		return bone2d;
	}
	return bone2d;
}




int GetPedDrawableVariation(CObject* ped, int group) {
	int res = -1;
	if (group > -1 && group < 13) {
		if (ped != nullptr && ped->pCPedStyle->propIndex[group] != 255)
			res = (int)ped->pCPedStyle->propIndex[group];
	}
	return res;
}

int GetPedTextureVariation(CObject* ped, int group) {
	int res = -1;
	if (group > -1 && group < 13) {
		if (ped != nullptr && ped->pCPedStyle->textureIndex[group] != 255)
			res = (int)ped->pCPedStyle->textureIndex[group];
	}
	return res;
}

int GetPedPaletteVariation(CObject* ped, int group) {
	int res = -1;
	if (group > -1 && group < 13) {
		if (ped != nullptr && ped->pCPedStyle->paletteIndex[group] != 255)
			res = (int)ped->pCPedStyle->paletteIndex[group];
	}
	return res;
}

string GetPedComponentHash(CObject* ped) {
	string r = "\0";

	for (int i = 0; i < 13; i++) {
		int c = GetPedTextureVariation(ped, i);
		if (c > -1) {
			r += to_string(c);
		}
	}
	for (int i = 0; i < 13; i++) {
		int c = GetPedPaletteVariation(ped, i);
		if (c > -1) {
			r += to_string(c);
		}
	}
	return r;
}

//==========================================================================================================================
// Game Thread



typedef DWORD(__cdecl* tSTART_SHAPE_TEST_CAPSULE)(PVector3 From, PVector3 To, float radius, IntersectOptions flags, DWORD entity, int p9);
tSTART_SHAPE_TEST_CAPSULE _START_SHAPE_TEST_CAPSULE = nullptr;

typedef int(__cdecl* t_GET_RAYCAST_RESULT)(DWORD Handle, bool* hit, PVector3* endCoords, PVector3* surfaceNormal, DWORD* entityHit);
static t_GET_RAYCAST_RESULT _GET_RAYCAST_RESULT = nullptr;

// game hooks
typedef bool(__stdcall* Is_Dlc_Present_t) (std::uint64_t hash, bool a2);
Is_Dlc_Present_t pIs_Dlc_Present = NULL;
Is_Dlc_Present_t Is_Dlc_Present;

typedef BOOL(__stdcall* sub_7FF766264E90_t) (DWORD hash, int* out, int unk0);
sub_7FF766264E90_t sub_7FF766264E90 = NULL;
sub_7FF766264E90_t psub_7FF766264E90 = NULL;





typedef __int64(__stdcall* get_rendering_cam_t) ();
get_rendering_cam_t get_rendering_cam = NULL;
get_rendering_cam_t pget_rendering_cam = NULL;



typedef __int64(__stdcall* stat_get_int_t) (__int64 a1, __int64 a2);
stat_get_int_t stat_get_int = NULL;
stat_get_int_t pstat_get_int = NULL;


typedef bool(__stdcall* does_cam_exists_t) (__int64 a1);
does_cam_exists_t does_cam_exists = NULL;
does_cam_exists_t pdoes_cam_exists = NULL;


typedef bool(__stdcall* is_cam_active_t) (__int64 a1);
is_cam_active_t is_cam_active = NULL;
is_cam_active_t pis_cam_active = NULL;

typedef bool(__stdcall* is_aim_cam_active_t) ();
is_aim_cam_active_t is_aim_cam_active = NULL;
is_aim_cam_active_t pis_aim_cam_active = NULL;


typedef float(__stdcall* get_gameplay_cam_zoom_t) ();
get_gameplay_cam_zoom_t get_gameplay_cam_zoom = NULL;
get_gameplay_cam_zoom_t pget_gameplay_cam_zoom = NULL;



//void* HandleEventWrap(void* group, rage::fwEvent* event) {

typedef void* (__stdcall* handle_event_t) (void* group, void* eventData);
handle_event_t o_handle_event;
handle_event_t handle_event;
void* hk_handle_event(void* group, void* eventData) {
	const char* eventName = typeid(eventData).name();
	cout << "eventName ->" << eventName << endl;

	return o_handle_event(group, eventData);
}

typedef bool (__stdcall* get_event_data_t) (std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
get_event_data_t get_event_data;
get_event_data_t o_get_event_data;

bool hk_get_event_data(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount) {

	cout << "eventGroup ->" << eventGroup << endl;
	cout << "eventIndex ->" << eventIndex << endl;
	cout << "argCount ->" << argCount << endl;

	return o_get_event_data(eventGroup, eventIndex, args, argCount);
}



void onGameTick() {
	//cout << "ontick" << endl;	
	if (Game.running) {
		static uint64_t    last = 0;
		uint64_t        cur = *(uint64_t*)(FrameCount);
		if (last != cur) {
			last = cur;
			Game.tick();
		}
	}
}
// stat_get_int
float __stdcall hk_get_gameplay_cam_zoom() {
	cout << "hk_get_gameplay_cam_zoom" << endl;
	if (Game.running) {
		onGameTick();
	}
	return pget_gameplay_cam_zoom();
}


// stat_get_int
bool __stdcall hk_stat_get_int(__int64 a1, __int64 a2) {
	cout << "stat_get_int" << endl;
	if (Game.running) {
		onGameTick();
	}
	return pstat_get_int(a1, a2);
}
// does_cam_exists
bool __fastcall hk_is_aim_cam_active() {
	cout << "hk_is_aim_cam_active" << endl;
	if (Game.running) {
		onGameTick();
	}
	return pis_aim_cam_active();
}



// does_cam_exists
bool __fastcall hk_does_cam_exists(__int64 a1) {
	if (Game.running) {
		onGameTick();
	}
	return pdoes_cam_exists(a1);
}



bool __stdcall hookIs_Dlc_Present(std::uint64_t hash, bool a2) {
	//cout << "hookIs_Dlc_Present" << endl;
	if (Game.running) {
		onGameTick();
	}
	return pIs_Dlc_Present(hash, a2);
}




//==========================================================================================================================

