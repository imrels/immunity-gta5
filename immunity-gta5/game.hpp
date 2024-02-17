#pragma once
#include "imports.h"
#include "database.hpp"


namespace game {
	int menu_key = VK_END;
	int game_ticks = 0;
	float tick_ms = 0.f;
	bool visible_peds[1024] = {0};



	DWORD freemode_f = 0x9C9EFFD8;
	DWORD freemode_m = 0x705E61F2;
	bool isValidPlayer(DWORD hash, CObject* player) {
		if(!config::get("misc", "misc_show_npcs", 0) && !config::get("misc", "misc_npc_only", 0)) {
			return hash == freemode_f || hash == freemode_m;
		} else if(config::get("misc", "misc_show_npcs", 0) && !config::get("misc", "misc_npc_only", 0)) {
			return true;
		} else if(config::get("misc", "misc_npc_only", 0)) {
			return hash != freemode_f && hash != freemode_m;
		}
		return false;
	}

	int update_calls = 0;

	void update_visiblity() {
		update_calls ++;
		if (update_calls < config::get("misc", "visibility_update_time", 0) && config::get("misc", "visibility_update_enable", 0)) return;

		if(IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->ped_interface)) {
			if(IsValidPtr(local.player)) {

				DWORD handle = pointer_to_handle((uintptr_t)local.player);

				auto localplayerHand = GetBonePosition(local.player, RIGHT_HAND); // right hand -> 6
				for(int i = 0; i < Game.ReplayInterface->ped_interface->max_peds; i++) {
					auto tmpEntity = reinterpret_cast<CObject*>(Game.ReplayInterface->ped_interface->get_ped(i));
					if(IsValidPtr(tmpEntity)) {
						//cout << "tmpEntity" << endl;

						if(get_distance(local.player->fPosition, tmpEntity->fPosition) > config::get("hack", "max_range", 1000.f)) continue;

						DWORD hash = tmpEntity->ModelInfo()->GetHash();
						if(!game::isValidPlayer(hash, tmpEntity)) continue;


						ImVec2 screen;
						if(!WorldToScreen2(tmpEntity->fPosition, &screen)) continue;
						if(!isW2SValid(screen)) continue;

						PVector3 EmtVecMap(0, 0, 0);
						RawRaycastResult RaycastResultMap;
						PVector3 HitCordMap(0, 0, 0);


						auto tmpEntityHead = GetBonePosition(tmpEntity, HEAD); //HEAD

						DWORD flags = IntersectMap;

						if (config::get("misc", "misc_include_vegetation", 1)) {
							flags |= IntersectVegetation;
						}
						if (config::get("misc", "misc_include_vehicles", 1)) {
							flags |= IntersectVehicles;
						}
						if (config::get("misc", "misc_include_objects", 0)) {
							flags |= IntersectObjects;
						}

						DWORD Raycast = _START_SHAPE_TEST_CAPSULE(PVector3(localplayerHand.x, localplayerHand.y, localplayerHand.z), PVector3(tmpEntityHead.x, tmpEntityHead.y, tmpEntityHead.z), 0.0f, (IntersectOptions)flags, (DWORD)0, 7);
						//DWORD Raycast = _START_SHAPE_TEST_CAPSULE(PVector3(tmpLocalRightHand.x, tmpLocalRightHand.y, tmpLocalRightHand.z), PVector3(tmpEntityHead.x, tmpEntityHead.y, tmpEntityHead.z), 0.0f, (IntersectOptions)flags, (DWORD)handle, 7);

						if (Raycast) {
							_GET_RAYCAST_RESULT(Raycast, &RaycastResultMap.DidHitAnything, &HitCordMap, &EmtVecMap, &RaycastResultMap.HitEntity);

							if (RaycastResultMap.DidHitAnything) {
								visible_peds[i] = false;
							} else {
								visible_peds[i] = true;
							}
						}


					}
				}
			}
		}
		return;
	}

	//std::atomic<int> x = 0;
	//vector<pair<int, map<Bones, Vector3>>> bone_position_list;
	map <int, map<Bones, Vector3>> bone_position_list;
	//map<Bones, Vector3> bone_positions[1000];

	map<int, map<Bones, Vector3>> temp;
	void update_skeleton() {
		if(IsValidPtr(local.player)) {
			//cout << "update_skeleton" << endl;
			if(IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->ped_interface)) {
				temp.clear();
				for(int i = 0; i < Game.ReplayInterface->ped_interface->max_peds; i++) {
					CObject* ped = reinterpret_cast<CObject*>(Game.ReplayInterface->ped_interface->get_ped(i));
					if(IsValidPtr(ped)) {
						if(get_distance(local.player->fPosition, ped->fPosition) > config::get("hack", "max_range", 1000.f)) continue;

						//only update if onscreen


						try {
							ImVec2 screen;
							if(!WorldToScreen(ped->fPosition, &screen)) continue;
							if(isW2SValid(screen)) {
								map<Bones, Vector3> bone_map;

								bone_map[HEAD] = GetBonePosition(ped, HEAD);
								bone_map[NECK] = GetBonePosition(ped, NECK);


								bone_map[RIGHT_UPPER_ARM] = GetBonePosition(ped, RIGHT_UPPER_ARM);
								bone_map[RIGHT_FOREARM] = GetBonePosition(ped, RIGHT_FOREARM);
								bone_map[RIGHT_HAND] = GetBonePosition(ped, RIGHT_HAND);


								bone_map[LEFT_UPPER_ARM] = GetBonePosition(ped, LEFT_UPPER_ARM);
								bone_map[LEFT_FOREARM] = GetBonePosition(ped, LEFT_FOREARM);
								bone_map[LEFT_HAND] = GetBonePosition(ped, LEFT_HAND);


								bone_map[SPINE3] = GetBonePosition(ped, SPINE3);
								bone_map[SPINE2] = GetBonePosition(ped, SPINE2);
								bone_map[SPINE1] = GetBonePosition(ped, SPINE1);
								bone_map[SPINE_ROOT] = GetBonePosition(ped, SPINE_ROOT);


								bone_map[RIGHT_THIGH] = GetBonePosition(ped, RIGHT_THIGH);
								bone_map[RIGHT_CALF] = GetBonePosition(ped, RIGHT_CALF);
								bone_map[RIGHT_FOOT] = GetBonePosition(ped, RIGHT_FOOT);


								bone_map[LEFT_THIGH] = GetBonePosition(ped, LEFT_THIGH);
								bone_map[LEFT_CALF] = GetBonePosition(ped, LEFT_CALF);
								bone_map[LEFT_FOOT] = GetBonePosition(ped, LEFT_FOOT);

								temp[i] = bone_map;
								//temp.insert(make_pair(i, bone_map));
							}
						} catch(const std::exception& e) {

							printf("err skeleton %s\n", e.what());
						}
					}
				}
				bone_position_list.clear();
				bone_position_list = temp;
			}
		}
	}







	void crosshair() {

		RGBA CrosshairColorRGBA = RGBA(
			(float)config::get("visual", "crosshair_color_r", 1.f) * 255,
			(float)config::get("visual", "crosshair_color_g", 1.f) * 255,
			(float)config::get("visual", "crosshair_color_b", 1.f) * 255,
			(float)config::get("visual", "crosshair_color_a", 1.f) * 255
		);

		int ScreenWidth = ImGui::GetIO().DisplaySize.x;
		int ScreenHeight = ImGui::GetIO().DisplaySize.y;

		ImVec2 Center = ImVec2(ScreenWidth / 2, ScreenHeight / 2);

		float gap = config::get("visual", "visual_crosshair_gap", 0.f);
		float thickness = config::get("visual", "visual_crosshair_thickness", 0.f);
		float width = config::get("visual", "visual_crosshair_width", 0.f);
		int border = config::get("visual", "visual_crosshair_border", 0);


		ImVec2 Center_Left = ImVec2(Center.x - gap, Center.y);
		ImVec2 Center_Left_End = ImVec2(Center.x - width, Center.y);
		if(border) {
			renderer.RenderLine(ImVec2(Center_Left.x + 1, Center_Left.y), ImVec2(Center_Left_End.x - 1, Center_Left_End.y), RGBA(0, 0, 0, 255), thickness + 2.0f);

		}
		renderer.RenderLine(Center_Left, Center_Left_End, CrosshairColorRGBA, thickness);


		ImVec2 Center_Right = ImVec2(Center.x + gap, Center.y);
		ImVec2 Center_Right_End = ImVec2(Center.x + width, Center.y);
		if(border) {
			renderer.RenderLine(ImVec2(Center_Right.x - 1, Center_Right.y), ImVec2(Center_Right_End.x + 1, Center_Right_End.y), RGBA(0, 0, 0, 255), thickness + 2.0f);
		}
		renderer.RenderLine(Center_Right, Center_Right_End, CrosshairColorRGBA, thickness);



		ImVec2 Center_Top = ImVec2(Center.x, Center.y - gap);
		ImVec2 Center_Top_End = ImVec2(Center.x, Center.y - width);
		if(border) {

			renderer.RenderLine(ImVec2(Center_Top.x, Center_Top.y + 1), ImVec2(Center_Top_End.x, Center_Top_End.y - 1), RGBA(0, 0, 0, 255), thickness + 2.0f);
		}
		renderer.RenderLine(Center_Top, Center_Top_End, CrosshairColorRGBA, thickness);




		ImVec2 Center_Bottom = ImVec2(Center.x, Center.y + gap);
		ImVec2 Center_Bottom_End = ImVec2(Center.x, Center.y + width);
		if(border) {
			renderer.RenderLine(ImVec2(Center_Bottom.x, Center_Bottom.y - 1), ImVec2(Center_Bottom_End.x, Center_Bottom_End.y + 1), RGBA(0, 0, 0, 255), thickness + 2.0f);
		}
		renderer.RenderLine(Center_Bottom, Center_Bottom_End, CrosshairColorRGBA, thickness);
	}


	void setFOV(float fov) {
		CPlayerAngles* cam = Game.getCam();
		if(IsValidPtr(cam)) {
			cam->m_cam_data->Fov_Zoom = fov;
		}
	}



	vector<pair<CObject*, DataPed>> ped_list;
	vector<CObject*> object_list;
	vector<CVehicle*> vehicle_list;
	vector<CVehicle*> nearby_vehicle_list;


	map<string, string> dummy_group;
	map<DWORD, string> dummy_group1;
	void update_vehicles() {
		if(IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->vehicle_interface)) {
			if(IsValidPtr(local.player)) {
				map<DWORD, string> veh_list_find = config::get("misc", "vehicle_esp_vehicle_list", dummy_group1);
				vector<CVehicle*> new_vehicle_list;
				vector<CVehicle*> new_nearby_vehicle_list;
				for(int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
					CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));

					if(IsValidPtr(veh)) {
						float dist = get_distance(local.player->fPosition, veh->fPosition);
						if(dist > config::get("hack", "max_range", 1000.f)) continue;


						if(dist < 50.f) {
							new_nearby_vehicle_list.push_back(veh);
						}



						DWORD hash = veh->ModelInfo()->GetHash();
						//if (!game::isValidModel(hash)) continue;

						map<DWORD, string>::iterator it = veh_list_find.find(hash);
						if(config::get("misc", "vehicle_esp_all", 0) || (it != veh_list_find.end())) {
							new_vehicle_list.push_back(veh);
						}



					}
				}
				vehicle_list.clear();
				vehicle_list = new_vehicle_list;


				nearby_vehicle_list.clear();
				nearby_vehicle_list = new_nearby_vehicle_list;
			}
		}
	}
	void update_objects() {

		if(IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->object_interface)) {
			if(IsValidPtr(local.player)) {
				map<DWORD, string> object_findlist = config::get("misc", "object_esp_object_list", dummy_group1);
				vector<CObject*> new_obj_list;
				for(int i = 0; i < Game.ReplayInterface->object_interface->iMaxObjects; i++) {
					CObject* obj = reinterpret_cast<CObject*>(Game.ReplayInterface->object_interface->get_object(i));


					if(IsValidPtr(obj)) {
						float dist = get_distance(local.player->fPosition, obj->fPosition);
						if(dist > config::get("hack", "max_range", 1000.f)) continue;

						DWORD hash = obj->ModelInfo()->GetHash();
						//if (!game::isValidModel(hash)) continue;

						map<DWORD, string>::iterator it = object_findlist.find(hash);
						if(config::get("misc", "object_esp_all", 0) ||
							(it != object_findlist.end())
							) {
							new_obj_list.push_back(obj);
						}
					}
				}
				object_list.clear();
				object_list = new_obj_list;
			}
		}
	}
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;
	void update_peds() {
		if(IsValidPtr(Game.ReplayInterface) && IsValidPtr(Game.ReplayInterface->ped_interface)) {
			if(IsValidPtr(local.player)) {
				map<string, string> group_list = config::get("hack", "group_list", dummy_group);
				vector<pair<CObject*, DataPed>> new_list;
				for(int i = 0; i < Game.ReplayInterface->ped_interface->max_peds; i++) {
					CObject* ped = reinterpret_cast<CObject*>(Game.ReplayInterface->ped_interface->get_ped(i));

					if(IsValidPtr(ped)) {
						if(ped->HP <= 0) continue;
						if(!config::get("visual", "esp_self", 0) && local.player == ped) continue;
						if(get_distance(local.player->fPosition, ped->fPosition) > config::get("hack", "max_range", 1000.f)) continue;
						DWORD hash = ped->ModelInfo()->GetHash();
						if(!game::isValidPlayer(hash, ped)) continue;
						if(config::get("misc", "ignore_invisible", 0) && (ped->GetAlpha() < config::get("misc", "alpha_threshold", 0.f))) continue;
						if(config::get("misc", "force_visible", 0)) {
							if(ped->GetAlpha() < config::get("misc", "alpha_threshold", 0.f)) {
								ped->SetAlpha(config::get("misc", "alpha_threshold", 0.f));
							}
						}


						if(config::get("misc", "hide_anticheat_npcs", 0)) {
							auto handle = pointer_to_handle((uintptr_t)ped);
							if (handle) {

								if (native::brain::get_is_task_active(handle, 221) ||
									native::brain::get_is_task_active(handle, 222) ||
									native::brain::get_is_task_active(handle, 241) ||
									native::brain::get_is_task_active(handle, 151) ||
									native::brain::get_is_task_active(handle, 138) ||
									native::brain::get_is_task_active(handle, 100) ||
									native::brain::get_is_task_active(handle, 218) ||
									native::brain::get_is_task_active(handle, 220) ||
									native::brain::get_is_task_active(handle, 34) ||
									native::brain::get_is_task_active(handle, 101)) continue;
							}
						}



						DataPed PedData;
						PedData.index = i;
						PedData.visible = visible_peds[i] || false;


						//Fetch Group
						string group_name = "\0";
						if((config::get("hack", "group_window", 0)) || config::get("visual", "display_groups", 0) || config::get("aimbot", "aim_only_groups", 0)) {
							string componentId = GetPedComponentHash(ped);
							map<string, string>::iterator it = group_list.find(componentId);
							if(it != group_list.end()) {
								string cid = it->first;
								string dataString = it->second;
								if(cid == "") continue;
								if(cid.size() == 0) continue;
								if(dataString == "") continue;
								if(dataString.size() == 0) continue;
								json data = json::parse(dataString);
								string name = data["name"];


								PedData.group.inGroup = true;
								PedData.group.name = name;

								PedData.group.ally = data["ally"];

								if(PedData.visible) {
									PedData.group.color = RGBA(
										(float)data["visible_color_r"] * 255,
										(float)data["visible_color_g"] * 255,
										(float)data["visible_color_b"] * 255,
										(float)data["visible_color_a"] * 255
									);
								} else {
									PedData.group.color = RGBA(
										(float)data["invisible_color_r"] * 255,
										(float)data["invisible_color_g"] * 255,
										(float)data["invisible_color_b"] * 255,
										(float)data["invisible_color_a"] * 255
									);
								}
							}
						}

						if(config::get("aimbot", "enable", 0) || (config::get("visual", "enable", 0) && config::get("visual", "draw_skeleton", 0))) {


							ImVec2 screen;

							if(WorldToScreen2(ped->fPosition, &screen)) {
								if(isW2SValid(screen)) {
									PedData.bones.HEAD = GetBonePosition(ped, HEAD);
									PedData.bones.NECK = GetBonePosition(ped, NECK);


									PedData.bones.RIGHT_UPPER_ARM = GetBonePosition(ped, RIGHT_UPPER_ARM);
									PedData.bones.RIGHT_FOREARM = GetBonePosition(ped, RIGHT_FOREARM);
									PedData.bones.RIGHT_HAND = GetBonePosition(ped, RIGHT_HAND);


									PedData.bones.LEFT_UPPER_ARM = GetBonePosition(ped, LEFT_UPPER_ARM);
									PedData.bones.LEFT_FOREARM = GetBonePosition(ped, LEFT_FOREARM);
									PedData.bones.LEFT_HAND = GetBonePosition(ped, LEFT_HAND);


									PedData.bones.SPINE3 = GetBonePosition(ped, SPINE3);
									PedData.bones.SPINE2 = GetBonePosition(ped, SPINE2);
									PedData.bones.SPINE1 = GetBonePosition(ped, SPINE1);
									PedData.bones.SPINE_ROOT = GetBonePosition(ped, SPINE_ROOT);


									PedData.bones.RIGHT_THIGH = GetBonePosition(ped, RIGHT_THIGH);
									PedData.bones.RIGHT_CALF = GetBonePosition(ped, RIGHT_CALF);
									PedData.bones.RIGHT_FOOT = GetBonePosition(ped, RIGHT_FOOT);


									PedData.bones.LEFT_THIGH = GetBonePosition(ped, LEFT_THIGH);
									PedData.bones.LEFT_CALF = GetBonePosition(ped, LEFT_CALF);
									PedData.bones.LEFT_FOOT = GetBonePosition(ped, LEFT_FOOT);
								}
							}
						}

						new_list.push_back(pair(ped, PedData));
					}
				}
				ped_list.clear();
				ped_list = new_list;
			}
		}

	}


	void tick() {
		if(IsValidPtr(local.player)) {
			//GAME TICK!!
			if(config::get("visual", "enable", 0) || config::get("aimbot", "enable", 0)) {
				update_visiblity();
			}
			if(config::get("visual", "enable", 0) || config::get("aimbot", "enable", 0)) {
				update_peds();
			}

			hacks::game_tick();

			if((config::get("aimbot", "aim_mode", 0) == 3) && config::get("aimbot", "enable", 0)) {
				aimbot::do_silent_aim();
			}

			return;
		}
	}

	void render() {
		if(!Game.running) return;
		if(config::get("hack", "watermark", 1)) {
			renderer.RenderText("immunity gta5", ImVec2(0, 13), 13.f, RGBA(255, 255, 255, 255), false, true);
		}


		if(config::get("hack", "screenshot_warning", 1)) {
			if(Game.renderMethod == 2) {
				renderer.RenderText("The Cheat renders using its own renderer\nIngame screenshots might show it.\nThis warning can be disabled in settings ('Screen Warning')", ImVec2(Game.screen.x / 2, (Game.screen.y / 2) - 50.f), 13.f, RGBA(255, 0, 0, 255), true, true);
				Game.menuOpen = true;
				ui::changeTab(4);
			}
		}



		if(Game.World != nullptr) {
			local.player = Game.World->getLocalPlayer();

			if(local.player) {
				if(config::get("misc", "object_esp", 0)) {
					update_objects();
				}
				if(config::get("misc", "misc_vehicle_esp", 0) || config::get("hacks", "unlock_nearby", 0) || config::get("hacks", "no_collision_veh", 0)) {
					update_vehicles();
				}
				esp::render();
			}
			if(config::get("visual", "visual_crosshair", 0)) {
				crosshair();
			}

			if(config::get("aimbot", "show_aim_radius", 0)) {
				int w = ImGui::GetIO().DisplaySize.x;
				int h = ImGui::GetIO().DisplaySize.y;
				renderer.RenderCircle(ImVec2(w / 2, h / 2), config::get("aimbot", "aim_radius", 50.f), RGBA(0, 255, 0, 150), 1.0f, 50);
				if(config::get("aimbot", "aim_stop_radius", 0.f) > 0.f) {
					renderer.RenderCircle(ImVec2(w / 2, h / 2), config::get("aimbot", "aim_stop_radius", 0.f), RGBA(255, 0, 0, 150), 1.0f, 50);
				}
			}

			if(config::get("hacks", "custom_fov", 0)) {
				setFOV(config::get("hacks", "custom_fov_value", 50.f));
			}

			if(config::get("hacks", "freecam_steal", 0) && config::get("hacks", "freecam_enabled", 0)) {

				ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
				renderer.RenderLine(ImVec2(Center.x - 25.f, Center.y), ImVec2(Center.x + 25.f, Center.y), RGBA(0, 255, 0, 255), 2.0f);
				renderer.RenderLine(ImVec2(Center.x, Center.y - 25.f), ImVec2(Center.x, Center.y + 25.f), RGBA(0, 255, 0, 255), 2.0f);

				if(hacks::freecam::target_entity != 0) {
					renderer.RenderText("[LMB] to Steal", ImVec2(Game.screen.x / 2, (Game.screen.y / 2) + 17), 13.f, RGBA(255, 255, 255, 255), true, true);
				}
			}



		}
		if(Game.menuOpen) {
			ui::render();
		}
	}
}