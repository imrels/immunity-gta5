#pragma once
#include "../imports.h"


namespace game {

	extern vector<pair<CObject*, DataPed>> ped_list;
	extern vector<CObject*> object_list;
	extern vector<CVehicle*> vehicle_list;
	extern vector<CVehicle*> nearby_vehicle_list;

	extern bool isValidPlayer(DWORD hash, CObject* player);

	extern DWORD freemode_f;
	extern DWORD freemode_m;
}

namespace hacks {
	void kill_aura() {
		for (pair<CObject*, DataPed>& entity : game::ped_list) {
			CObject* ped = entity.first;
			DataPed data = entity.second;
			if (IsValidPtr(ped)) {
				DWORD hash = ped->ModelInfo()->GetHash();
				if ((hash != game::freemode_f) && (hash != game::freemode_m)) {
					ped->HP = 0.f;
				}
			}
		}
		return;
	}
	map<char*, float> default_recoil;
	map<char*, float> default_spread;
	void update_hacks() {
		//static map<string, float> previous_values;
		// weapon cheats
		try {
			if (IsValidPtr(local.player)) {
				bool toggle_key = config::get("visual", "toggle_key", 0);
				if (toggle_key) {
					int toggle_key_num = config::get("visual", "toggle_key_num", 0);
					static bool old_state = Game.keyStates[toggle_key_num];
					if ((Game.keyStates[toggle_key_num] && !old_state) || ((toggle_key_num <= 6) && (GetAsyncKeyState(toggle_key_num) & 0x8000))) {
						bool visuals_enable = config::get("visual", "enable", 0);
						config::update(!visuals_enable, "visual", "enable", 0);
					}
					old_state = Game.keyStates[toggle_key_num];
				}


				if ((config::get("hacks", "reload_speed", 0)) || (config::get("hacks", "weapon_damage", 0)) || config::get("hacks", "extend_range", 0) || (config::get("hacks", "remove_recoil", 0)) || (config::get("hacks", "remove_spread", 0))) {
					CWeaponManager* wep = local.player->weapon();
					if (IsValidPtr(wep)) {
						if (IsValidPtr(wep->_WeaponInfo)) {
							auto aWep = wep->_WeaponInfo;

							if (IsValidPtr(aWep)) {
								char* name = aWep->GetSzWeaponName();


								if (config::get("hacks", "extend_range", 0)) {

									aWep->Range(config::get("hacks", "extended_range", 0.f));
								}
								if (config::get("hacks", "remove_recoil", 0)) {
									if (default_recoil.find(name) == default_recoil.end()) {
										default_recoil[name] = aWep->getRecoil();
										ui::DEBUG.AddLog("default for %s val %.3f %.3f\n", name, default_recoil[name]);
									}
									float new_recoil = default_recoil[name] / 100 * config::get("hacks", "remove_recoil_val", 0.f);
									//
									//ui::DEBUG.AddLog("new recoil %s\n", name);
									//ui::DEBUG.AddLog("default val %.0000f %.00000f\n", default_recoil[name], aWep->getRecoil());
									//ui::DEBUG.AddLog("new recoil val %.3f\n", new_recoil);


									aWep->setRecoil(new_recoil);
								}
								if (config::get("hacks", "remove_spread", 0)) {
									if (default_spread.find(name) == default_spread.end()) {
										default_spread[name] = aWep->getSpread();
										ui::DEBUG.AddLog("default spread for %s val %.3f %.3f\n", name, default_spread[name]);
									}
									float new_spread = default_spread[name] / 100 * config::get("hacks", "remove_spread_val", 0.f);

									aWep->setSpread(new_spread);
								}
								if (config::get("hacks", "weapon_damage", 0)) {
									aWep->setDamage(config::get("hacks", "weapon_damage_value", 0.f));
									//
								}
								if (config::get("hacks", "reload_speed", 0)) {
									aWep->setReloadSpeed(config::get("hacks", "reload_speed_value", 1.f));
								}
							}
						}
					}
				}

				static bool was_godmode_set = false;
				if (config::get("hacks", "godmode", 0)) {
					int gmode = config::get("hacks", "godmode_mode", 0);
					if (gmode == 0) {
						local.player->HP = 100.f + config::get("hacks", "godmode_val", 99.0f);// local.player->MaxHP > 100 ? local.player->MaxHP : 200.f;// 200.f;
						if (config::get("misc", "spoof_godmode_checks", 0)) {
							local.player->set_godmode(true);
						}
						was_godmode_set = true;
					} else if (gmode == 1) {
						float cur_hp = 100.f + config::get("hacks", "godmode_val", 99.0f);
						if (local.player->HP < cur_hp) {
							local.player->HP = cur_hp;
						}
						local.player->set_godmode(false);
					} else if (gmode == 2) {
						int key = config::get("hacks", "heal_key", 1);
						if (Game.keyStates[key] || ((key <= 6) && (GetAsyncKeyState(key) & 0x8000))) {
							local.player->HP = 100.f + config::get("hacks", "godmode_val", 99.0f);
							local.player->set_godmode(false);
						}
					}
				} else {
					if (was_godmode_set) {
						was_godmode_set = false;
						local.player->set_godmode(false);
					}
				}


				if (config::get("hacks", "auto_armor", 0)) {
					int amode = config::get("hacks", "autoarmor_mode", 0);
					if (amode == 0) {

						local.player->SetArmor(config::get("hacks", "auto_armor_val", 99.0f));
					} else if (amode == 1) {
						float cur_a = config::get("hacks", "auto_armor_val", 99.0f);
						if (local.player->GetArmor() < cur_a) {
							local.player->SetArmor(cur_a);
						}
					} else if (amode == 2) {

						int key = config::get("hacks", "armor_key", 1);
						if (Game.keyStates[key] || ((key <= 6) && (GetAsyncKeyState(key) & 0x8000))) {
							local.player->SetArmor(config::get("hacks", "auto_armor_val", 99.0f));
						}
					}
				}


				if (config::get("hacks", "force_engine", 0)) {
					if (local.player->IsInVehicle()) {
						CVehicle* veh = local.player->vehicle();
						if (IsValidPtr(veh)) {
							veh->setEngine(0x14);
						}
					}
				}
				if (config::get("hacks", "unlock_nearby", 0)) {

					for (CVehicle*& veh : game::nearby_vehicle_list) {
						veh->setUnlocked(0x0);
					}
				}

				//if (!isFiveM) {
				if (local.player->IsInVehicle()) {
					static bool was_gravity_applied = false;
					static bool was_god_applied = false;
					if (
						(config::get("hacks", "vehicle_gravity", 0) || was_gravity_applied) ||
						(config::get("hacks", "vehicle_godmode", 0) || was_god_applied) ||
						(config::get("hacks", "vehicle_acceleration", 0)) ||
						(config::get("hacks", "no_deformations", 0))
						) {


						CVehicle* veh = local.player->vehicle();
						if (IsValidPtr(veh)) {

							//if (!isFiveM) {
							if (config::get("hacks", "vehicle_gravity", 0)) {
								was_gravity_applied = true;
								veh->setGravity(config::get("hacks", "vehicle_gravity_val", 9.8f));
							} else {
								if (was_gravity_applied) {
									was_gravity_applied = false;
									veh->setGravity(9.8f);
								}
							}
							//}

							if (config::get("hacks", "vehicle_acceleration", 0)) {
								veh->handling()->setAcceleration(config::get("hacks", "vehicle_acceleration_val", 1.f));
							}

							if (config::get("hacks", "vehicle_godmode", 0)) {
								veh->fHealth = veh->fHealthMax;
								was_god_applied = true;
								//if (config::get("misc", "spoof_godmode_checks", 0)) {
								//	veh->btGodMode = 0x1;
								//}
							} else {
								if (was_god_applied) {
									was_god_applied = false;
									veh->btGodMode = 0x0;
								}
							}
							if (config::get("hacks", "no_deformations", 0)) {
								veh->handling()->setDeformationMult(0.0f);
								veh->handling()->setCollisionMult(0.0f);
								veh->handling()->setEngineMult(0.0f);
							}
						}
					}
				} else {
					static bool ragdoll_removed = false;
					if (config::get("hacks", "no_ragdoll", 0)) {
						local.player->setRagdoll(0x1);
						ragdoll_removed = true;
					} else if (!config::get("hacks", "no_ragdoll", 0) && ragdoll_removed) {
						ragdoll_removed = false;
						local.player->setRagdoll(0x20);
					}



					static bool collision_removed = false;
					if (config::get("hacks", "no_collision_ped", 0)) {
						local.player->setCollision(-1.0f);
						collision_removed = true;
					} else if (!config::get("hacks", "no_collision_ped", 0) && collision_removed) {
						local.player->setCollision(0.25f);
						collision_removed = false;
					}
				}
				//}



				if (config::get("hack", "debug", 0)) {

					CVehicle* veh = local.player->vehicle();
					if (IsValidPtr(veh)) {
						ui::DEBUG.AddLog("veh -> 0x%x\n", veh);
					}
					ui::DEBUG.AddLog("isInVeh -> 0x%x\n", local.player->IsInVehicle());



				}
			}
		} catch (const std::exception& e) { /* */
			logs::add(enc("error updating generic hacks"));
			logs::add(e.what());
		}
	}



	Vector3 get_cam_directions() {



		float heading = native::cam::get_gameplay_cam_relative_heading() + native::entity::get_entity_heading(native::player::player_ped_id());
		float pitch = native::cam::get_gameplay_cam_relative_pitch();

		float x = -sin(heading * PI / 180.0f);
		float y = cos(heading * PI / 180.0f);
		float z = sin(pitch * PI / 180.0f);

		double len = sqrt(x * x + y * y + z * z);
		if (len != 0) {
			x = x / len;
			y = y / len;
			z = z / len;
		}

		return Vector3(x, y, z);
	}

	void no_clip() {

		bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);

		auto pos = local.player->fPosition;
		native::type::any entity;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float speed = config::get("hacks", "noclip_speed", 1.f);

		if (!is_in_vehicle) {
			entity = native::player::get_player_ped(-1);
			x = pos.x;
			y = pos.y;
			z = pos.z;
		} else {

			if (local.player->IsInVehicle()) {
				CVehicle* veh = local.player->vehicle();
				pos = veh->fPosition;
			}

			entity = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
			x = pos.x;
			y = pos.y;
			z = pos.z;
		}

		auto cam_direction = get_cam_directions();

		float dx = cam_direction.x;
		float dy = cam_direction.y;
		float dz = cam_direction.z;
		float heading = native::cam::get_gameplay_cam_relative_heading() + native::entity::get_entity_heading(entity);
		float pitch = native::cam::get_gameplay_cam_relative_pitch();

		if (GetAsyncKeyState(0x57)) {
			x = x + speed * dx;
			y = y + speed * dy;
			z = z + speed * dz;

		}

		if (GetAsyncKeyState(0x53)) {
			x = x - speed * dx;
			y = y - speed * dy;
			z = z - speed * dz;
		}

		if (GetAsyncKeyState(0x45)) // e
			z = z + speed;

		if (GetAsyncKeyState(0x51)) // q
			z = z - speed;

		native::entity::set_entity_coords_no_offset(entity, x, y, z, true, true, true);

		if (is_in_vehicle) {
			native::entity::set_entity_rotation(entity, pitch, 0, heading, 0, true);
		}

	}




	void veh_boost() {
		try {
			if (local.player->IsInVehicle()) {
				int veh_boost_key = config::get("hacks", "veh_boost_key", 1);
				if ((Game.keyStates[veh_boost_key]) || ((veh_boost_key <= 6) && (GetAsyncKeyState(veh_boost_key & 0x8000)))) {
					native::type::any lp_veh = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
					if (lp_veh) {
						float veh_boost_speed = config::get("hacks", "veh_boost_speed", 1.f);
						native::vehicle::set_vehicle_forward_speed(lp_veh, veh_boost_speed);
					}
				}
			}
		} catch (const std::exception& e) { /* */
			logs::add(enc("error updating veh boost"));
			logs::add(e.what());
		}
	}
	static bool freecamCreated = false;
	native::type::cam freecamEntity;

	namespace freecam {
		Vector3 rot;
		Vector3 pos;
		Vector3 vecX;
		Vector3 vecY;
		Vector3 vecZ;

		native::type::any target_entity;
	}

	float to_rad(float e) {
		return (e * PI) / 180;
	}

	void update_vec(Vector3 rot) {
		float radX = to_rad(rot.x);
		float radY = to_rad(rot.y);
		float radZ = to_rad(rot.z);

		float sinX = sin(radX);
		float sinY = sin(radY);
		float sinZ = sin(radZ);
		float cosX = cos(radX);
		float cosY = cos(radY);
		float cosZ = cos(radZ);

		Vector3 vecX;
		Vector3 vecY;
		Vector3 vecZ;

		vecX.x = cosY * cosZ;
		vecX.y = cosY * sinZ;
		vecX.z = -sinY;

		vecY.x = cosZ * sinX * sinY - cosX * sinZ;
		vecY.y = cosX * cosZ - sinX * sinY * sinZ;
		vecY.z = cosY * sinX;

		vecZ.x = -cosX * cosZ * sinY + sinX * sinZ;
		vecZ.y = -cosZ * sinX + cosX * sinY * sinZ;
		vecZ.z = cosX * cosY;

		freecam::vecX = Vector3(vecX.x, vecX.y, vecX.z);
		freecam::vecY = Vector3(vecY.x, vecY.y, vecY.z);
		freecam::vecZ = Vector3(vecZ.x, vecZ.y, vecZ.z);
	}

	void do_freecam() {
		using namespace native;

		if (freecamCreated && !config::get("hacks", "freecam_enabled", 0)) {
			//Destroy freecam


			freecamCreated = false;
			cam::destroy_cam(freecamEntity, false);


			player::set_player_control(native::player::player_id(), true, (1 << 8));
			cam::render_script_cams(false, false, 0, false, false);

			freecamEntity = 0;
			return;
		}
		if (!freecamCreated) {
			freecamCreated = true;
			freecamEntity = native::cam::create_cam("DEFAULT_SCRIPTED_CAMERA", true);
			cam::set_cam_fov(freecamEntity, config::get("hacks", "freecam_fov", 60.f));

			auto gameplay_cam_pos = cam::get_gameplay_cam_coord();
			auto gameplay_cam_rot = cam::get_gameplay_cam_rot(0);



			freecam::rot = Vector3(gameplay_cam_rot.x, gameplay_cam_rot.y, gameplay_cam_rot.z);
			freecam::pos = Vector3(gameplay_cam_pos.x, gameplay_cam_pos.y, gameplay_cam_pos.z);
			cam::set_cam_coord(freecamEntity, gameplay_cam_pos.x, gameplay_cam_pos.y, gameplay_cam_pos.z);
			cam::set_cam_rot(freecamEntity, gameplay_cam_rot.x, gameplay_cam_rot.y, gameplay_cam_rot.z, 0);


			player::set_player_control(native::player::player_id(), false, (1 << 8));
			cam::render_script_cams(true, false, 0, false, false);

			return;
		}

		if (freecamCreated && freecamEntity) {
			auto lookX = controls::get_disabled_control_normal(0, 1);
			auto lookY = controls::get_disabled_control_normal(0, 2);

			auto moveX = controls::get_disabled_control_normal(0, 30);
			auto moveY = controls::get_disabled_control_normal(0, 31);

			auto moveZ1 = controls::get_disabled_control_normal(0, 152);
			auto moveZ2 = controls::get_disabled_control_normal(0, 153);


			freecam::rot.x = freecam::rot.x + (-lookY * 5);
			freecam::rot.z = freecam::rot.z + (-lookX * 5);
			freecam::rot.y = freecam::rot.y;


			float speedMultiplier = config::get("hacks", "freecam_speed", 2.f);
			freecam::pos = freecam::pos + (freecam::vecX * moveX * speedMultiplier);
			freecam::pos = freecam::pos + (freecam::vecY * -moveY * speedMultiplier);

			if (moveZ1) {
				freecam::pos = freecam::pos + (freecam::vecZ * -1 * speedMultiplier);
			} else if (moveZ2) {
				freecam::pos = freecam::pos + (freecam::vecZ * 1 * speedMultiplier);
			}

			cam::set_cam_coord(freecamEntity, freecam::pos.x, freecam::pos.y, freecam::pos.z);
			cam::set_cam_rot(freecamEntity, freecam::rot.x, freecam::rot.y, freecam::rot.z, 0);



			cam::set_cam_fov(freecamEntity, config::get("hacks", "freecam_fov", 60.f));

			update_vec(freecam::rot);


			if (config::get("hacks", "freecam_enabled", 0)) {

				PVector3 HitNormal = PVector3(0, 0, 0);
				PVector3 HitCoords = PVector3(0, 0, 0);
				Vector3 target;
				Vector3 start;
				start = freecam::pos + (freecam::vecX * 1);
				start = freecam::pos + (freecam::vecY * 1);
				start = freecam::pos + (freecam::vecZ * 1);


				target = freecam::pos + (freecam::vecX * 1000000);
				target = freecam::pos + (freecam::vecY * 1000000);

				RaycastResult result;
				worldprobe::get_shape_test_result(worldprobe::start_shape_test_capsule(start.x, start.y, start.z,
					target.x, target.y, target.z, 0.2f, IntersectVehicles, 0, 7), (bool*)&result.DidHitAnything, &result.HitCoords, &result.HitNormal, &result.HitEntity);



				if (result.DidHitAnything) {
					if (result.HitEntity) {
						if (player::get_player_ped(-1) != result.HitEntity) {
							freecam::target_entity = result.HitEntity;
							auto snatch = controls::is_disabled_control_just_pressed(0, 24);
							if (snatch) {
								auto pos = local.player->fPosition;
								native::entity::set_entity_coords_no_offset(freecam::target_entity, pos.x + 1, pos.y + 1, pos.z, true, true, true);
							}
						} else {
							freecam::target_entity = 0;
						}
					} else {
						freecam::target_entity = 0;
					}
				} else {
					freecam::target_entity = 0;
				}

			}

		}

	}



	static bool was_no_veh_collision_set = false;
	void do_veh_no_collision() {
		try {
			bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
			if (is_in_vehicle) {
				native::type::any lp_veh = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
				if (config::get("hacks", "no_collision_veh", 0)) {

					if (IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->vehicle_interface)) {
						for (int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
							CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));
							if (IsValidPtr(veh)) {
								float dist = get_distance(local.player->fPosition, veh->fPosition);
								if (dist > 100.f) continue;
								auto veh_handle = pointer_to_handle((uintptr_t)veh);
								if (veh_handle) {
									native::entity::set_entity_no_collision_entity(veh_handle, lp_veh, false);
								}
							}
						}
					}
					was_no_veh_collision_set = true;
				} else if (was_no_veh_collision_set && !config::get("hacks", "no_collision_veh", 0)) {
					if (IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->vehicle_interface)) {
						for (int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
							CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));
							if (IsValidPtr(veh)) {
								float dist = get_distance(local.player->fPosition, veh->fPosition);
								if (dist > 100.f) continue;
								auto veh_handle = pointer_to_handle((uintptr_t)veh);
								if (veh_handle) {
									native::entity::set_entity_no_collision_entity(veh_handle, lp_veh, true);
								}
							}
						}
					}
					was_no_veh_collision_set = false;
				}
			}
		} catch (const std::exception& e) { /* */
			logs::add(enc("error in updating vehicle collision"));
			logs::add(e.what());

		}

	}

	static bool was_fast_run_set = false;
	void do_fast_run() {
		try {
			if (!local.player->IsInVehicle()) {
				if (config::get("hacks", "fast_run", 0)) {
					was_fast_run_set = true;
					native::player::set_swim_multiplier_for_player(native::player::player_ped_id(), config::get("hacks", "fast_run_val", 1.f));
					native::ped::set_ped_move_rate_override(native::player::player_ped_id(), config::get("hacks", "fast_run_val", 1.f));
				} else if (was_fast_run_set && !config::get("hacks", "fast_run", 0)) {
					was_fast_run_set = true;
					native::player::set_swim_multiplier_for_player(native::player::player_ped_id(), 1.f);
					native::ped::set_ped_move_rate_override(native::player::player_ped_id(), 1.f);
				}
			}
		} catch (const std::exception& e) { /* */
			logs::add(enc("error in updating fast run"));
			logs::add(e.what());
		}
	}
	void game_tick() {
		if (IsValidPtr(local.player)) {
			if (config::get("hacks", "noclip_toggle_key", 0)) {
				int key = config::get("hacks", "noclip_toggle_key_num", 0);
				static bool old_state = false;
				if ((Game.keyStates[key] && !old_state) || ((key <= 6) && (GetAsyncKeyState(key & 0x8000)))) {
					bool noclip_enabled = config::get("hacks", "noclip", 0);
					config::update(!noclip_enabled, "hacks", "noclip", 0);
				}
				old_state = Game.keyStates[key];
			}

			if (config::get("hacks", "noclip", 0)) {
				no_clip();
			}
			if (config::get("hacks", "veh_boost_enabled", 0)) {
				veh_boost();
			}
			//if (config::get("hacks", "fast_run", 0) || was_fast_run_set) {
			//	do_fast_run();
			//}
			if (config::get("hacks", "no_collision_veh", 0) || was_no_veh_collision_set) {
				do_veh_no_collision();
			}

			if (config::get("hacks", "clear_task_key", 0)) {
				int key = config::get("hacks", "clear_task_key_key_num", 0);
				static bool old_state = false;
				if ((Game.keyStates[key] && !old_state) || ((key <= 6) && (GetAsyncKeyState(key & 0x8000)))) {
					native::brain::clear_ped_tasks_immediately(pointer_to_handle((uintptr_t)local.player));
				}
				old_state = Game.keyStates[key];
			}


		}
	}

	void tick() {
		if (IsValidPtr(local.player)) {
			update_hacks();
		}
	}
	void init() {
		cout << "hacks init" << endl;
		while (Game.running) {
			//if (!isFiveM) {
			tick();
			//}
			std::this_thread::sleep_for(10ms);
		}
		cout << "exit hacks" << endl;
	}

}