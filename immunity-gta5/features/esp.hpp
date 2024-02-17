#pragma once
#include "imports.h"


namespace game {
	extern vector<pair<CObject*, DataPed>> ped_list;
	extern vector<CObject*> object_list;
	extern vector<CVehicle*> vehicle_list;
	extern map <int, map<Bones, Vector3>> bone_position_list;

	extern bool isValidPlayer(DWORD hash, CObject* player);
}

namespace esp {
	namespace utils {
		ImVec2 WorldToRadar(Vector3 Location, FLOAT RadarX, FLOAT RadarY, float RadarSize, float RadarZoom) {
			Vector2 Coord(0, 0);
			ImVec2 Return(0, 0);
			CPlayerAngles* cam = Game.getCam();
			if(IsValidPtr(cam) && IsValidPtr(local.player)) {


				float rot = acosf(cam->m_fps_angles.x) * 180.0f / PI;
				if(asinf(cam->m_fps_angles.y) * 180.0f / PI < 0.0f) rot *= -1.0f;

				Vector2 forwardVec(0, 0);
				float ForwardDirection = DirectX::XMConvertToRadians(rot);
				forwardVec.x = cos(ForwardDirection);
				forwardVec.y = sin(ForwardDirection);

				FLOAT CosYaw = forwardVec.x;
				FLOAT SinYaw = forwardVec.y;

				FLOAT DeltaX = Location.x - local.player->fPosition.x;
				FLOAT DeltaY = Location.y - local.player->fPosition.y;

				FLOAT LocationX = (DeltaY * CosYaw - DeltaX * SinYaw) / RadarZoom;
				FLOAT LocationY = (DeltaX * CosYaw + DeltaY * SinYaw) / RadarZoom;

				if(LocationX > RadarSize / 2.0f - 2.5f)
					LocationX = RadarSize / 2.0f - 2.5f;
				else if(LocationX < -(RadarSize / 2.0f - 2.5f))
					LocationX = -(RadarSize / 2.0f - 2.5f);

				if(LocationY > RadarSize / 2.0f - 2.5f)
					LocationY = RadarSize / 2.0f - 2.5f;
				else if(LocationY < -(RadarSize / 2.0f - 2.5f))
					LocationY = -(RadarSize / 2.0f - 2.5f);

				Return.x = -LocationX + RadarX;
				Return.y = -LocationY + RadarY;

				//delete pPlayerAngles;
			}

			return Return;
		}


		void draw_hp_bar(float x, float y, float w, float h, float health, float max, bool armor, bool horizontal = false) {
			if(!max)
				return;

			if(health < 0)
				health = 0;

			float ratio = health / max;
			RGBA col = RGBA(0, 0, 0, 0);
			if(armor == true) {
				col = RGBA(46, 175, 230, 255);//Color
			} else {
				col = RGBA(46, 230, 46, 200);//Color
			}
			if(!horizontal) {

				float step = (w / max);
				float draw = (step * health);

				renderer.RenderRectFilled(ImVec2(x - 1, y - 1), ImVec2(x + w + 2, y + h + 2), RGBA(0, 0, 0, 150), 0.0f, 0);
				renderer.RenderRectFilled(ImVec2(x, y), ImVec2(x + draw, y + h), col, 0.0f, 0);
			} else {

				float step = (h / max);
				float draw = (step * health);

				renderer.RenderRectFilled(ImVec2(x - 1, y - 1), ImVec2(x + w + 2, y + h + 2), RGBA(0, 0, 0, 150), 0.0f, 0);
				renderer.RenderRectFilled(ImVec2(x, y), ImVec2(x + w, y + draw), col, 0.0f, 0);
			}
		}
	}



	//void drawBones(CObject* ped, RGBA color,float thickness) {
	void drawBones(PlayerBones bones, RGBA current_color, float thickness) {

		//game::bone_position_list

		//auto it = std::find_if(game::bone_position_list.begin(), game::bone_position_list.end(), [&index](map<int, map<Bones, Vector3>> element) { return element.first == index; });
		//if (it != game::bone_position_list.end()) {
			//cout << "found entry" << endl;



		Vector3 bone_positions[][2] = {
			{ bones.HEAD, bones.NECK },

			{ bones.NECK,bones.RIGHT_UPPER_ARM },
			{ bones.RIGHT_FOREARM, bones.RIGHT_UPPER_ARM },
			{ bones.RIGHT_FOREARM, bones.RIGHT_HAND },

			{ bones.NECK, bones.LEFT_UPPER_ARM },
			{ bones.LEFT_FOREARM, bones.LEFT_UPPER_ARM },
			{ bones.LEFT_FOREARM, bones.LEFT_HAND },

			{ bones.NECK, bones.SPINE3 },
			{ bones.SPINE3, bones.SPINE2 },
			{ bones.SPINE2, bones.SPINE1 },
			{ bones.SPINE1, bones.SPINE_ROOT },

			{ bones.SPINE_ROOT, bones.RIGHT_THIGH },
			{ bones.RIGHT_THIGH ,bones.RIGHT_CALF},
			{ bones.RIGHT_CALF,bones.RIGHT_FOOT },

			{ bones.SPINE_ROOT, bones.LEFT_THIGH },
			{ bones.LEFT_THIGH,bones.LEFT_CALF },
			{ bones.LEFT_CALF,bones.LEFT_FOOT },

		};

		int arrSize = sizeof(bone_positions) / sizeof(bone_positions[0]);

		for(size_t i = 0; i < arrSize; i++) {
			ImVec2 from_point;
			ImVec2 to_point;
			if(!WorldToScreen(bone_positions[i][0], &from_point)) return;
			if(!WorldToScreen(bone_positions[i][1], &to_point)) return;

			//bool WorldToScreen2(Vector3 WorldPos, ImVec2 * screen)



			renderer.RenderLine(from_point, to_point, current_color, thickness);
		}

		/*Bones bone_positions[][2] = {
			{ HEAD, NECK },

			{ NECK, RIGHT_UPPER_ARM },
			{ RIGHT_FOREARM, RIGHT_UPPER_ARM },
			{ RIGHT_FOREARM, RIGHT_HAND },

			{ NECK, LEFT_UPPER_ARM },
			{ LEFT_FOREARM, LEFT_UPPER_ARM },
			{ LEFT_FOREARM, LEFT_HAND },

			{ NECK, SPINE3 },
			{ SPINE3, SPINE2 },
			{ SPINE2, SPINE1 },
			{ SPINE1, SPINE_ROOT },

			{ SPINE_ROOT, RIGHT_THIGH },
			{ RIGHT_THIGH ,RIGHT_CALF},
			{ RIGHT_CALF,RIGHT_FOOT },

			{ SPINE_ROOT, LEFT_THIGH },
			{ LEFT_THIGH,LEFT_CALF },
			{ LEFT_CALF,LEFT_FOOT },

		};

		int arrSize = sizeof(bone_positions) / sizeof(bone_positions[0]);

		for (size_t i = 0; i < arrSize; i++) {

			ImVec2 bone1 = GetBoneScreenPosition(ped, bone_positions[i][0]);
			ImVec2 bone2 = GetBoneScreenPosition(ped, bone_positions[i][1]);
			if (bone1.x == 0 || bone1.y == 0) continue;
			if (bone2.x == 0 || bone2.y == 0) continue;

			renderer.RenderLine(bone1, bone2, color, thickness);
		}*/

	}

	void draw_player_esp() {
		RGBA visible_color = RGBA(
			config::get("visual", "visible_r", 0.f) * 255,
			config::get("visual", "visible_g", 1.f) * 255,
			config::get("visual", "visible_b", 0.f) * 255,
			config::get("visual", "visible_a", 1.f) * 255
		);
		RGBA invisible_color = RGBA(
			config::get("visual", "invisible_r", 1.f) * 255,
			config::get("visual", "invisible_g", 0.f) * 255,
			config::get("visual", "invisible_b", 0.f) * 255,
			config::get("visual", "invisible_a", 1.f) * 255
		);
		for(pair<CObject*, DataPed>& entity : game::ped_list) {
			CObject* ped = entity.first;
			DataPed data = entity.second;

			if(get_distance(local.player->fPosition, ped->fPosition) > config::get("hack", "max_range", 1000.f)) continue;
			DWORD hash = ped->ModelInfo()->GetHash();
			if(!game::isValidPlayer(hash, ped)) continue;

			auto Distance = get_distance(local.player->fPosition, ped->fPosition);

			RGBA current_color = RGBA(255, 255, 255, 255);
			auto Top = Vector3(ped->fPosition.x, ped->fPosition.y, ped->fPosition.z + 0.85f);
			auto Bottom = Vector3(ped->fPosition.x, ped->fPosition.y, ped->fPosition.z - 1.0f);

			//if(config::get("visual", "draw_skeleton", 0)) {
			//	Top = data.bones.HEAD;
			//	Bottom = data.bones.LEFT_FOOT;
			//}




			ImVec2 top2d;
			ImVec2 bottom2d;


			if(!WorldToScreen(Top, &top2d)) continue;
			if(!WorldToScreen(Bottom, &bottom2d)) continue;

			float BoxHeight = abs(top2d.y - bottom2d.y);


			float BoxWidth = BoxHeight / 2.0f;
			float TextStartY = top2d.y + BoxHeight;
			int textRow = 0;

			if(data.visible) {
				current_color = visible_color;
			} else {
				current_color = invisible_color;
			}
			if((config::get("visual", "display_groups", 0)) && (data.group.name != "\0")) {
				current_color = data.group.color;
			}

			if((config::get("hack", "group_window", 0) && Game.menuOpen)) {
				if(ImGui::IsMouseClicked(0, false)) {
					ImVec2 mPos = ImGui::GetMousePos();
					if((mPos.x >= top2d.x - (BoxWidth / 2.0f)) && (mPos.x <= top2d.x - (BoxWidth / 2.0f) + BoxWidth)) {
						if((mPos.y >= top2d.y) && (mPos.y <= top2d.y + BoxHeight)) {
							strcpy(ui::toAddHashSets, GetPedComponentHash(ped).c_str());
						}
					}
				}

				ImVec2 mPos = ImGui::GetMousePos();
				if((mPos.x >= top2d.x - (BoxWidth / 2.0f)) && (mPos.x <= top2d.x - (BoxWidth / 2.0f) + BoxWidth)) {
					if((mPos.y >= top2d.y) && (mPos.y <= top2d.y + BoxHeight)) {
						renderer.RenderRectFilled(ImVec2(top2d.x - (BoxWidth / 2.0f), top2d.y), ImVec2(top2d.x - (BoxWidth / 2.0f) + BoxWidth, top2d.y + BoxHeight), RGBA(current_color.r, current_color.g, current_color.b, 150), 0.0f, 0);
						renderer.RenderText("Copy Style ID", ImVec2(top2d.x, top2d.y + BoxHeight / 2), 13, RGBA(255, 255, 255, 255), true, true);
					}
				}
			}




			float thickness_box = config::get("visual", "box_thickness", 0.f);
			if((config::get("visual", "display_groups", 0)) && (data.group.name != "\0")) {
				renderer.RenderText("[" + data.group.name + "]", ImVec2(top2d.x, (TextStartY + thickness_box) + (14 * textRow)), 13, current_color, true, true);
				textRow += 1;
			}
			if(config::get("visual", "draw_box", 0)) {
				int box_style = config::get("visual", "draw_box_style", 0);
				if((box_style == 2) || (box_style == 3)) {
					RGBA box_fill_color = RGBA(
						config::get("visual", "draw_box_fill_color_r", 0.2f) * 255,
						config::get("visual", "draw_box_fill_color_g", 0.2f) * 255,
						config::get("visual", "draw_box_fill_color_b", 0.2f) * 255,
						config::get("visual", "draw_box_fill_color_a", 0.2f) * 255
					);

					renderer.RenderRectFilled(ImVec2(top2d.x - (BoxWidth / 2.0f), top2d.y), ImVec2(top2d.x - (BoxWidth / 2.0f) + BoxWidth, top2d.y + BoxHeight), box_fill_color, 0.0f, 0);
				}

				if((box_style == 0) || (box_style == 2)) {
					renderer.RenderRect(ImVec2(top2d.x - (BoxWidth / 2.0f), top2d.y), ImVec2(top2d.x - (BoxWidth / 2.0f) + BoxWidth, top2d.y + BoxHeight), current_color, 0.0f, 0, thickness_box);
				}
				if((box_style == 1) || (box_style == 3)) {
					//renderer.RenderRect(ImVec2(top2d.x - (BoxWidth / 2.0f), top2d.y), ImVec2(top2d.x - (BoxWidth / 2.0f) + BoxWidth, top2d.y + BoxHeight), current_color, 0.0f, 0, thickness_box);

					auto top_left = ImVec2(top2d.x - (BoxWidth / 2.0f), top2d.y);
					auto bottom_right = ImVec2(top2d.x - (BoxWidth / 2.0f) + BoxWidth, top2d.y + BoxHeight);

					//top left corner
					renderer.RenderLine(ImVec2(top_left.x - thickness_box/2,top_left.y), ImVec2(top_left.x + BoxWidth / 4.f, top_left.y), current_color, thickness_box);
					renderer.RenderLine(ImVec2(top_left.x , top_left.y - thickness_box / 2), ImVec2(top_left.x, top_left.y + BoxHeight / 4.f), current_color, thickness_box);



					//bottom left corner
					renderer.RenderLine(ImVec2(top_left.x, top_left.y + BoxHeight + thickness_box / 2), ImVec2(top_left.x, top_left.y + BoxHeight - BoxHeight / 4.f), current_color, thickness_box);
					renderer.RenderLine(ImVec2(top_left.x - thickness_box / 2, top_left.y + BoxHeight), ImVec2(top_left.x + BoxWidth / 4.f, top_left.y + BoxHeight), current_color, thickness_box);



					//top right corner
					renderer.RenderLine(ImVec2(bottom_right.x, bottom_right.y - BoxHeight - thickness_box / 2), ImVec2(bottom_right.x, bottom_right.y - BoxHeight + BoxHeight / 4.f), current_color, thickness_box);
					renderer.RenderLine(ImVec2(bottom_right.x + thickness_box / 2, bottom_right.y - BoxHeight), ImVec2(bottom_right.x - BoxWidth / 4.f, bottom_right.y - BoxHeight), current_color, thickness_box);

					//bottom right corner
					renderer.RenderLine(ImVec2(bottom_right.x, bottom_right.y + thickness_box / 2), ImVec2(bottom_right.x, bottom_right.y - BoxHeight / 4.f), current_color, thickness_box);
					renderer.RenderLine(ImVec2(bottom_right.x + thickness_box / 2, bottom_right.y), ImVec2(bottom_right.x - BoxWidth / 4.f, bottom_right.y), current_color, thickness_box);
				}
			}

			if(config::get("visual", "draw_healthbar", 0)) {



				int bar_mode = config::get("visual", "esp_bar_position", 0);
				if((bar_mode == 0)) {
					// top
					float w_health = 20.0;
					utils::draw_hp_bar(top2d.x - (BoxWidth > w_health?BoxWidth / 2:(w_health / 2)), (top2d.y - thickness_box) - 8.0f, (BoxWidth > w_health?BoxWidth:w_health), 3, ped->HP - 100.0f, ped->MaxHP - 100.0f, false);
					if(ped->GetArmor() > 1.0f) {
						utils::draw_hp_bar(top2d.x - (BoxWidth > w_health?BoxWidth / 2:(w_health / 2)), (top2d.y - thickness_box) - 15.0f, (BoxWidth > w_health?BoxWidth:w_health), 3, ped->GetArmor(), 100.0f, true);
					}
				}
				if((bar_mode == 1)) {
					// bottom

					textRow += 1;
					float w_health = 20.0;
					utils::draw_hp_bar(top2d.x - (BoxWidth > w_health?BoxWidth / 2:(w_health / 2)), (top2d.y + thickness_box + BoxHeight) + 8.0f, (BoxWidth > w_health?BoxWidth:w_health), 3, ped->HP - 100.0f, ped->MaxHP - 100.0f, false);
					if(ped->GetArmor() > 1.0f) {
						textRow += 1;
						utils::draw_hp_bar(top2d.x - (BoxWidth > w_health?BoxWidth / 2:(w_health / 2)), (top2d.y + thickness_box + BoxHeight) + 15.0f, (BoxWidth > w_health?BoxWidth:w_health), 3, ped->GetArmor(), 100.0f, true);
					}
				}
				if(bar_mode == 2) {
					// left
					float w_health = 20.0;
					utils::draw_hp_bar(top2d.x - BoxWidth / 2 - 12.f, top2d.y, 4.f, BoxHeight, ped->HP - 100.0f, ped->MaxHP - 100.0f, false, true);
					if(ped->GetArmor() > 1.0f) {
						utils::draw_hp_bar(top2d.x - BoxWidth / 2 - 24.f, top2d.y, 4.f, BoxHeight, ped->GetArmor(), 100.0f, true, true);
					}
				}
				if(bar_mode == 3) {
					// right
					float w_health = 20.0;
					utils::draw_hp_bar(top2d.x + BoxWidth / 2 + 8.f, top2d.y, 4.f, BoxHeight, ped->HP - 100.0f, ped->MaxHP - 100.0f, false, true);
					if(ped->GetArmor() > 1.0f) {
						utils::draw_hp_bar(top2d.x + BoxWidth / 2 + 20.f, top2d.y, 4.f, BoxHeight, ped->GetArmor(), 100.0f, true, true);
					}
				}


			}

			if(config::get("visual", "draw_skeleton", 0)) {
				drawBones(data.bones, current_color, config::get("visual", "skeleton_thickness", 0.f));
			}
			if(config::get("visual", "draw_healthtext", 0)) {
				renderer.RenderText(("[HP " + std::to_string((int)(ped->HP - 100.0f)) + "]").c_str(), ImVec2(top2d.x, (TextStartY + thickness_box) + (14 * textRow)), 13, current_color, true, true);
				textRow += 1;
				if(ped->GetArmor() > 1.0f) {
					renderer.RenderText(("[A " + std::to_string((int)(ped->GetArmor())) + "]").c_str(), ImVec2(top2d.x, (TextStartY + thickness_box) + (14 * textRow)), 13, current_color, true, true);
					textRow += 1;
				}
			}

			if(config::get("visual", "draw_aim_dot", 0)) {
				ImVec2 aim2d;
				int b = config::get("aimbot", "aim_bone", 0) == 0?0:config::get("aimbot", "aim_bone", 0) == 1?7:8;

				if(!WorldToScreen(ped->get_bone(b), &aim2d)) continue;

				renderer.RenderDot(ImVec2(aim2d.x - 5, aim2d.y - 5), ImVec2(aim2d.x + 5, aim2d.y + 5), current_color, 1.f);
			}




			//float font_size = ImLerp(6.f, 13.f, 1 - (1.f / 500 * Distance));
			//if (font_size > 13.f) font_size = 13.f;

			if(config::get("visual", "draw_distance", 0)) {
				renderer.RenderText(("[" + std::to_string((int)floor(Distance + 0.5)) + "m]").c_str(), ImVec2(top2d.x, (TextStartY + thickness_box) + (textRow * 14)), 13, current_color, true, true);
				//renderer.RenderText(("[" + std::to_string((int)floor(Distance + 0.5)) + "m]").c_str(), ImVec2(top2d.x, (TextStartY + thickness_box) + (textRow * (font_size + 1.f))), font_size, current_color, true, true);
				textRow += 1;
			}
			if(config::get("visual", "draw_weapons", 0)) {
				CWeaponManager* wep = ped->weapon();
				if(IsValidPtr(wep)) {
					if(IsValidPtr(wep->_WeaponInfo)) {
						auto str = wep->_WeaponInfo->GetSzWeaponName();
						string lstr(str, str + strlen(str));
						//if (!config::get("visual", "draw_weapons_unarmed", 0) && lstr == "Unarmed") continue;
						if(lstr == "UNARMED") continue;
						renderer.RenderText(("[" + lstr + "]").c_str(), ImVec2(top2d.x, (TextStartY + thickness_box) + (textRow * 14)), 13, current_color, true, true);
						textRow += 1;

						//delete str;
					}
				}
			}
		}

		return;
	}

	map<DWORD, string>m;
	void draw_objects() {
		RGBA object_color = RGBA(
			config::get("misc", "object_esp_color_r", 1.f) * 255,
			config::get("misc", "object_esp_color_g", 1.f) * 255,
			config::get("misc", "object_esp_color_b", 1.f) * 255,
			config::get("misc", "object_esp_color_a", 1.f) * 255
		);



		ImColor box_fill_color = ImColor(
			config::get("visual", "draw_box_fill_color_r", 0.2f),
			config::get("visual", "draw_box_fill_color_g", 0.2f),
			config::get("visual", "draw_box_fill_color_b", 0.2f),
			config::get("visual", "draw_box_fill_color_a", 1.f)
		);

		map<DWORD, string> object_list = config::get("misc", "object_esp_object_list", m);
		for(CObject*& entity : game::object_list) {
			if(!IsValidPtr(entity)) continue;

			float dist = get_distance(local.player->fPosition, entity->fPosition);
			if(dist > config::get("hack", "max_range", 1000.f)) continue;
			DWORD hash = entity->ModelInfo()->GetHash();
			//if (!game::isValidModel(hash)) continue;

			map<DWORD, string>::iterator it = object_list.find(hash);
			if(config::get("misc", "object_esp_all", 0) ||
				(it != object_list.end())
				) {

				char tHash[32];
				sprintf(tHash, "0x%X\n", hash);
				string name(tHash);

				if(it != object_list.end()) {
					name = it->second;
				}

				ImVec2 object2d;

				if(!WorldToScreen(entity->fPosition, &object2d)) continue;
				if(!isW2SValid(object2d)) continue;

				renderer.RenderText(name, object2d, 13, object_color, true, true);
				renderer.RenderText(("[" + std::to_string((int)floor(dist + 0.5)) + "m]").c_str(), ImVec2(object2d.x, object2d.y + 15), 13, object_color, true, true);

				if(it == object_list.end()) {
					ImVec2 mPos = ImGui::GetMousePos();
					if(ImGui::IsMouseClicked(0, false)) {
						if(screen_distance(object2d.x, object2d.y, mPos.x, mPos.y) < 10) {
							sprintf(ui::toAddHash, "0x%X\n", hash);
							renderer.RenderText("Copied", ImVec2(object2d.x, object2d.y + 30), 13, RGBA(255, 255, 255, 255), true, true);
						}
					} else {
						if(screen_distance(object2d.x, object2d.y, mPos.x, mPos.y) < 20) {
							renderer.RenderText("Copy", ImVec2(object2d.x, object2d.y + 30), 13, RGBA(255, 255, 255, 255), true, true);
						}
					}
				}



			}

		}
	}
	void draw_vehicles() {
		RGBA veh_color = RGBA(
			config::get("misc", "vehicle_esp_color_r", 1.f) * 255,
			config::get("misc", "vehicle_esp_color_g", 1.f) * 255,
			config::get("misc", "vehicle_esp_color_b", 1.f) * 255,
			config::get("misc", "vehicle_esp_color_a", 1.f) * 255
		);
		map<DWORD, string> veh_list = config::get("misc", "vehicle_esp_vehicle_list", m);
		for(CVehicle*& entity : game::vehicle_list) {
			if(!IsValidPtr(entity)) continue;

			float dist = get_distance(local.player->fPosition, entity->fPosition);
			if(dist > config::get("hack", "max_range", 1000.f)) continue;
			DWORD hash = entity->ModelInfo()->GetHash();
			//if (!game::isValidModel(hash)) continue;

			map<DWORD, string>::iterator it = veh_list.find(hash);
			if(config::get("misc", "vehicle_esp_all", 0) ||
				(it != veh_list.end())
				) {

				char tHash[32];
				sprintf(tHash, "0x%X\n", hash);
				string name(tHash);

				if(it != veh_list.end()) {
					name = it->second;
				}

				ImVec2 veh2d;

				if(!WorldToScreen(entity->fPosition, &veh2d)) continue;


				renderer.RenderText(name, veh2d, 13, veh_color, true, true);
				renderer.RenderText(("[" + std::to_string((int)floor(dist + 0.5)) + "m]").c_str(), ImVec2(veh2d.x, veh2d.y + 15), 13, veh_color, true, true);

				if(it == veh_list.end()) {
					ImVec2 mPos = ImGui::GetMousePos();
					if(ImGui::IsMouseClicked(0, false)) {
						if(screen_distance(veh2d.x, veh2d.y, mPos.x, mPos.y) < 10) {
							sprintf(ui::toAddHashVeh, "0x%X\n", hash);
							renderer.RenderText("Copied", ImVec2(veh2d.x, veh2d.y + 30), 13, RGBA(255, 255, 255, 255), true, true);
						}
					} else {
						if(screen_distance(veh2d.x, veh2d.y, mPos.x, mPos.y) < 20) {
							renderer.RenderText("Copy", ImVec2(veh2d.x, veh2d.y + 30), 13, RGBA(255, 255, 255, 255), true, true);
						}
					}
				}
			}
		}

	}


	void radar() {
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);
		ImGui::Begin("Radar", NULL, ImGuiWindowFlags_NoCollapse);


		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 vSizes = ImGui::GetWindowSize();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		ImVec2 Center = ImVec2(vMin.x + (vSizes.x / 2), vMin.y + (vSizes.y / 2));


		renderer.RenderLine(ImVec2(Center.x, vMin.y), ImVec2(Center.x, vMax.y), RGBA(0, 0, 0, 255), 1.0f);
		renderer.RenderLine(ImVec2(vMin.x, Center.y), ImVec2(vMax.x, Center.y), RGBA(0, 0, 0, 255), 1.0f);




		RGBA visible_color = RGBA(
			config::get("visual", "visible_r", 0.f) * 255,
			config::get("visual", "visible_g", 1.f) * 255,
			config::get("visual", "visible_b", 0.f) * 255,
			config::get("visual", "visible_a", 1.f) * 255
		);
		RGBA invisible_color = RGBA(
			config::get("visual", "invisible_r", 1.f) * 255,
			config::get("visual", "invisible_g", 0.f) * 255,
			config::get("visual", "invisible_b", 0.f) * 255,
			config::get("visual", "invisible_a", 1.f) * 255
		);


		for(pair<CObject*, DataPed>& entity : game::ped_list) {
			CObject* ped = entity.first;
			DataPed data = entity.second;
			if(local.player == ped) continue;
			if(get_distance(local.player->fPosition, ped->fPosition) > config::get("hack", "max_range", 1000.f)) continue;
			DWORD hash = ped->ModelInfo()->GetHash();
			if(!game::isValidPlayer(hash, ped)) continue;

			RGBA current_color = RGBA(255, 255, 255, 255);

			if(data.visible) {
				current_color = visible_color;
			} else {
				current_color = invisible_color;
			}
			if(data.group.name != "\0") {
				current_color = data.group.color;
			}
			ImVec2 RadarPos = utils::WorldToRadar(ped->fPosition, Center.x, Center.y, vSizes.x, config::get("visual", "radar_zoom", 1.f));
			renderer.RenderCircleFilled(RadarPos, 5.f, current_color, 16.f);


		}

		ImGui::End();
	}


	map<string, Vector3>waypoint_cache;
	void draw_waypoints() {
		if(!IsValidPtr(local.player)) return;


		map<string, Vector3> waypoint_list = config::get("misc", "waypoint_list", waypoint_cache);


		RGBA color = RGBA(
			config::get("misc", "waypoint_color_r", 1.f) * 255,
			config::get("misc", "waypoint_color_g", 1.f) * 255,
			config::get("misc", "waypoint_color_b", 1.f) * 255,
			config::get("misc", "waypoint_color_a", 1.f) * 255
		);



		map<string, Vector3>::iterator it;
		for(it = waypoint_list.begin(); it != waypoint_list.end(); it++) {


			string name = it->first;
			Vector3 pos = it->second;


			ImVec2 waypoint2d;
			if(!WorldToScreen(pos, &waypoint2d)) continue;
			if(!isW2SValid(waypoint2d)) continue;

			float dist = get_distance(local.player->fPosition, pos);
			renderer.RenderText(name, waypoint2d, 13, color, true, true);

			renderer.RenderText(("[" + std::to_string((int)floor(dist + 0.5)) + "m]").c_str(), ImVec2(waypoint2d.x, waypoint2d.y + 15), 13, color, true, true);



		}


	}

	void render() {
		//crashing after reinject

		if(config::get("visual", "enable", 0)) {
			draw_player_esp();
		}
		if(config::get("misc", "object_esp", 0)) {
			draw_objects();
		}
		if(config::get("misc", "misc_vehicle_esp", 0)) {
			draw_vehicles();
		}
		if(config::get("visual", "radar", 0)) {
			radar();
		}

		if(config::get("misc", "waypoints_enable", 0) && config::get("misc", "waypoints_draw", 0)) {
			draw_waypoints();

		}
		/*Vector3 localHead = GetBonePosition(local.player, SKEL_Head);
		renderer.RenderText(("X"+ to_string(localHead.x)), ImVec2(150, 200), 13.f, RGBA(255, 255, 255, 255), false, true);
		renderer.RenderText(("Y"+ to_string(localHead.y)), ImVec2(150, 220), 13.f, RGBA(255, 255, 255, 255), false, true);
		renderer.RenderText(("Z"+ to_string(localHead.z)), ImVec2(150, 240), 13.f, RGBA(255, 255, 255, 255), false, true);
		*/

	}
}