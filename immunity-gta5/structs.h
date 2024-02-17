#pragma once

#include <Windows.h>
#include <stdio.h> 
#include <string> 
#include <sstream> 
#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */


#include <dwmapi.h> 
#include <TlHelp32.h>


#pragma warning ( disable: 4102 ) 
#pragma warning ( disable: 4311 ) 
#pragma warning ( disable: 4312 ) 

#pragma warning ( disable: 4244 ) 
#pragma warning ( disable: 4996 ) 

bool isFiveM = false;

#define IsValidPtr(x) (x && !IsBadReadPtr(&x, sizeof(void*) && x != nullptr && x > nullptr))



#include "../includes/SimpleMath.h"
using namespace DirectX::SimpleMath;
using namespace std;

#pragma endregion


#pragma pack(push, 1)
struct PVector3
{
public:
	PVector3() = default;

	PVector3(float x, float y, float z) :
		x(x), y(y), z(z) {
	}
public:
	float x{};
private:
	char m_padding1[0x04]{};
public:
	float y{};
private:
	char m_padding2[0x04]{};
public:
	float z{};
private:
	char m_padding3[0x04]{};
};

#pragma pack(pop)

enum Bones {
	HEAD = 0x796E,
	NECK = 0x9995,
	RIGHT_HAND = 0xDEAD,
	RIGHT_FOREARM = 0x6E5C,
	RIGHT_UPPER_ARM = 0x9D4D,
	RIGHT_CLAVICLE = 0x29D2,

	LEFT_HAND = 0x49D9,
	LEFT_FOREARM = 0xEEEB,
	LEFT_UPPER_ARM = 0xB1C5,
	LEFT_CLAVICLE = 0xFCD9,

	PELVIS = 0x2E28,
	SPINE_ROOT = 0xE0FD,
	SPINE0 = 0x60F2,
	SPINE1 = 0x60F0,
	SPINE2 = 0x60F1,
	SPINE3 = 0x60F2,

	RIGHT_TOE = 0x512D,
	RIGHT_FOOT = 0xCC4D,
	RIGHT_CALF = 0x9000,
	RIGHT_THIGH = 0xCA72,
	LEFT_TOE = 0x083C,
	LEFT_FOOT = 0x3779,
	LEFT_CALF = 0xF9BB,
	LEFT_THIGH = 0xE39F
};


//For Function bones
/*
//for internal bones

enum Bones {
	HEAD = 0,
	NECK = 7,
	CORE = 8,
	RIGHT_HAND = 6,
	LEFT_HAND = 5,

	LEFT_FOOT = 3,
	RIGHT_FOOT = 4,

	RIGHT_FRONT_FOOT = 2,
	LEFT_FRONT_FOOT = 1,
};*/




static std::wstring asciiDecode(const std::string& s) {
	std::wostringstream    w;
	wchar_t                c;

	for (size_t i = 0; i < s.length(); i++) {
		mbtowc(&c, &s[i], 1);
		w << c;
	}
	return w.str();
}


/*
* 
* for 1.61
* 
* 
		class scrThread
		{
		public:
			virtual ~scrThread() = default;                                                                 // 0 (0x00)
			virtual void reset(std::uint32_t script_hash, void *args, std::uint32_t arg_count) = 0;         // 1 (0x08)
			virtual eThreadState run() = 0;                                                                 // 2 (0x10)
			virtual eThreadState tick(std::uint32_t ops_to_execute) = 0;                                    // 3 (0x18)
			virtual void kill() = 0;                                                                        // 4 (0x20)

			static scrThread* get()
			{
				return rage::tlsContext::get()->m_script_thread;
			}
		public:
			scrThreadContext m_context;                 // 0x08
			void *m_stack;                              // 0xB0
			char m_padding[0x18];                       // 0xB8
			const char *m_exit_message;                 // 0xD0
			char m_name[0x40];                          // 0xD8
			scriptHandler *m_handler;                   // 0x118
			scriptHandlerNetComponent *m_net_component; // 0x120
		};

		static_assert(sizeof(scrThreadContext) == 0xA8);
		static_assert(sizeof(scrThread) == 0x128);





	namespace rage
	{
		class tlsContext
		{
		public:
			char m_padding1[0xC8]; // 0x00
			sysMemAllocator* m_allocator; // 0xC8
			char m_padding2[0x770]; // 0xD0
			scrThread* m_script_thread; // 0x840
			bool m_is_script_thread_active; // 0x848

			static tlsContext* get()
			{
				return *reinterpret_cast<tlsContext**>(__readgsqword(0x58));
			}
		};

		static_assert(sizeof(tlsContext) == 0x850);
	}


*/

enum class thread_state_t: std::uint32_t
{
	idle,
	running,
	killed,
	unk_3,
	unk_4,
};


class thread_context_t
{
public:
	std::uint32_t m_thread_id;
	std::uint32_t m_script_hash; // + 4 (program id)
	thread_state_t m_state; // + 8
	std::uint32_t m_instruction_pointer; // 0x0C
	std::uint32_t m_frame_pointer;       // 0x10
	std::uint32_t m_stack_pointer;       // 0x14
	float m_timer_a;                     // 0x18
	float m_timer_b;                     // 0x1C
	float m_timer_c;                     // 0x20
	char m_padding1[0x2C];               // 0x24
	std::uint32_t m_stack_size;          // 0x50
	char m_padding2[0x54];               // 0x54
};

//class bullet_trace {
//public:
//	std::uint64_t m_tick_count;
//	Vector3 m_shoot_coord;
//	Vector3 m_bullet_impact;
//};

class game_thread
{
public:
	virtual ~game_thread() = default;
	virtual thread_state_t reset(std::uint32_t script_hash, void* args, std::uint32_t arg_count) = 0;
	virtual thread_state_t run() = 0;
	virtual thread_state_t tick() = 0;
	virtual void kill() = 0;

	inline static game_thread*& get() {
		//return *(game_thread**)(*reinterpret_cast<uintptr_t*>(__readgsqword(0x58)) + 0x830);
		return *(game_thread**)(*reinterpret_cast<uintptr_t*>(__readgsqword(0x58)) + 0x830);//new
	}
public:
	thread_context_t m_context; // 0x08
	void* m_stack;              // 0xB0
	char m_padding[0x10];       // 0xB8
	const char* m_exit_message; // 0xC8
	char m_name[0x40];          // 0xD0
	void* m_handler;   // 0x110
	char m_padding2[0x28];      // 0x118
	std::uint8_t m_flag1;       // 0x140
	std::uint8_t m_flag2;       // 0x141
	char m_padding3[0x16];      // 0x142
};

enum game_state_t
{
	playing,
	intro,
	unk,
	license,
	main_menu,
	load_online
};

/*shot stuff*/
typedef __int64(__fastcall* gta_event_gun_shot_t)(
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
	char a12
	);
gta_event_gun_shot_t gta_event_gun_shot = nullptr;
gta_event_gun_shot_t original_gta_event_gun_shot = nullptr;


/*thread stuff*/
using script_thread_tick_t = uintptr_t(*)(game_thread* thread, int ops_to_execute);
//get_bone_position_t ptr_gta_get_bone_position = nullptr;
script_thread_tick_t gta_script_thread_tick = nullptr;
script_thread_tick_t original_native_thread = nullptr;

using get_native_handler_t = void* (*)(uintptr_t* table, uint64_t hash);
get_native_handler_t gta_get_native_handler;

using ragemp_fetch_handler_t = uintptr_t(*)(uintptr_t hash);
ragemp_fetch_handler_t ragemp_fetch_handler;
ragemp_fetch_handler_t orig_ragemp_fetch_handler;


uintptr_t* gta_native_registration_table = 0;

using fix_context_vector_t = void(*)(void* context);
fix_context_vector_t gta_fix_context_vector;

//

struct CVector3 {
public:
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
	};
};

#pragma pack(push, 1)
struct screenReso {
	uint32_t w, h;
};


class CViewPort {
public:
	char _0x0000[0x24C];
	float fViewMatrix[0x10];
	//D3DXMATRIX fViewMatrix;
};//Size=0x028C
#pragma pack(pop)
static CViewPort* m_viewPort;


class GameViewMatrices;
class CPlayerInfo;
class CPlayers;
class CInventory;
class CWeaponManager;
class CObjectWrapper;
class CObject;
class CVehicleManager;
class CAttacker;
class CVehicleHandling;
class CVehicleColorOptions;
class CVehicleColors;
class CBoneManager;

class CObjectNavigation {
public:
	char pad_0[0x20];
	Vector4 Rotation;
	char pad_1[0x20];
	Vector3 Position;

	void setPosition(Vector3 p) {
		*(float*)(((DWORD64)this + 0x50)) = p.x;
		*(float*)(((DWORD64)this + 0x54)) = p.y;
		*(float*)(((DWORD64)this + 0x58)) = p.z;
	}
	void setRotation(Vector3 p) {
		*(float*)(((DWORD64)this + 0x30)) = p.x;
		*(float*)(((DWORD64)this + 0x34)) = p.y;
		*(float*)(((DWORD64)this + 0x38)) = p.z;
	}
};
/*class CObjectNavigation {
public:
char pad_0x0000[0x20]; //0x0000
float fHeading; //0x0020
float fHeading2; //0x0024
char pad_0x0028[0x8]; //0x0028
Vector3 v3Rotation; //0x0030
char pad_0x003C[0x14]; //0x003C
Vector3 v3Pos; //0x0050
}; //Size=0x005C
*/
class CEventGunShot
{
public:
	char pad_0x0000[0x40]; //0x0000
	Vector3 shoot_coord; //0x0040 
	char pad_0x004C[0x4]; //0x004C
	Vector3 bullet_impact; //0x0050 
	char pad_0x005C[0xC]; //0x005C
	__int32 weapon_hash; //0x0068 
	char pad_0x006C[0x8]; //0x006C

}; //Size=0x0074

class CReplayInterfacePed {
private:
	class CPedList {
	private:
		struct Ped {
			CObject* ped;
			__int32 iHandle; //0x0008 
			char _pad0[0x4];
		};

	public:
		Ped peds[256];
	};

public:
	char _pad0[0x100];			// 0000 - 0100
	CPedList* ped_list;			// 0100 - 0108
	int max_peds;				// 0108 - 010C
	char _pad1[0x4];			// 010C - 0110
	int number_of_peds;		// 0110 - 0114

	CObject* get_ped(const unsigned int& index) {
		return (index <= max_peds && IsValidPtr(ped_list->peds[index].ped) ? ped_list->peds[index].ped : nullptr);
	}
};

class CReplayInterfaceVehicle {
private:
	class VehicleHandle {
	public:
		CVehicleManager* vehicle; //0x0000 
		__int32 handle; //0x0008
		char _pad0[0x4];
	};//Size=0x0588

	class CVehicleList {
	public:
		VehicleHandle vehicles[0x100];//0x0000 
	};//Size=0x0490

public:
	char _0x0000[0x180];
	CVehicleList* vehicle_list; //0x0180
	__int32 max_vehicles; //0x0188 
	char _0x010C[0x4];
	__int32 number_of_vehicles; //0x0190 
	char _0x0114[0x34];

	CVehicleManager* get_vehicle(const unsigned int& index) {
		return (index <= max_vehicles && IsValidPtr(vehicle_list->vehicles[index].vehicle) ? vehicle_list->vehicles[index].vehicle : nullptr);
	}
};

class CPickup {
public:
	char pad_0x0000[0x30]; //0x0000
	CObjectNavigation* pCNavigation; //0x0030 
	char pad_0x0038[0x58]; //0x0038
	Vector3 v3VisualPos; //0x0090 
	char pad_0x009C[0x3F4]; //0x009C
	__int32 iValue; //0x0490 
	char pad_0x0494[0xC4]; //0x0494

}; //Size=0x0558

class CPickupHandle {
public:
	CPickup* pCPickup; //0x0000 
	__int32 iHandle; //0x0008 
	char pad_0x000C[0x4]; //0x000C

}; //Size=0x0010

class CPickupList {
public:
	CPickupHandle pickups[73]; //0x0000 

}; //Size=0x0490

class CPickupInterface {
public:
	char pad_0x0000[0x100]; //0x0000
	CPickupList* pCPickupList; //0x0100 
	__int32 iMaxPickups; //0x0108 
	char pad_0x010C[0x4]; //0x010C
	__int32 iCurPickups; //0x0110 

	CPickup* get_pickup(const int& index) {
		if (index < iMaxPickups)
			return pCPickupList->pickups[index].pCPickup;
		return nullptr;
	}
}; //Size=0x0114

class CObjectInterface {
private:
	class CObjectHandle {
	public:
		CObject* pCObject; //0x0000 
		__int32 iHandle; //0x0008 
		char pad_0x000C[0x4]; //0x000C

	}; //Size=0x0010

	class CObjectList {
	public:
		CObjectHandle ObjectList[2300]; //0x0000 

	}; //Size=0x8FC0

public:
	char pad_0x0000[0x158]; //0x0000
	CObjectList* pCObjectList; //0x0158 
	__int32 iMaxObjects; //0x0160 
	char pad_0x0164[0x4]; //0x0164
	__int32 iCurObjects; //0x0168 
	char pad_0x016C[0x5C]; //0x016C

	CObject* get_object(const unsigned int& index) {
		return (IsValidPtr(pCObjectList) && index < iMaxObjects&& IsValidPtr(pCObjectList->ObjectList[index].pCObject) ? pCObjectList->ObjectList[index].pCObject : nullptr);
	}
	bool Remove_object(const unsigned int& index) {
		if (IsValidPtr(pCObjectList) && index < iMaxObjects && IsValidPtr(pCObjectList->ObjectList[index].pCObject) ? pCObjectList->ObjectList[index].pCObject : nullptr) {
			pCObjectList->ObjectList[index].iHandle = 0;
		}
		return true;
	}
	__int32 getHandle(const unsigned int& index) {
		if (IsValidPtr(pCObjectList) && index < iMaxObjects && IsValidPtr(pCObjectList->ObjectList[index].pCObject) ? pCObjectList->ObjectList[index].pCObject : nullptr) {
			return pCObjectList->ObjectList[index].iHandle;
		}
		return NULL;
	}

}; //Size=0x01C8

class CReplayInterface {
public:
	void* game_interface;
	void* camera_interface;
	CReplayInterfaceVehicle* vehicle_interface;
	CReplayInterfacePed* ped_interface;
	CPickupInterface* pickup_interface;
	CObjectInterface* object_interface;
};

class CModelInfo {
public:
	char pad_0x0000[0x270]; //0x0000
	BYTE Type; //0x0270 
	char pad_0x0271[0xF]; //0x0271

	DWORD GetHash() {
		return (*(DWORD*)((DWORD64)this + 0x18));
	}

	char* GetName() {
		return (*reinterpret_cast<char**>((DWORD64)this + 0x1C8));
	}


}; //Size=0x0280


class CVehicleManager {
public:
	char pad_0x0000[0x30]; //0x0000
	CObjectNavigation* _ObjectNavigation; //0x0030 
	char pad_0x0038[0x10]; //0x0038
	CVehicleColorOptions* CVehicleColorOptions; //0x0048 
	char pad_0x0050[0x40]; //0x0050
	Vector3 fPosition; //0x0090 
	char pad_0x009C[0xED]; //0x009C
	BYTE btGodMode; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float fHealth; //0x0280 
	char pad_0x0284[0x1C]; //0x0284
	float fHealthMax; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* CAttacker; //0x02A8 
	char pad_0x02B0[0x59C]; //0x02B0
	float fHealth2; //0x084C 
	char pad_0x0850[0x28]; //0x0850
	CVehicleHandling* CVehicleHandling; //0x0878 
	char pad_0x0880[0x3]; //0x0880
	BYTE btBulletproofTires; //0x0883 
	char pad_0x0884[0x2F8]; //0x0884
	float fGravity; //0x0B7C 


	CModelInfo* ModelInfo() {
		return (*(CModelInfo**)((DWORD64)this + 0x20));
	}

	bool isGod() {
		return (btGodMode & 0x01) ? true : false;
	}

	bool hasBulletproofTires() {
		return (btBulletproofTires & 0x20) ? true : false;
	}


	float getGravity() {
		return *(float*)this + 0xC5C;
	}
	void setGravity(float v) {
		*(float*)(this + 0xC5C) = v;
	}







}; //Size=0x0B80


class CVehicleModelInfoArray {};
class CNavigation {};
class CVehicleDrawHandler {};
class CMatrix {};
class CHandlingData {};
class CPed1 {};
class CAttacker1 {};
//1032
class CVehicle_1032 {
public:
	char pad_0x0000[0x20];                                  // 0x0000
	CVehicleModelInfoArray* VehicleModelInfoArrayContainer; // 0x0020
	unsigned char typeByte;                                 // 0x0028
	char pad_0x0029[0x7];                                   // 0x0029
	CNavigation* CNavigation;                               // 0x0030
	char pad_0x0038[0x10];                                  // 0x0038
	CVehicleDrawHandler* CVehicleColorOptions;              // 0x0048
	char pad_0x0050[0x10];                                  // 0x0050
	CMatrix N000072BA;                                      // 0x0060
	char pad_0x00A0[0x20];                                  // 0x00A0
	DWORD dwBitmapc0;                                       // 0x00C0 1<<17=onfire
	char pad_0x00C4[0x14];                                  // 0x00C4
	__int8 N000072C5;                                       // 0x00D8 & 0x02 == 2(not in car), 0 (in car)
	char pad_0x00D9[0xB0];                                  // 0x00D9
	unsigned char btGodMode;                                // 0x0189
	char pad_0x018A[0xF6];                                  // 0x018A
	float fHealth;                                          // 0x0280
	char pad_0x0284[0x1C];                                  // 0x0284
	float fHealthMax;                                       // 0x02A0
	char pad_0x02A4[0x4];                                   // 0x02A4
	CAttacker1* CAttacker;                                   // 0x02A8
	char pad_0x02B0[0x68];                                  // 0x02B0
	__int32 bRocketBoostEnabled;                            // 0x0318
	float fRocketBoostCharge;                               // 0x031C
	char pad_0x0320[0x440];                                 // 0x0320
	Vector3 v3Velocity;                                     // 0x0760
	float N00002422;                                        // 0x076C
	float fRotationSpeedRoll;                               // 0x0770
	float fRotationSpeedPitch;                              // 0x0774
	float fRotationSpeedYaw;                                // 0x0778
	float N00006E17;                                        // 0x077C
	char pad_0x0780[0x30];                                  // 0x0780
	float fUnknownHealth;                                   // 0x07B0 explodes when damage (not fire) decreases to 0
	float fFuelTankHealth;                                  // 0x07B4 big bang when reaches -1000
	float fFuelLevel;                                       // 0x07B8
	float fFireTimer;                                       // 0x07BC
	char pad_0x07C0[0x30];                                  // 0x07C0
	__int8 iGearNext;                                       // 0x07F0
	char pad_0x07F1[0x1];                                   // 0x07F1
	__int8 iGearCurrent;                                    // 0x07F2
	char pad_0x07F3[0x3];                                   // 0x07F3
	__int8 iGearTopGear;                                    // 0x07F6
	char pad_0x07F7[0x2D];                                  // 0x07F7
	float fCurrentRPM;                                      // 0x0824
	float fCurrentRPM2;                                     // 0x0828
	char pad_0x082C[0x4];                                   // 0x082C
	float fClutch;                                          // 0x0830
	float fThrottle;                                        // 0x0834
	char pad_0x0838[0x8];                                   // 0x0838
	__int32 UnknownFC40CBF7B90CA77C;                        // 0x0840 signed
	char pad_0x0844[0x4];                                   // 0x0844
	float fTurbo;                                           // 0x0848
	float fSteeringAngleZERO;                               // 0x084C
	char pad_0x0850[0x8];                                   // 0x0850
	float fVehicleCrashDowndown;                            // 0x0858
	float fHealth2;                                         // 0x085C
	char pad_0x0860[0x28];                                  // 0x0860
	CHandlingData* vehicleHandling;                         // 0x0888
	unsigned char vehicleType;                              // 0x0890 2=car,boat 6=heli,plane (0x04=flying + 0x02=vehicle)
	unsigned char dw4or5;                                   // 0x0891 always 4 then 5 in plane
	unsigned char bitmapCarInfo1;                           // 0x0892 3=in, 6=getting in, 2=out, 1=dead car; 2=in heli; 4=props on plane turning. (& 0x02 = drivable)
	unsigned char bitmapBulletProofTires;                   // 0x0893 wheelsCan'tBreak << 6
	char pad_0x0894[0x2];                                   // 0x0894
	unsigned char bitmapUnknown2;                           // 0x0896 << 4 (|= 16) by vehicle_6ebfb22d646ffc18
	char pad_0x0897[0x3];                                   // 0x0897
	unsigned char bitmapJumpRelated;                        // 0x089A & 0x10 = something
	char pad_0x089B[0x1];                                   // 0x089B
	__int8 bitmapOwnedByPlayer;                             // 0x089C 0x02 = hasBeenOwnedByPlayer
	char pad_0x089D[0xE];                                   // 0x089D
	unsigned char bitmapUnknownVehicleType1;                // 0x08AB VEHICLE__CAC66558B944DA67 &= 0xf7; |= 8*(enabled&1)
	char pad_0x08AC[0x4C];                                  // 0x08AC
	float N00002483;                                        // 0x08F8
	float fWheelBias2;                                      // 0x08FC -1.0 to 1.0
	char pad_0x0900[0x4];                                   // 0x0900
	float fWheelBias;                                       // 0x0904 -0.65~ to +~0.65
	char pad_0x0908[0x4];                                   // 0x0908
	float fThrottlePosition;                                // 0x090C
	float fBreakPosition;                                   // 0x0910
	unsigned char bHandbrake;                               // 0x0914
	char pad_0x0915[0x33];                                  // 0x0915
	float fDirtLevel;                                       // 0x0948
	char pad_0x094C[0x70];                                  // 0x094C
	float fEngineTemp;                                      // 0x09BC
	char pad_0x09C0[0x34];                                  // 0x09C0
	DWORD dwCarAlarmLength;                                 // 0x09F4
	char pad_0x09F8[0x8];                                   // 0x09F8
	float fDashSpeed;                                       // 0x0A00
	char pad_0x0A04[0x6];                                   // 0x0A04
	unsigned char bUsedInB2E0C0D6922D31F2;                  // 0x0A0A
	char pad_0x0A0B[0x45];                                  // 0x0A0B
	Vector3 vTravel;                                        // 0x0A50
	char pad_0x0A5C[0x44];                                  // 0x0A5C
	float fVelocityX;                                       // 0x0AA0
	float fVelocityY;                                       // 0x0AA4
	float fVelocityZ;                                       // 0x0AA8
	char pad_0x0AAC[0x6C];                                  // 0x0AAC
	__int32 iVehicleType;                                   // 0x0B18 0=car recently;1=plane, 0d = boat, 8=heli, 0x0e=train, 0x0b=motorbike, <= 0x0a = CAutomobile?
	char pad_0x0B1C[0x24];                                  // 0x0B1C
	unsigned char btOpenableDoors;                          // 0x0B40
	char pad_0x0B41[0x4B];                                  // 0x0B41
	float fGravity;                                         // 0x0B8C
	char pad_0x0B90[0x8];                                   // 0x0B90
	CPed1 PedsInSeats[15];                                   // 0x0B98
	char pad_0x0C10[0x798];                                 // 0x0C10
	unsigned char ReducedGrip;                              // 0x13A8 &=0xBF; |= (reduceGrip & 1) << 6
	unsigned char CreatesMoneyWhenExploded;                 // 0x13A9 &= 0xFD; |= 2 * enabled
	char pad_0x13AA[0x3F6];                                 // 0x13AA
	float fSudoJumpValue;                                   // 0x17A0
	char pad_0x17A4[0x130];                                 // 0x17A4

															//-Wiretrick :
															//(byte)0x92C : 4 (wiretrick animation), 2 (key are present)
															//	-Door locked :
															//(byte)0x1370 : 7 (lock), 1 (unlock)
};


class CVehicle {
public:
	char pad_0x0000[0x30]; //0x0000
	CObjectNavigation* ObjectNavigation; //0x0030  CNavigation
	char pad_0x0038[0x10]; //0x0038
	void* pCVehicleMods; //0x0048  CVehicleMods*
	char pad_0x0050[0x40]; //0x0050
	Vector3 fPosition; //0x0090 
	char pad_0x009C[0x3C]; //0x009C
	BYTE btState; //0x00D8 
	char pad_0x00D9[0xB0]; //0x00D9
	BYTE btGodMode; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float fHealth; //0x0280 
	//char pad_0x0284[0x1C]; //0x0284
	float fHealthMax; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* pCAttacker; //0x02A8 
	char pad_0x02B0[0x6A]; //0x02B0
	BYTE btVolticRocketState; //0x031A 
	char pad_0x031B[0x1]; //0x031B
	float fVolticRocketEnergy; //0x031C 
	char pad_0x0320[0x430]; //0x0320
	Vector3 v3Velocity; //0x0750 
	char pad_0x075C[0xF0]; //0x075C
	float fHealth2; //0x084C 
	char pad_0x0850[0x28]; //0x0850
	CVehicleHandling* pVehicleHandling; //0x0878 
	char pad_0x0880[0x3]; //0x0880
	BYTE btBulletproofTires; //0x0883 
	char pad_0x0884[0x4]; //0x0884
	BYTE btStolen; //0x0888 
	char pad_0x0889[0x11]; //0x0889
	BYTE N00000954; //0x089A 
	char pad_0x089B[0x41]; //0x089B
	float N0000081E; //0x08DC 
	char pad_0x08E0[0x58]; //0x08E0
	float fDirtLevel; //0x0938			max = 15.f
	char pad_0x093C[0xA8]; //0x093C
	DWORD dwCarAlarmLength; //0x09E4 
	char pad_0x09E8[0x148]; //0x09E8
	BYTE btOpenableDoors; //0x0B30 
	char pad_0x0B31[0x4B]; //0x0B31
	float fGravity; //0x0B7C
	BYTE btMaxPassengers; //0x0B80 
	char pad_0x0B81[0x1]; //0x0B81
	BYTE btNumOfPassengers; //0x0B82 
	char pad_0x0B83[0x3D]; //0x0B83

	bool isGod() {
		return (btGodMode & 0x01) ? true : false;
	}

	bool hasBulletproofTires() {
		return (btBulletproofTires & 0x20) ? true : false;
	}

	bool disabledDoors() {
		return (btOpenableDoors == 0) ? true : false;
	}

	void openDoors() {
		btOpenableDoors = 1;
	}




	void giveHealth() {
		if (fHealth < fHealthMax)
			fHealth = fHealthMax;
		if (fHealth2 < fHealthMax)
			fHealth2 = fHealthMax;
	}

	//btState & 7    0 = inside of vehicle, 2 = outside of vehicle, 3 = vehicle blown up
	bool isVehicleDestroyed() {
		return (btState & 7) == 3;
	}

	void set_stolen(bool toggle) {
		this->btStolen &= 0xFEu;
		this->btStolen |= toggle & 1;
	}



	CModelInfo* ModelInfo() {
		return (*(CModelInfo**)((DWORD64)this + 0x20));
	}

	void setUnlocked(BYTE n) {
		*(BYTE*)((DWORD64)this + 0x1370) = n;
		//*(BYTE*)((DWORD64)this + 0x1390) = n;
	}

	void setEngine(BYTE n) {
		*(BYTE*)((DWORD64)this + 0x92A) = n;
		//*(BYTE*)((DWORD64)this + 0x94A) = n;
	}

	float getGravity() {
		return *(float*)((DWORD64)this + 0x0C3C); //new
		//return *(float*)((DWORD64)this + (0xC5C - 0x2));

	}
	void setGravity(float v) {
		*(float*)((DWORD64)this + 0x0C3C) = v; //new
		//*(float*)((DWORD64)this + (0xC5C)) = v;
	}

	CVehicleHandling* handling() {
		return *(CVehicleHandling**)((uintptr_t)this + 0x0918); //new
		//return *(CVehicleHandling**)((uintptr_t)this + 0x938);
	}

}; //Size=0x0BC0




class CWeapon {
public:
	char pad_0x0000[0x10]; //0x0000
	DWORD dwNameHash; //0x0010 
	DWORD dwModelHash; //0x0014 
	char pad_0x0018[0x24]; //0x0018
	DWORD dwAmmoType; //0x003C 
	DWORD dwWeaponWheelSlot; //0x0040 
	char pad_0x0044[0x4]; //0x0044
	void* CAmmoInfo; //0x0048 
	char pad_0x0050[0xC]; //0x0050
	float fSpread; //0x005C 
	char pad_0x0060[0x38]; //0x0060
	float fBulletDamage; //0x0098 
	char pad_0x009C[0x1C]; //0x009C
	float fForce; //0x00B8 
	float fForcePed; //0x00BC 
	float fForceVehicle; //0x00C0 
	float fForceFlying; //0x00C4 
	char pad_0x00C8[0x34]; //0x00C8
	float fMuzzleVelocity; //0x00FC 
	DWORD dwBulletInBatch; //0x0100 
	float fBatchSpread; //0x0104 
	char pad_0x0108[0x8]; //0x0108
	float fVehicleReloadTime; //0x0110 
	float fAnimReloadRate; //0x0114 
	char pad_0x0118[0xC]; //0x0118
	float fSpinUpTime; //0x0124 
	float fSpinTime; //0x0128 
	char pad_0x012C[0x12C]; //0x012C
	float fLockOnRange; //0x0258 
	float fWeaponRange; //0x025C 
	char pad_0x0260[0x44]; //0x0260
	DWORD dwRecoil; //0x02A4 
	DWORD dwRecoilFP; //0x02A8 
}; //Size=0x02A8







   /*
   class CWeaponInfo {
   public:
   char pad_0000[96]; //0x0000
   void* _AmmoInfo; //0x0060 //70
   char pad_0068[12]; //0x0068
   float Spread; //0x0074 //84
   char pad_0078[56]; //0x0078
   float Damage; //0x00B0
   char pad_00B4[36]; //0x00B4
   float MinImpulse; //0x00D8
   float MaxImpulse; //0x00DC
   char pad_00E0[52]; //0x00E0
   float Velocity; //0x0114
   int32_t BulitsPerShoot; //0x0118
   float MultiBulitSpread; //0x011C
   char pad_0120[12]; //0x0120
   float ReloadTimeMultiplier; //0x012C
   char pad_0130[4]; //0x0130
   float TimeToShoot; //0x0134 //144
   char pad_0138[404]; //0x0138
   //float RecoilFirstPerson; //0x02CC
   char pad_02D0[8]; //0x02D0
   float RecoilThirdPerson; //0x02D8 //2E8
   char pad_02DC[772]; //0x02DC
   char* szWeaponName; //0x05E0 //5F0
   char pad_05E8[40]; //0x05E8

   void Range(float value) {
   *(float*)( (DWORD64)this + 0x28C ) = value; //MinRange
   *(float*)( (DWORD64)this + 0x29C ) = value; //MaxRange
   }
   float getRange() {
   return *(float*)( (DWORD64)this + 0x29C );
   }
   void setRecoil(float value) {
   *(float*)( (DWORD64)this + 0x2E8 ) = value;
   }
   float getRecoil() {
   return *(float*)( (DWORD64)this + 0x2E8 );
   }
   void setSpread(float value) {
   *(float*)( (DWORD64)this + 0x84 ) = value;//0.0f; //Spread
   //*(float*)( (DWORD64)this + 0x11C ) = 0.0f; //Multi Bulit spread
   }
   float getSpread() {
   return *(float*)( (DWORD64)this + 0x84 );
   }
   void NoReload() {
   *(float*)( (DWORD64)this + 0x12C ) = 22.0f;
   }
   void SuperImpulse() {
   *(float*)( (DWORD64)this + 0xD8 ) = 9999.0f;
   *(float*)( (DWORD64)this + 0xDC ) = 9999.0f;
   }
   void SuperDmg() {
   *(float*)( (DWORD64)this + 0xB0 ) = 9999.0f;
   }
   char *GetSzWeaponName() {
   return ( *reinterpret_cast<char **>( (DWORD64)this + 0x5F0 ) );
   }

   //0x1D4 = Visual shoot animation radius
   }*/ //Size: 0x0610
class CWeaponInfo {
public:
	char pad_0000[96]; //0x0000
	void* _AmmoInfo; //0x0060 //70
	char pad_0068[12]; //0x0068
	float Spread; //0x0074 //84
	char pad_0078[56]; //0x0078
	float Damage; //0x00B0 
	char pad_00B4[36]; //0x00B4
	float MinImpulse; //0x00D8 
	float MaxImpulse; //0x00DC
	char pad_00E0[52]; //0x00E0
	float Velocity; //0x0114 
	int32_t BulitsPerShoot; //0x0118 
	float MultiBulitSpread; //0x011C 
	char pad_0120[12]; //0x0120
	float ReloadTimeMultiplier; //0x012C
	char pad_0130[4]; //0x0130
	float TimeToShoot; //0x0134 //144
	char pad_0138[404]; //0x0138
						//float RecoilFirstPerson; //0x02CC 
	char pad_02D0[8]; //0x02D0
	float RecoilThirdPerson; //0x02D8 //2E8
	char pad_02DC[772]; //0x02DC
	char* szWeaponName; //0x05E0 //5F0
	char pad_05E8[40]; //0x05E8

	void Range(float value) {
		*(float*)((DWORD64)this + 0x28C) = value; //MinRange
		*(float*)((DWORD64)this + 0x29C) = value; //MaxRange
	}
	float getRange() {
		return *(float*)((DWORD64)this + 0x29C);
	}
	void setRecoil(float value) {
		if (isFiveM) {
			*(float*)((DWORD64)this + 0x2E8) = value;
			return;
		}
		//*(float*)((DWORD64)this + 0x2E8) = value;
		*(float*)((DWORD64)this + 0x2F4) = value;
	}
	float getRecoil() {
		return *(float*)((DWORD64)this + 0x2F4);
	}
	void setSpread(float value) {
		*(float*)((DWORD64)this + 0x84) = value;//0.0f; //Spread
												//*(float*)( (DWORD64)this + 0x11C ) = 0.0f; //Multi Bulit spread
	}
	float getSpread() {
		return *(float*)((DWORD64)this + 0x84);
	}
	void setTimeToShoot(float value) {
		*(float*)((DWORD64)this + 0x13C) = value;//0.0f; //Spread
												 //*(float*)( (DWORD64)this + 0x11C ) = 0.0f; //Multi Bulit spread
	}
	float getTimeToShoot() {
		return *(float*)((DWORD64)this + 0x13C);
	}
	void SuperImpulse() {
		*(float*)((DWORD64)this + 0xD8) = 9999.0f;
		*(float*)((DWORD64)this + 0xDC) = 9999.0f;
	}
	void setDamage(float dmg) {
		*(float*)((DWORD64)this + 0xB0) = dmg;
	}

	void setReloadSpeed(float speed) {
		*(float*)((DWORD64)this + 0x134) = speed;
		*(float*)((DWORD64)this + 0x130) = speed;
	}

	char* GetSzWeaponName() {
		return (*reinterpret_cast<char**>((DWORD64)this + 0x5F0));
	}

	//0x1D4 = Visual shoot animation radius
}; //Size: 0x0610
static CWeaponInfo* m_pCWeaponInfo = nullptr;
static CWeaponInfo m_CWeapon;

//class CWeaponManager
//{
//public:
//	char pad_0000[32]; //0x0000
//	CWeaponInfo* _WeaponInfo; //0x0020
//	char pad_0028[96]; //0x0028
//	void* _CurrentWeapon; //0x0088
//	char pad_0090[272]; //0x0090
//	Vector3 Impact; //0x01A0
//	char pad_01AC[24]; //0x01AC
//
//	Vector3 GetImpact2() {
//		return (Vector3)(*(Vector3 *)((DWORD64)this + 0x1B0));
//	}
//}; //Size: 0x01C4


class CWeaponManager {
public:
	char pad_0000[32]; //0x0000
	CWeaponInfo* _WeaponInfo; //0x0020w
	char pad_0028[96]; //0x0028
	void* _CurrentWeapon; //0x0088
	char pad_0090[272]; //0x0090
	Vector3 Aiming_at_Cord; //0x01A0
	char pad_01AC[4]; //0x01AC
	Vector3 Last_Impact_Cord; //0x01B0
	char pad_01BC[44]; //0x01BC
}; //Size: 0x01E8







class CPedStyle {
public:
	char _0x0000[48];
	void* N4100AE4A; //0x0030 
	void* pColors; //0x0038 
	char _0x0040[168];
	BYTE propIndex[12]; //0x00E8 
	char _0x00F4[12];
	BYTE textureIndex[12]; //0x0100 
	BYTE paletteIndex[12]; //0x10C
	char _0x010C[9];
	BYTE N4100F70F; //0x0121 
	BYTE N4134BE09; //0x0122 
	BYTE N4134C676; //0x0123 
	BYTE btEars; //0x0124 
	char _0x0125[3];
	BYTE btEarsTexture; //0x0128 
	char _0x0129[72];
	BYTE N4100F719; //0x0171 
	BYTE N413811BF; //0x0172 
	BYTE N41381E8E; //0x0173 
	BYTE btHats; //0x0174 
	char _0x0175[3];
	BYTE btHatsTexture; //0x0178 
	BYTE N4100F71A; //0x0179 
	BYTE N4461C523; //0x017A 
	BYTE N4461D118; //0x017B 
	BYTE N4461C524; //0x017C 
	BYTE N445D0F2B; //0x017D 
	BYTE N445D279D; //0x017E 
	BYTE N445D3102; //0x017F 
	BYTE N445D279E; //0x0180 
	BYTE N4100F71B; //0x0181 
	BYTE N445D522D; //0x0182 
	BYTE N445D5D09; //0x0183 
	char _0x0184[61];
	BYTE N4100F723; //0x01C1 
	BYTE N4132EABE; //0x01C2 
	BYTE N4132F80B; //0x01C3 
	BYTE btGlasses; //0x01C4 
	char _0x01C5[3];
	BYTE btGlassesTexture; //0x01C8 
};//Size=0x03A8






Vector3 LastPosition[1028];

struct Groups {
	bool inGroup = false;
	bool ally = false;
	string name = "\0";

	RGBA color = RGBA(0, 0, 0, 255);
};


struct PlayerBones {
	Vector3 HEAD;
	Vector3 NECK;

	Vector3 RIGHT_HAND;
	Vector3 RIGHT_FOREARM;
	Vector3 RIGHT_UPPER_ARM;
	Vector3 RIGHT_CLAVICLE;

	Vector3 LEFT_HAND;
	Vector3 LEFT_FOREARM;
	Vector3 LEFT_UPPER_ARM;
	Vector3 LEFT_CLAVICLE;

	Vector3 PELVIS;
	Vector3 SPINE_ROOT;
	Vector3 SPINE0;
	Vector3 SPINE1;
	Vector3 SPINE2;
	Vector3 SPINE3;

	Vector3 RIGHT_TOE;
	Vector3	RIGHT_FOOT;
	Vector3	RIGHT_CALF;
	Vector3	RIGHT_THIGH;

	Vector3	LEFT_TOE;
	Vector3	LEFT_FOOT;
	Vector3	LEFT_CALF;
	Vector3	LEFT_THIGH;
};


struct DataPed {
	int visible;
	int index;

	PlayerBones bones;
	Groups group;
};

//extern HackBase Game;
class CObject {
public:
	char pad_0x0000[0x2C]; //0x0000
	BYTE btInvisibleSP; //0x002C 
	char pad_0x002D[0x1]; //0x002D
	BYTE btFreezeMomentum; //0x002E 
	char pad_0x002F[0x1]; //0x002F




	//CObjectNavigation* _ObjectNavigation; //0x0030 
	//char pad_0x0038[0x58]; //0x0038
	//Vector3 fPosition; //0x0090 

	CObjectNavigation* ObjectNavigation; //0x0030 
	char pax_0x0038[0x10]; //0x0038
	CPedStyle* pCPedStyle; //0x0048
	char pad_0x0038[0x40]; //0x0050
	Vector3 fPosition; //0x0090 

	char pad_0x009C[0xED]; //0x009C
	BYTE godmode; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float HP; //0x0280 
	//char pad_0x0284[0x1C]; //0x0284
	float MaxHP; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* CAttacker; //0x02A8 
	char pad_0x02B0[0x70]; //0x02B0
	Vector3 v3Velocity; //0x0320 
	char pad_0x032C[0x9FC]; //0x032C
	CVehicleManager* VehicleManager; //0x0D28 
	char pad_0x0D30[0x378]; //0x0D30
	BYTE btNoRagdoll; //0x10A8 
	char pad_0x10A9[0xF]; //0x10A9
	CPlayerInfo* PlayerInfo; //0x10B8 
	char pad_0x10C0[0x18]; //0x10C0
	CWeaponManager* WeaponManager; //0x10C8 change to 10D8 
	char pad_0x10D0[0x31C]; //0x10D0
	BYTE btSeatBelt; //0x13EC 
	char pad_0x13ED[0xB]; //0x13ED
	BYTE btSeatbeltWindshield; //0x13F8 
	char pad_0x13F9[0x72]; //0x13F9
	BYTE btIsInVehicle; //0x146B 
	char pad_0x146C[0x44]; //0x146C
	float Armor; //0x14B0 
	char pad_0x14B4[0x3C]; //0x14B4
	CVehicleManager* VehicleManager2; //0x14F0 

	void setRagdoll(BYTE val) {
		//*(BYTE*)((uintptr_t)this + 0x10B8) = val;
		*(BYTE*)((uintptr_t)this + 0x1098) = val; //new
	}
	void setCollision(float v) {
		uintptr_t* value = *(uintptr_t**)((uintptr_t)this + 0x30);
		if (!IsValidPtr(value)) logs::add("collision ptr1 invalid");
		uintptr_t* value1 = *(uintptr_t**)((uintptr_t)value + 0x10);
		if (!IsValidPtr(value1)) logs::add("collision ptr2 invalid");
		uintptr_t* value2 = *(uintptr_t**)((uintptr_t)value1 + 0x20);
		if (!IsValidPtr(value2)) logs::add("collision ptr3 invalid");
		uintptr_t* value3 = *(uintptr_t**)((uintptr_t)value2 + 0x70);
		if (!IsValidPtr(value3)) logs::add("collision ptr4 invalid");
		uintptr_t* value4 = *(uintptr_t**)((uintptr_t)value3 + 0x0);
		if (!IsValidPtr(value4)) logs::add("collision ptr5 invalid");

		//float value5 = *(float*)((uintptr_t)value4 + 0x2C);

		*(float*)((uintptr_t)value4 + 0x2C) = v;
	}
	//getAddress("[[[[[[[WorldPTR]+8]+30]+10]+20]+70]+0]+2C")
	float getCollision() {
		uintptr_t* value = *(uintptr_t**)((uintptr_t)this + 0x30);
		if (!IsValidPtr(value)) logs::add("collision ptr1 invalid");
		uintptr_t* value1 = *(uintptr_t**)((uintptr_t)value + 0x10);
		if (!IsValidPtr(value1)) logs::add("collision ptr2 invalid");
		uintptr_t* value2 = *(uintptr_t**)((uintptr_t)value1 + 0x20);
		if (!IsValidPtr(value2)) logs::add("collision ptr3 invalid");
		uintptr_t* value3 = *(uintptr_t**)((uintptr_t)value2 + 0x70);
		if (!IsValidPtr(value3)) logs::add("collision ptr4 invalid");
		uintptr_t* value4 = *(uintptr_t**)((uintptr_t)value3 + 0x0);
		if (!IsValidPtr(value4)) logs::add("collision ptr5 invalid");

		float value5 = *(float*)((uintptr_t)value4 + 0x2C);
		return value5;
	}
	CPlayerInfo* player_info() {
		//return *(CPlayerInfo**)((uintptr_t)this + 0x10C8);

		//return *(CPlayerInfo**)((uintptr_t)this + 0x10C8);
		return *(CPlayerInfo**)((uintptr_t)this + 0x10A8);//new
	}

	CWeaponManager* weapon() {
		//return *(CWeaponManager**)((uintptr_t)this + 0x10D8);
		return *(CWeaponManager**)((uintptr_t)this + 0x10B8); //new
	}

	CVehicle* vehicle() {
		//if(isFiveM) {
		//	return *(CVehicle**)((uintptr_t)this + 0xD28);
		//}
		//return *(CVehicle**)((uintptr_t)this + 0xD30);
		return *(CVehicle**)((uintptr_t)this + 0xD10); //new
		//return *(CVehicle**)((DWORD64)this + 0xD30);

	}

	Vector3 get_bone(int32_t bone_id) {
		//OUTDATED since 27.10.2022
		Vector3 ret;
		auto boneManager = *reinterpret_cast<__m128**>(reinterpret_cast<int64_t>(this) + 0x180);
		if (!boneManager) return ret;
		auto boneVector = *reinterpret_cast<__m128i*>(reinterpret_cast<int64_t>(this) + 0x430 + bone_id * 0x10);
		auto v1 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0)), *boneManager);
		auto v2 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0x55)), boneManager[1]);
		auto v3 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0xAA)), boneManager[2]);
		auto v4 = _mm_add_ps(_mm_add_ps(v1, boneManager[3]), v2);

		ret = Vector3(_mm_add_ps(v3, v4));

		return ret;
	}

	bool IsVisible() {
		return (*(BYTE*)((DWORD64)this + 0xAC) > 0 ? true : false);
		//return true;
	}
	void SetAlpha(float alpha) {
		(*(BYTE*)((DWORD64)this + 0xAC)) = alpha;
	}
	BYTE GetAlpha() {
		return (*(BYTE*)((DWORD64)this + 0xAC));
	}


	void freeze() {
		this->btFreezeMomentum = 1;

	}
	void ResetHealth() {
		this->HP = this->MaxHP;
		*(float*)((DWORD64)this + 0x14B8) = 100.0f;
	}


	float GetArmor() {
		//uintptr_t offset = (0x14E0 + 0x50);
		uintptr_t offset = 0x150C; //new
		//return (*(float*)((DWORD64)this + 0x14B8));
		return (*(float*)((DWORD64)this + offset));
	}
	void SetArmor(float v) {
		//uintptr_t offset = (0x14E0 + 0x50); // 0x50 
		uintptr_t offset = 0x150C;//new
		*(float*)((DWORD64)this + offset) = v;
	}

	bool is_god_active() {
		return (godmode & 0x01) ? true : false;
	}
	void set_godmode(bool toggle) {
		this->godmode = toggle ? 0x1 : 0x0;
	}

	bool isInvisSP() {
		return (btInvisibleSP & 0x01) ? true : false;
	}

	bool hasFrozenMomentum() {
		return (btFreezeMomentum & 0x02) ? true : false;
	}

	bool canBeRagdolled() {
		return (btNoRagdoll & 0x20) ? false : true;
	}


	WORD PedType() {
		return (*(WORD*)((DWORD64)this + 0x10A8) << 11 >> 25);
	}

	void SetPedType(int Type) {
		*(WORD*)(((DWORD64)this + 0x10A8) << 11 >> 25) = Type;
	}

	bool IsInVehicle() {
		//return (*(int*)((DWORD64)this + 0x1477)) == 0 ? true : false; //new
		return (*(int*)((DWORD64)this + (0x1477 + 0x30))) == 0 ? true : false; //new
		//return (*(int*)((DWORD64)this + (0x1477 + 0x50))) == 0 ? true : false;
		//return (*(int*)((DWORD64)this + 0x1477)) == 0 ? true : false;
		//return (*(BYTE*)((DWORD64)this + 0x1493)) == 0x02 ? true : false;
	}
	void setSeatbelt() {
		//*(BYTE*)((DWORD64)this + 0x140C) = (BYTE)56;

		*(int*)((DWORD64)this + (0x140C + 0x50)) = 0xC9;
	}


	CModelInfo* ModelInfo() {
		return (*(CModelInfo**)((DWORD64)this + 0x20));
	}



	/*
	float getSpeed() {
	float now = clock() * 0.001f;
	static Vector3 LastPosition = fPosition;
	static float LastCheck = clock() * 0.001f;

	float speed = Tools->Get3dDistance(fPosition, LastPosition) / ( now - LastCheck );

	LastCheck = now;

	return speed;
	}*/

	//void SetPosClass(Vector3 Pos) {
		//if (IsValidPtr(_ObjectNavigation))
		//	fPosition = Pos, _ObjectNavigation->WritePos(Pos);
	//}
	//CBoneManager *BoneManager() {
	//	return (*(CBoneManager**)((DWORD64)this + 0x180));
	//}

	//Vector3 HeadPos() {
	//	return (*(Vector3 *)((DWORD64)this + addToBone));
	//}

}; //Size=0x14F8



class CPed {
public:
	char pad_0x0000[0x28]; //0x0000
	BYTE btEntityType; //0x0028 
	char pad_0x0029[0x3]; //0x0029
	BYTE btInvisible; //0x002C 
	char pad_0x002D[0x1]; //0x002D
	BYTE btFreezeMomentum; //0x002E 
	char pad_0x002F[0x1]; //0x002F
	void* pCNavigation; //0x0030 
	char pax_0x0038[0x10]; //0x0038
	CPedStyle* pCPedStyle; //0x0048
	char pad_0x0038[0x40]; //0x0050
	Vector3 v3VisualPos; //0x0090 
	char pad_0x009C[0xED]; //0x009C
	BYTE btGodMode; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float fHealth; //0x0280 
	char pad_0x0284[0x1C]; //0x0284
	float fHealthMax; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* pCAttacker; //0x02A8 
	char pad_0x02B0[0x70]; //0x02B0
	Vector3 v3Velocity; //0x0320 
	char pad_0x032C[0x9FC]; //0x032C
	CVehicle* pCVehicleLast; //0x0D28 
	char pad_0x0D30[0x378]; //0x0D30
	BYTE btNoRagdoll; //0x10A8 
	char pad_0x10A9[0xF]; //0x10A9
	CPlayerInfo* pCPlayerInfo; //0x10B8 
	char pad_0x10C0[0x8]; //0x10C0
	CWeaponManager* pCWeaponManager; //0x10C8  // change to 1D8
	char pad_0x10D0[0x31C]; //0x10D0
	BYTE btSeatBelt; //0x13EC 
	char pad_0x13ED[0xB]; //0x13ED
	BYTE btSeatbeltWindshield; //0x13F8 
	char pad_0x13F9[0x1]; //0x13F9
	BYTE btCanSwitchWeapons;
	char pad_0x13FB[0x5];
	BYTE btForcedAim; //0x1400 
	BYTE N00000936; //0x1401 
	BYTE N00000939; //0x1402 
	BYTE N00000937; //0x1403 
	char pad_0x1404[0x67]; //0x1404
	BYTE btIsInVehicle; //0x146B 
	char pad_0x146C[0x44]; //0x146C
	float fArmor; //0x14B0 
	char pad_0x14B4[0x20]; //0x14B4
	float fFatiguedHealthThreshold; //0x14D4 
	float fInjuredHealthThreshold; //0x14D8 
	float fDyingHealthThreshold; //0x14DC 
	float fHurtHealthThreshold; //0x14E0 
	char pad_0x14E4[0xC]; //0x14E4
	CVehicle* pCVehicleLast2; //0x14F0 
	char pad_0x14F8[0xDC]; //0x14F8
	__int32 iCash; //0x15D4 


	bool isInvisible() {
		return (btInvisible & 0x01) ? true : false;
	}

	bool hasFrozenMomentum() {
		return (btFreezeMomentum & 0x02) ? true : false;
	}

	bool canBeRagdolled() {
		return (btNoRagdoll & 0x20) ? false : true;
	}

	bool hasSeatbelt() {
		return (btSeatBelt & 0x01) ? true : false;
	}

	bool hasSeatbeltWindshield() {
		return (btSeatbeltWindshield & 0x10) ? true : false;
	}

	bool isInVehicle() {
		return (btIsInVehicle & 0x10) ? false : true;
	}

	void setForcedAim(bool toggle) {
		btForcedAim ^= (btForcedAim ^ -(char)toggle) & 0x20;
	}
}; //Size=0x15D8



class CAttacker {
public:
	CObject* CPed0; //0x0000 
	char pad_0x0008[0x10]; //0x0008
	CObject* CPed1; //0x0018 
	char pad_0x0020[0x10]; //0x0020
	CObject* CPed2; //0x0030 

}; //Size=0x0038

class CWantedData {
public:
	char pad_0x0000[0x1C]; //0x0000
	float fWantedCanChange; //0x001C 
	char pad_0x0020[0x10]; //0x0020
	Vector3 v3WantedCenterPos; //0x0030 
	char pad_0x003C[0x4]; //0x003C
	Vector3 v3WantedCenterPos2; //0x0040 
	char pad_0x004C[0x38]; //0x004C
	BYTE btFlag0; //0x0084 
	BYTE btFlag1; //0x0085 
	BYTE btFlag2; //0x0086 
	BYTE btFlag3; //0x0087 
	char pad_0x0088[0xC]; //0x0088
	DWORD dwWantedLevelFake; //0x0094 
	DWORD dwWantedLevel; //0x0098 

}; //Size=0x00AC

class CPlayerInfo {
public:
	char pad_0x0000[0x34]; //0x0000
	int32_t iInternalIP; //0x0034 
	int16_t iInternalPort; //0x0038 
	char pad_0x003A[0x2]; //0x003A
	int32_t iRelayIP; //0x003C 
	int16_t iRelayPort; //0x0040 
	char pad_0x0042[0x2]; //0x0042
	int32_t iExternalIP; //0x0044 
	int16_t iExternalPort; //0x0048 
	char pad_0x004A[0x32]; //0x004A
	char sName[20]; //0x007C 
	char pad_0x0090[0x4]; //0x0090
	int32_t iTeam; //0x0094 
	char pad_0x0098[0xB0]; //0x0098
	float fSwimSpeed; //0x0148 
	float fRunSpeed; //0x014C 
	char pad_0x0150[0x68]; //0x0150
	uint32_t ulState; //0x01B8 
	char pad_0x01BC[0xC]; //0x01BC
	CObject* pCPed; //0x01C8 
	char pad_0x01D0[0x29]; //0x01D0
	BYTE btFrameFlags; //0x01F9 
	char pad_0x01FA[0x566]; //0x01FA
	CWantedData CWantedData; //0x0760 
	char pad_0x07FC[0x464]; //0x07FC
	float fStamina; //0x0C60 
	float fStaminaMax; //0x0C64 
	char pad_0x0C68[0x10]; //0x0C68
	float fDamageMod; //0x0C78 
	char pad_0x0C7C[0x4]; //0x0C7C
	float fMeleeDamageMod; //0x0C80 
	char pad_0x0C84[0x4]; //0x0C84
	float fVehicleDamageMod; //0x0C88 
	char pad_0x0C8C[0x1B4]; //0x0C8C
	BYTE btAiming; //0x0E40 
	char pad_0x0E41[0x7]; //0x0E41

	void SetWanted(int lvl) {
		*(__int32*)((DWORD64)this + 0x814) = lvl,
			* (__int32*)((DWORD64)this + 0x818) = lvl;
	}
	__int32 GetWanted() {
		return *(__int32*)((DWORD64)this + 0x814);
	}
	char GetName() {
		return *(char*)((DWORD64)this + 0x84);
	}
	int RockstarID() {
		return *(int*)((DWORD64)this + 0x70);
	}

	int getSprintSpoeed() {
		return *(int*)((DWORD64)this + 0x70);
	}

	void setSwimSpeed(float v) {
		*(float*)((uintptr_t)this + 0x150) = v;
	}
	void setWalkSpeed(float v) {
		*(float*)((uintptr_t)this + 0x16C) = v;
	}
	void setRunSpeed(float v) {
		if (isFiveM) {
			*(float*)((uintptr_t)this + 0x14C) = v;
			return;
		}
		*(float*)((uintptr_t)this + 0xCD0) = v;
	}
	void setFrameflags(BYTE flags) {
		*(BYTE*)((uintptr_t)this + 0x1F9) = flags;
	}
	//oSwimSpeed = 0x150 -- float
	//oFrameFlags = 0x1F9
	//oWalkSpeed = 0x16C
	//oWanted = 0x868 --int8
	//oRunSpeed = 0xCD0 -- float  



};

class CPrimaryAmmoCount {
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 AmmoCount; //0x0018 

	void InfGunAmmo() {
		AmmoCount = 999;
	}

};//Size=0x0040

class CAmmoCount {
public:
	CPrimaryAmmoCount* _PrimaryAmmoCount; //0x0000 
	char pad_0x0008[0x38]; //0x0008

};//Size=0x0040

class CAmmoInfo {
public:
	char pad_0x0000[0x8]; //0x0000
	CAmmoCount* _AmmoCount; //0x0008 
	char pad_0x0010[0x18]; //0x0010
	__int32 AmmoMax; //0x0028 
	char pad_0x002C[0x40C]; //0x002C

}; //Size=0x0438

class CWeaponObject {
public:
	char _0x0000[8];
	CAmmoInfo* m_pAmmoInfo; //0x0008 
	char _0x0010[16];
	__int32 m_iAmmo; //0x0020 
	char _0x0024[20];

};//Size=0x0038

class CInventory {
public:
	char _0x0000[72];
	CWeaponObject** WeaponList; //0x0048 

	CWeaponObject* getWeapon(int index) {
		return (CWeaponObject*)WeaponList[index];
	}
};//Size=0x0050

class CObjectWrapper {
public:
	char _0x0000[60];
	__int16 TeamId; //0x003C 
	char _0x0040[104];
	CPlayerInfo* playerInfo; //0x00A8 
	char _0x00B0[64];

	__int16 GetTeamId() {
		return ((TeamId) << 0 >> 0 << 0);
	}

};//Size=0x00F0




class CPlayers {
public:
	char _0x0000[376];
	__int32 numPlayersOnline; //0x0178 
	char _0x017C[4];
	CObjectWrapper* ObjectWrapperList[256]; //0x0180 

	CObject* getPlayer(int index) {
		if (IsValidPtr(ObjectWrapperList[index]) && IsValidPtr(ObjectWrapperList[index]->playerInfo) && IsValidPtr(ObjectWrapperList[index]->playerInfo->pCPed))
			return (CObject*)ObjectWrapperList[index]->playerInfo->pCPed;
		return NULL;
	}

	CPlayerInfo* getPlayerInfo(int index) {
		if (IsValidPtr(ObjectWrapperList[index]) && IsValidPtr(ObjectWrapperList[index]->playerInfo))
			return (CPlayerInfo*)ObjectWrapperList[index]->playerInfo;
		return NULL;
	}

	char* getPlayerName(int index) {
		if (IsValidPtr(ObjectWrapperList[index]) && IsValidPtr(ObjectWrapperList[index]->playerInfo))
			return (char*)ObjectWrapperList[index]->playerInfo->sName;
		return NULL;
	}

	int GetNumPlayersOnline() {
		if (numPlayersOnline > 1 && numPlayersOnline <= 0x20)
			return numPlayersOnline;
		else
			return 0;
	}
};


DWORD64 FrameCount = 0x0;
static uint64_t GetFrameCount() {
	uint64_t        cur = *(uint64_t*)(FrameCount);
	return cur;
}


class CWorld {
public:
	char _0x0000[8];
	CObject* pLocalPlayer; //0x0008 

	CObject* getLocalPlayer() {
		if (IsValidPtr(pLocalPlayer)) {
			return pLocalPlayer;
		} else {
			return nullptr;
		}
	}
};//Size=0x0016

class CGameCameraAngles;
class CCameraManagerAngles;
class CCameraAngles;
class CPlayerAngles;

class CGameCameraAngles {
public:
	CCameraManagerAngles* CameraManagerAngles; //0x0000  
	char _0x0008[56];

};//Size=0x0040 

class CCameraManagerAngles {
public:
	CCameraAngles* MyCameraAngles; //0x0000  

};//Size=0x0008 




class CCameraAngles
{
public:
	char pad_0x0000[0x2C0]; //0x0000
	CPlayerAngles* VehiclePointer1; //0x02C0 
	CPlayerAngles* VehiclePointer2; //0x02C8 
	char pad_0x02D0[0xF0]; //0x02D0
	CPlayerAngles* pMyFPSAngles; //0x03C0 
	char pad_0x03C8[0x10]; //0x03C8
	__int64 pTPSCamEDX; //0x03D8 
	char pad_0x03E0[0x60]; //0x03E0

}; //Size=0x0440
// Old
// class CCameraAngles {
//public:
//	char _0x0000[960];
//	CPlayerAngles* pMyFPSAngles; //0x03C0  
//	char _0x03C8[16];
//	__int64 pTPSCamEDX; //0x03D8  // This is passed as second parameter to oGetTPSCamera 
//	char _0x03E0[40];
//
//};//Size=0x0408 

class CPlayerCameraData {
public:
	char _0x0000[48];
	float Fov_Zoom; //0x0030 
	char _0x0034[36];
	__int32 m_ZoomState; //0x0058 1 = Normal, 0 = Zoom, 10000+ = FPS



	float getMass() {
		return *(float*)((uintptr_t)this + 0xC);
	}



};//Size=0x0548

class CPlayerAngles {
public:
	char pad_0000[16]; //0x0000
	CPlayerCameraData* m_cam_data; //0x0010
	char pad_0018[40]; //0x0018
	Vector3 m_fps_angles; //0x0040
	char pad_004C[20]; //0x004C
	Vector3 m_cam_pos; //0x0060 // for fps and tps
	char pad_006C[212]; //0x006C
	Vector3 m_cam_pos2; //0x0140
	char pad_014C[644]; //0x014C
	Vector3 m_tps_angles; //0x03D0
	char pad_03DC[20]; //0x03DC
	Vector3 m_fps_cam_pos; //0x03F0
	char pad_03FC[88]; //0x03FC
};

//struct CPlayerAngles {
//	uint8_t pad1[16];
//	CPlayerCameraData* m_cam_data; // CPlayerCameraData *
//	uint8_t pad2[24];
//	rage::fvector3 right;
//	uint8_t pad3[4];
//	rage::fvector3 forward;
//	uint8_t pad4[4];
//	rage::fvector3 up;
//	uint8_t pad5[4];
//	rage::fvector3 position;
//	uint8_t pad6[36];
//};

class CVehicleColors {
public:
	char pad_0x0000[0xA4]; //0x0000
	BYTE btPrimaryBlue; //0x00A4 
	BYTE btPrimaryGreen; //0x00A5 
	BYTE btPrimaryRed; //0x00A6 
	BYTE btPrimaryAlpha; //0x00A7 
	BYTE btSecondaryBlue; //0x00A8 
	BYTE btSecondaryGreen; //0x00A9 
	BYTE btSecondaryRed; //0x00AA 
	BYTE btSecondaryAlpha; //0x00AB 

	void SetColor(float Primary[2], float Secondary[2]) {
		btPrimaryRed = BYTE(Primary[0] * 255);
		btPrimaryGreen = BYTE(Primary[1] * 255);
		btPrimaryBlue = BYTE(Primary[2] * 255);

		btSecondaryRed = BYTE(Secondary[0] * 255);
		btSecondaryGreen = BYTE(Secondary[1] * 255);
		btSecondaryBlue = BYTE(Secondary[2] * 255);
	}

}; //Size=0x00AC

class CVehicleColorOptions {
public:
	char pad_0x0000[0x20]; //0x0000
	CVehicleColors* CVehicleColor; //0x0020 

}; //Size=0x0028

class CVehicleHandling {
public:
	char pad_0x0000[0xC]; //0x0000
	float fMass; //0x000C 
	char pad_0x0010[0x10]; //0x0010
	Vector3 v3CentreOfMassOffset; //0x0020 
	char pad_0x002C[0x4]; //0x002C
	Vector3 v3InertiaMult; //0x0030 
	char pad_0x003C[0x4]; //0x003C
	float fPercentSubmerged; //0x0040 
	char pad_0x0044[0x8]; //0x0044
	float fAcceleration; //0x004C 
	BYTE btInitialDriveGears; //0x0050 
	char pad_0x0051[0x3]; //0x0051
	float fDriveInertia; //0x0054 
	float fClutchChangeRateScaleUpShift; //0x0058 
	float fClutchChangeRateScaleDownShift; //0x005C 
	float fInitialDriveForce; //0x0060 
	char pad_0x0064[0x8]; //0x0064
	float fBrakeForce; //0x006C 
	char pad_0x0070[0x4]; //0x0070
	float fBrakeBiasFront; //0x0074 
	char pad_0x0078[0x4]; //0x0078
	float fHandBrakeForce; //0x007C 
	char pad_0x0080[0x8]; //0x0080
	float fTractionCurveMax; //0x0088 
	char pad_0x008C[0x4]; //0x008C
	float fTractionCurveMin; //0x0090 
	char pad_0x0094[0xC]; //0x0094
	float fTractionSpringDeltaMax; //0x00A0 
	char pad_0x00A4[0x4]; //0x00A4
	float fLowSpeedTractionLossMult; //0x00A8 
	float fCamberStiffnesss; //0x00AC 
	float fTractionBiasFront; //0x00B0 
	float fTwoMinus_fTractionBiasFront; //0x00B4 
	float fTractionLossMult; //0x00B8 
	float fSuspensionForce; //0x00BC 
	float fSuspensionCompDamp; //0x00C0 
	float fSuspensionReboundDamp; //0x00C4 
	float fSuspensionUpperLimit; //0x00C8 
	float fSuspensionLowerLimit; //0x00CC 
	char pad_0x00D0[0xC]; //0x00D0
	float fAntiRollBarForce; //0x00DC 
	char pad_0x00E0[0x8]; //0x00E0
	float fRollCentreHeightFront; //0x00E8 
	float fRollCentreHeightRear; //0x00EC 
	float fCollisionDamageMult; //0x00F0 
	float fWeaponDamageMult; //0x00F4 
	float fDeformationDamageMult; //0x00F8 
	float fEngineDamageMult; //0x00FC 
	float fPetrolTankVolume; //0x0100 
	float fOilVolume; //0x0104 



	float getMass() {
		return *(float*)((uintptr_t)this + 0xC);
	}
	void setMass(float v) {
		*(float*)((uintptr_t)this + 0xC) = v;
	}
	float getAcceleration() {
		return *(float*)((uintptr_t)this + 0x4C);
	}
	void setAcceleration(float v) {
		*(float*)((uintptr_t)this + 0x4C) = v;
	}
	float getInitialDriveForce() {
		return *(float*)((uintptr_t)this + 0x60);
	}
	void setInitialDriveForce(float v) {
		*(float*)((uintptr_t)this + 0x60) = v;
	}

	void setEngineMult(float v) {
		*(float*)((uintptr_t)this + 0xFC) = v;
	}
	void setCollisionMult(float v) {
		*(float*)((uintptr_t)this + 0xF0) = v;
	}
	void setDeformationMult(float v) {
		*(float*)((uintptr_t)this + 0xF8) = v;
	}
	void setThrust(float v) {
		*(float*)((uintptr_t)this + 0x338) = v;
	}




	//oBouyancy = 0x40 -- float
	//oAcceleration = 0x4C
	//oDriveInertia = 0x54
	//oInitialDriveForce = 0x60
	//oBrakeForce = 0x6C
	//oHandbrakeForce = 0x7C
	//oTractionCurveMax = 0x88
	//oTractionCurveMin = 0x90
	//oCollisionMult = 0xF0
	//oWeaponMult = 0xF4
	//oDeformationMult = 0xF8
	//oEngineMult = 0xFC
	//oThrust = 0x338







}; //Size=0x0108



enum ePedType {
	PEDTYPE_PLAYER_0,                // michael 
	PEDTYPE_PLAYER_1,                // franklin 
	PEDTYPE_NETWORK_PLAYER,            // mp character 
	PEDTYPE_PLAYER_2,                // trevor 
	PEDTYPE_CIVMALE,
	PEDTYPE_CIVFEMALE,
	PEDTYPE_COP,
	PEDTYPE_GANG_ALBANIAN,
	PEDTYPE_GANG_BIKER_1,
	PEDTYPE_GANG_BIKER_2,
	PEDTYPE_GANG_ITALIAN,
	PEDTYPE_GANG_RUSSIAN,
	PEDTYPE_GANG_RUSSIAN_2,
	PEDTYPE_GANG_IRISH,
	PEDTYPE_GANG_JAMAICAN,
	PEDTYPE_GANG_AFRICAN_AMERICAN,
	PEDTYPE_GANG_KOREAN,
	PEDTYPE_GANG_CHINESE_JAPANESE,
	PEDTYPE_GANG_PUERTO_RICAN,
	PEDTYPE_DEALER,
	PEDTYPE_MEDIC,
	PEDTYPE_FIREMAN,
	PEDTYPE_CRIMINAL,
	PEDTYPE_BUM,
	PEDTYPE_PROSTITUTE,
	PEDTYPE_SPECIAL,
	PEDTYPE_MISSION,
	PEDTYPE_SWAT,
	PEDTYPE_ANIMAL,
	PEDTYPE_ARMY
};

enum ExplosionTypes {
	EXPLOSION_GRENADE,
	EXPLOSION_GRENADELAUNCHER,
	EXPLOSION_STICKYBOMB,
	EXPLOSION_MOLOTOV,
	EXPLOSION_ROCKET,
	EXPLOSION_TANKSHELL,
	EXPLOSION_HI_OCTANE,
	EXPLOSION_CAR,
	EXPLOSION_PLANE,
	EXPLOSION_PETROL_PUMP,
	EXPLOSION_BIKE,
	EXPLOSION_DIR_STEAM,
	EXPLOSION_DIR_FLAME,
	EXPLOSION_DIR_WATER_HYDRANT,
	EXPLOSION_DIR_GAS_CANISTER,
	EXPLOSION_BOAT,
	EXPLOSION_SHIP_DESTROY,
	EXPLOSION_TRUCK,
	EXPLOSION_BULLET,
	EXPLOSION_SMOKEGRENADELAUNCHER,
	EXPLOSION_SMOKEGRENADE,
	EXPLOSION_BZGAS,
	EXPLOSION_FLARE,
	EXPLOSION_GAS_CANISTER,
	EXPLOSION_EXTINGUISHER,
	EXPLOSION_PROGRAMMABLEAR,
	EXPLOSION_TRAIN,
	EXPLOSION_BARREL,
	EXPLOSION_PROPANE,
	EXPLOSION_BLIMP,
	EXPLOSION_DIR_FLAME_EXPLODE,
	EXPLOSION_TANKER,
	EXPLOSION_PLANE_ROCKET,
	EXPLOSION_VEHICLE_BULLET,
	EXPLOSION_GAS_TANK,
	EXPLOSION_BIRD_CRAP
};

enum WeaponHashes {
	WEAPON_ADVANCEDRIFLE = 0xAF113F99,
	WEAPON_AIRSTRIKE_ROCKET = 0x13579279,
	WEAPON_ANIMAL = 0xF9FBAEBE,
	WEAPON_APPISTOL = 0x22D8FE39,
	WEAPON_ASSAULTRIFLE = 0xBFEFFF6D,
	WEAPON_ASSAULTSHOTGUN = 0xE284C527,
	WEAPON_ASSAULTSMG = 0xEFE7E2DF,
	WEAPON_TEC9 = 0xDB1AA450,
	WEAPON_DOUBLEBARRELED = 0xDB1AA450,
	WEAPON_BALL = 0x23C9F95C,
	WEAPON_BARBED_WIRE = 0x48E7B178,
	WEAPON_BAT = 0x958A4A8F,
	WEAPON_BLEEDING = 0x8B7333FB,
	WEAPON_BOTTLE = 0xF9E6AA4B,
	WEAPON_BRIEFCASE = 0x88C78EB7,
	WEAPON_BRIEFCASE_02 = 0x01B79F17,
	WEAPON_BULLPUPRIFLE = 0x7F229F94,
	WEAPON_COMPACTRIFLE = 0x624FE830,
	WEAPON_BULLPUPSHOTGUN = 0x9D61E50F,
	WEAPON_BZGAS = 0xA0973D5E,
	WEAPON_CARBINERIFLE = 0x83BF0278,
	WEAPON_COMBATMG = 0x7FD62962,
	WEAPON_PARACHUTE = 0xFBAB5776,
	WEAPON_COMBATPDW = 0xA3D4D34,
	WEAPON_COMBATPISTOL = 0x5EF9FEC4,
	WEAPON_COUGAR = 0x08D4BE52,
	WEAPON_CROWBAR = 0x84BD7BFD,
	WEAPON_DAGGER = 0x92A27487,
	WEAPON_DIGISCANNER = 0xFDBADCED,
	WEAPON_DROWNING = 0xFF58C4FB,
	WEAPON_DROWNING_IN_VEHICLE = 0x736F5990,
	WEAPON_ELECTRIC_FENCE = 0x92BD4EBB,
	WEAPON_EXHAUSTION = 0x364A29EC,
	WEAPON_EXPLOSION = 0x2024F4E8,
	WEAPON_FALL = 0xCDC174B0,
	WEAPON_FIRE = 0xDF8E89EB,
	WEAPON_FIREEXTINGUISHER = 0x060EC506,
	WEAPON_FIREWORK = 0x7F7497E5,
	WEAPON_FLARE = 0x497FACC3,
	WEAPON_FLAREGUN = 0x47757124,
	WEAPON_GARBAGEBAG = 0xE232C28C,
	WEAPON_GOLFCLUB = 0x440E4788,
	WEAPON_GRENADE = 0x93E220BD,
	WEAPON_GRENADELAUNCHER = 0xA284510B,
	WEAPON_GRENADELAUNCHER_SMOKE = 0x4DD2DC56,
	WEAPON_GUSENBERG = 0x61012683,
	WEAPON_HAMMER = 0x4E875F73,
	WEAPON_HANDCUFFS = 0xD04C944D,
	WEAPON_HEAVYPISTOL = 0xD205520E,
	WEAPON_HEAVYSHOTGUN = 0x3AABBBAA,
	WEAPON_HEAVYSNIPER = 0x0C472FE2,
	WEAPON_HELI_CRASH = 0x145F1012,
	WEAPON_HIT_BY_WATER_CANNON = 0xCC34325E,
	WEAPON_HOMINGLAUNCHER = 0x63AB0442,
	WEAPON_KNIFE = 0x99B507EA,
	WEAPON_KNUCKLE = 0xD8DF3C3C,
	WEAPON_MARKSMANPISTOL = 0xDC4DB296,
	WEAPON_MARKSMANRIFLE = 0xC734385A,
	WEAPON_MG = 0x9D07F764,
	WEAPON_HATCHET = 0xF9DCBF2D,
	WEAPON_FLASHLIGHT = 0x8BB05FD7,
	WEAPON_MACHINEPISTOL = 0xDB1AA450,
	WEAPON_MACHETE = 0xDD5DF8D9,
	WEAPON_SWITCHBLADE = 0xDFE37640,
	WEAPON_REVOLVER = 0xC1B3C3D1,
	WEAPON_MICROSMG = 0x13532244,
	WEAPON_MINIGUN = 0x42BF8A85,
	WEAPON_MOLOTOV = 0x24B17070,
	WEAPON_MUSKET = 0xA89CB99E,
	WEAPON_NIGHTSTICK = 0x678B81B1,
	WEAPON_PASSENGER_ROCKET = 0x166218FF,
	WEAPON_PETROLCAN = 0x34A67B97,
	WEAPON_PISTOL = 0x1B06D571,
	WEAPON_PISTOL50 = 0x99AEEB3B,
	WEAPON_PROXMINE = 0xAB564B93,
	WEAPON_PUMPSHOTGUN = 0x1D073A89,
	WEAPON_RAILGUN = 0x6D544C99,
	WEAPON_RAMMED_BY_CAR = 0x07FC7D7A,
	WEAPON_REMOTESNIPER = 0x33058E22,
	WEAPON_RPG = 0xB1CA77B1,
	WEAPON_RUN_OVER_BY_CAR = 0xA36D413E,
	WEAPON_SAWNOFFSHOTGUN = 0x7846A318,
	WEAPON_SMG = 0x2BE6766B,
	WEAPON_SMOKEGRENADE = 0xFDBC8A50,
	WEAPON_SNIPERRIFLE = 0x05FC3C11,
	WEAPON_SNOWBALL = 0x787F0BB,
	WEAPON_SNSPISTOL = 0xBFD21232,
	WEAPON_SPECIALCARBINE = 0xC0A3098D,
	WEAPON_STICKYBOMB = 0x2C3731D9,
	WEAPON_STINGER = 0x687652CE,
	WEAPON_STUNGUN = 0x3656C8C1,
	WEAPON_UNARMED = 0xA2719263,
	WEAPON_VEHICLE_ROCKET = 0xBEFDC581,
	WEAPON_VINTAGEPISTOL = 0x083839C4
};

struct RawRaycastResult {
	int Result = 0;
	bool DidHitAnything = false;
	bool DidHitEntity = false;
	DWORD HitEntity = (DWORD)0;
	Vector3 HitCoords;
};


typedef struct
{
	int Result;
	bool DidHitAnything;
	bool DidHitEntity;
	int32_t HitEntity;
	PVector3 HitCoords;
	PVector3 HitNormal;
} RaycastResult;

enum IntersectOptions {
	IntersectEverything = -1,
	IntersectMap = 1,
	IntersectVehicles = 2,
	IntersectPeds1 = 4,
	IntersectPeds2 = 8,
	IntersectObjects = 16,
	IntersectVegetation = 256
};
