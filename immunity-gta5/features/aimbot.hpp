#pragma once
#include "imports.h"


namespace game {
	extern vector<pair<CObject*, DataPed>> ped_list;

	extern bool isValidPlayer(DWORD hash, CObject* player);
}


namespace aimbot {
	CObject* current_target;
	DataPed current_target_data;
	static Vector3 lastTargetVector = Vector3(0, 0, 0);
	static auto targetTimeout = std::chrono::high_resolution_clock::now();
	static float aimTargetDist = 99999.0f;
	static Vector3 target_aim_angle;


	struct vehicleState {
		bool state;
		uint64_t ticksSinceChange;
	};
	vehicleState vehicleAimbot;

	Vector3 get_aimbone_complex(Vector3 current, DataPed data) {
		Vector3 aim = current;
		PlayerBones bones = data.bones;
		Vector3 bone_positions[] = {
			bones.HEAD,
			bones.NECK,
			bones.RIGHT_HAND,
			bones.RIGHT_FOREARM,
			bones.RIGHT_UPPER_ARM,
			bones.RIGHT_CLAVICLE,
			bones.LEFT_HAND,
			bones.LEFT_FOREARM,
			bones.LEFT_UPPER_ARM,
			bones.LEFT_CLAVICLE,
			bones.PELVIS,
			bones.SPINE_ROOT,
			bones.SPINE0,
			bones.SPINE1,
			bones.SPINE2,
			bones.SPINE3,
			bones.RIGHT_TOE,
			bones.RIGHT_FOOT,
			bones.RIGHT_CALF,
			bones.RIGHT_THIGH,
			bones.LEFT_TOE,
			bones.LEFT_FOOT,
			bones.LEFT_CALF,
			bones.LEFT_THIGH
		};
		float closest_distance = 999.f;
		int arrSize = sizeof(bone_positions) / sizeof(bone_positions[0]);


		for (size_t i = 0; i < arrSize; i++) {
			ImVec2 screen_point;
			if (!WorldToScreen(bone_positions[i], &screen_point)) continue;
			float dist = screen_distance(Game.screen.x / 2, Game.screen.y / 2, screen_point.x, screen_point.y);
			if (closest_distance > dist) {
				closest_distance = dist;
				aim = bone_positions[i];
			}
		}


		return aim;
	}

	void get_new_target(CPlayerAngles* cam) {
		bool target_accquired = false;
		if (IsValidPtr(cam) && IsValidPtr(local.player)) {
			Vector3 CrosshairPos = cam->m_cam_pos;
			//float last_closest_dist = 9999.f;
			CObject* closest = nullptr;
			DataPed closest_data = {0};

			for (pair<CObject*, DataPed>& entity : game::ped_list) {
				CObject* ped = entity.first;
				DataPed data = entity.second;
				if (IsValidPtr(ped)) {
					if (local.player == ped) continue;
					if (ped->HP <= 0) continue;
					if (get_distance(local.player->fPosition, ped->fPosition) > config::get("hack", "max_range", 1000.f)) continue;
					DWORD hash = ped->ModelInfo()->GetHash();
					if (!game::isValidPlayer(hash, ped)) continue;


					//Vector3 aimpos = GetBonePosition(ped, HEAD);

					Vector3 aimpos = config::get("aimbot", "aim_bone", 0) == 0 ? data.bones.HEAD : config::get("aimbot", "aim_bone", 0) == 1 ? data.bones.NECK : data.bones.SPINE2;//GetBonePosition(current_target, AssistBone == 0 ? BONETAG_HEAD : AssistBone == 1 ? BONETAG_NECK : BONETAG_STOMACH); // GetBonePosition1

					if (config::get("aimbot", "aim_bone", 0) == 3) {
						aimpos = get_aimbone_complex(aimpos, data);
					}

					//config::get("aimbot", "only_visible", 0)

					if (!data.visible && config::get("aimbot", "only_visible", 0)) continue;

					if (data.group.ally) continue;
					if ((data.group.inGroup == false) && config::get("aimbot", "aim_only_groups", 0) && config::get("aimbot", "group_check", 0)) continue;

					ImVec2 aim2d;
					if (!WorldToScreen2(aimpos, &aim2d)) continue;


					float Distance2d = screen_distance(Game.screen.x / 2, Game.screen.y / 2, aim2d.x, aim2d.y);

					if (
						(Distance2d > config::get("aimbot", "aim_stop_radius", 0.f)) &&
						(config::get("aimbot", "aim_radius", 50.f) > Distance2d)
						) {
						if ((aimTargetDist > Distance2d) || (current_target == ped)) {
							aimTargetDist = Distance2d;
							target_accquired = true;
							closest = ped;
							closest_data = data;
						}
					}

				}
			}

			if (closest) {
				auto now = std::chrono::high_resolution_clock::now();
				auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - targetTimeout).count();

				if (closest != current_target) {
					if (dur > config::get("aimbot", "target_timeout", 0.f)) {
						//cout << "new targt " << dur << endl;
						current_target = closest;
						current_target_data = closest_data;
						targetTimeout = std::chrono::high_resolution_clock::now();


						return;
					}
				}
				if (current_target == nullptr) {
					if (dur > config::get("aimbot", "target_timeout", 0.f)) {
						//cout << "new targt " << dur << endl;
						current_target = closest;
						current_target_data = closest_data;
						targetTimeout = std::chrono::high_resolution_clock::now();
						return;
					}
				}
			}


		}
		if (!target_accquired) {
			current_target = nullptr;
			aimTargetDist = 9999.f;
			target_aim_angle = {0};
		}
	}

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	void do_silent_aim() {
		if (target_aim_angle.x != 0 && target_aim_angle.y != 0 && target_aim_angle.z != 0) {
			if (IsValidPtr(local.player)) {
				native::player::disable_player_firing(native::player::get_player_index(), true);
				if (native::controls::is_disabled_control_pressed(0, 24) || config::get("aimbot", "silent_aim_auto", 0)) {
					auto player = native::player::get_player_ped(-1);
					if (native::ped::is_ped_reloading(player)) return;

					native::ped::set_ped_shoots_at_coord(player, target_aim_angle.x, target_aim_angle.y, target_aim_angle.z, false);

				}
			}
		}
	}

	void setViewAnglesFix(Vector3 targetAngle) {


	}

	void tick() {
		float max_aim_radius = config::get("aimbot", "aim_radius", 50.f);
		float aim_stop_radius = config::get("aimbot", "aim_stop_radius", 50.f);
		float threshold = 0.5f / config::get("aimbot", "aim_strength", 500.f);
		CPlayerAngles* cam = Game.getCam();

		if (!config::get("aimbot", "enable", 0)) return;
		if (IsValidPtr(cam) && IsValidPtr(local.player)) {
			if (
				(GetAsyncKeyState(config::get("aimbot", "aim_key", 0)) & 0x8000) ||
				(config::get("aimbot", "aim_mode", 0) == 3)
				) {

				if ((local.player->IsInVehicle() == true) && !((config::get("aimbot", "aim_mode", 0) == 3))) return;


				CWeaponManager* wep = local.player->weapon();
				if (IsValidPtr(wep)) {
					if (IsValidPtr(wep->_WeaponInfo)) {
						auto str = wep->_WeaponInfo->GetSzWeaponName();
						string lstr(str, str + strlen(str));
						if (lstr == "UNARMED") return;
					}
				}
				auto old_target = current_target;
				get_new_target(cam);

				if (IsValidPtr(current_target)) {
					//cout << "current_target" << endl;
					if (current_target != old_target) return;
					if (get_distance(local.player->fPosition, current_target->fPosition) > config::get("hack", "max_range", 1000.f)) return;

					//int b = config::get("aimbot", "aim_bone", 0) == 0 ? 0 : config::get("aimbot", "aim_bone", 0) == 1 ? 7 : 8;
					//Vector3 aim3d = current_target->get_bone(b);//GetBonePosition(current_target, AssistBone == 0 ? BONETAG_HEAD : AssistBone == 1 ? BONETAG_NECK : BONETAG_STOMACH); // GetBonePosition1

					//Vector3 aim3d = config::get("aimbot", "aim_bone", 0) == 0 ? data.bones.HEAD : config::get("aimbot", "aim_bone", 0) == 1 ? data.bones.NECK : data.bones.SPINE0;



					auto match = std::find_if(game::ped_list.begin(), game::ped_list.end(), [](const pair<CObject*, DataPed>& v) {return v.first == current_target; });
					if (match == game::ped_list.end()) return;
					int index = std::distance(game::ped_list.begin(), match);

					DataPed data = game::ped_list[index].second;
					Vector3 aim3d = config::get("aimbot", "aim_bone", 0) == 0 ? data.bones.HEAD : config::get("aimbot", "aim_bone", 0) == 1 ? data.bones.NECK : data.bones.SPINE2;
					if (config::get("aimbot", "aim_bone", 0) == 3) {
						aim3d = get_aimbone_complex(aim3d, data);
					}
					//current_target_data
				//}



					ImVec2 aim2d;
					if (!WorldToScreen(aim3d, &aim2d)) return;
					float Distance2d = screen_distance(Game.screen.x / 2, Game.screen.y / 2, aim2d.x, aim2d.y);
					if (
						(Distance2d > config::get("aimbot", "aim_stop_radius", 0.f)) &&
						(config::get("aimbot", "aim_radius", 50.f) > Distance2d)
						) {

						Vector3 Crosshair3D = cam->m_cam_pos;
						if (config::get("aimbot", "aim_movement_prediction", 0)) {
							Vector3 EnemySpeed = current_target->v3Velocity;
							if (current_target->IsInVehicle()) {
								CVehicle* veh = current_target->vehicle();
								if (IsValidPtr(veh)) {
									EnemySpeed = veh->v3Velocity;
								}
							}
							aim3d = PredictMovement(aim3d, EnemySpeed, config::get("aimbot", "aim_movement_prediction_distance", 0.f));

							if (local.player->IsInVehicle()) {
								CVehicle* veh = local.player->vehicle();
								if (IsValidPtr(veh)) {
									Crosshair3D = PredictMovement(Crosshair3D, veh->v3Velocity, 1.f);
									//Crosshair3D += veh->v3Velocity;
								}
							}
						}



						if (config::get("aimbot", "aim_offset", 0)) {
							lastTargetVector = Vector3(0, 0, 0);
							if ((old_target != current_target)) {
								// reset random vector cuz we renew it anyways

								if (config::get("aimbot", "aim_offset_range", 0)) {
									// if EnableAimOffsetRandomRange use random offset between x and y
									lastTargetVector.x = RandomFloat(config::get("aimbot", "aim_target_offset_range_x_min", 0.f), config::get("aimbot", "aim_target_offset_range_x_max", 0.f));
									lastTargetVector.y = RandomFloat(config::get("aimbot", "aim_target_offset_range_y_min", 0.f), config::get("aimbot", "aim_target_offset_range_y_max", 0.f));
									lastTargetVector.z = RandomFloat(config::get("aimbot", "aim_target_offset_range_z_min", 0.f), config::get("aimbot", "aim_target_offset_range_z_max", 0.f));
								}
							}

							if (config::get("aimbot", "aim_offset_always_renew", 0)) {
								if (config::get("aimbot", "aim_offset_range", 0)) {
									lastTargetVector.x = RandomFloat(config::get("aimbot", "aim_target_offset_range_x_min", 0.f), config::get("aimbot", "aim_target_offset_range_x_max", 0.f));
									lastTargetVector.y = RandomFloat(config::get("aimbot", "aim_target_offset_range_y_min", 0.f), config::get("aimbot", "aim_target_offset_range_y_max", 0.f));
									lastTargetVector.z = RandomFloat(config::get("aimbot", "aim_target_offset_range_z_min", 0.f), config::get("aimbot", "aim_target_offset_range_z_max", 0.f));
								}
							}


							if (!config::get("aimbot", "aim_offset_range", 0) && !config::get("aimbot", "aim_offset_always_renew", 0)) {
								lastTargetVector.x = config::get("aimbot", "aim_target_offset_static_x", 0.f);
								lastTargetVector.y = config::get("aimbot", "aim_target_offset_static_y", 0.f);
								lastTargetVector.z = config::get("aimbot", "aim_target_offset_static_z", 0.f);
							}

							aim3d += lastTargetVector;
						}

						//Vector3 Crosshair3D = cam->m_cam_pos2;

						float dist = get_distance(Crosshair3D, current_target->fPosition);

						Vector3 Out((aim3d.x - Crosshair3D.x) / dist, (aim3d.y - Crosshair3D.y) / dist, (aim3d.z - Crosshair3D.z) / dist);
						Vector3 viewAngles = cam->m_tps_angles;
						//Vector3 viewAngles = cam->m_fps_angles;


						if (config::get("aimbot", "aim_mode", 0) > 0) {
							Vector3 delta = Out - cam->m_tps_angles;
							float newX = 0;//ImLerp(viewAngles.x, Out.x, 1 / 1000.f * config::get("aimbot", "aim_strength", 500.f));
							float newY = 0;//ImLerp(viewAngles.y, Out.y, 1 / 1000.f * config::get("aimbot", "aim_strength", 500.f));
							float newZ = 0;//ImLerp(viewAngles.z, Out.z, 1 / 1000.f * config::get("aimbot", "aim_strength", 500.f));

							if (config::get("aimbot", "aim_mode", 0) == 1) {
								newX = viewAngles.x + (delta.x / (1001.f - config::get("aimbot", "aim_strength", 500.f)));
								newY = viewAngles.y + (delta.y / (1001.f - config::get("aimbot", "aim_strength", 500.f)));
								newZ = viewAngles.z + (delta.z / (1001.f - config::get("aimbot", "aim_strength", 500.f)));
							}
							if (config::get("aimbot", "aim_mode", 0) == 2) {
								newX = viewAngles.x + (delta.x / (1001.f - config::get("aimbot", "aim_strength", 500.f)));
								newY = viewAngles.y + (delta.y / (1001.f - config::get("aimbot", "aim_strength", 500.f)));
							}

							if ((config::get("aimbot", "aim_mode", 0) == 3)) {
								target_aim_angle = aim3d;
							} else {
								if (newX != 0) {
									//cam->m_fps_angles.x = newX;
									cam->m_tps_angles.x = newX;
								}
								if (newY != 0) {
									//cam->m_fps_angles.y = newY;
									cam->m_tps_angles.y = newY;
								}
								if (newZ != 0) {
									//cam->m_fps_angles.z = newZ;
									cam->m_tps_angles.z = newZ;
								}
							}
						} else if (config::get("aimbot", "aim_mode", 0) == 0) {
							//cam->m_fps_angles = Out;
							cam->m_tps_angles = Out;


							//char address[100];
							//std::snprintf(address, sizeof(address), "address %X\n", );
							//logs::add(address);
						}
					}
				}
			}
		}
	}

	void init() {
		cout << "aimbot init" << endl;
		while (Game.running) {
			tick();
			std::this_thread::sleep_for(1ms);
		}
		cout << "exit aimbot" << endl;
	}
}