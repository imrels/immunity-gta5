#pragma once
#include "../imports.h"
#include "../ui_extend.hpp"

namespace game {
	extern int game_ticks;
	extern float tick_ms;
}

char version[20] = "3.1.3";

const char* AimBones[] = {"Head", "Neck", "Body","Closest"};
const char* AimModes[] = {"Raw", "Normal", "Horizontal","Silent (not legit!)"};
const char* ESPHPModes[] = {"Off", "Bar", "Text", "Text Combined"};
const char* ESPBarModes[] = {"Top", "Bottom", "Left", "Right"};
const char* ESPBoxModes[] = {"Border", "Corners", "Filled", "Filled & Corners"};
const char* HackRestoreModes[] = {"Full", "Auto", "On Key"};



const char* GameWeapons[] = {
"weapon_dagger",
"weapon_bat",
"weapon_bottle",
"weapon_crowbar",
"weapon_unarmed",
"weapon_flashlight",
"weapon_golfclub",
"weapon_hammer",
"weapon_hatchet",
"weapon_knuckle",
"weapon_knife",
"weapon_machete",
"weapon_switchblade",
"weapon_nightstick",
"weapon_wrench",
"weapon_battleaxe",
"weapon_poolcue",
"weapon_stone_hatchet",
"weapon_pistol",
"weapon_pistol_mk2",
"weapon_combatpistol",
"weapon_appistol",
"weapon_stungun",
"weapon_pistol50",
"weapon_snspistol",
"weapon_snspistol_mk2",
"weapon_heavypistol",
"weapon_vintagepistol",
"weapon_flaregun",
"weapon_marksmanpistol",
"weapon_revolver",
"weapon_revolver_mk2",
"weapon_doubleaction",
"weapon_raypistol",
"weapon_ceramicpistol",
"weapon_navyrevolver",
"weapon_microsmg",
"weapon_smg",
"weapon_smg_mk2",
"weapon_assaultsmg",
"weapon_combatpdw",
"weapon_machinepistol",
"weapon_minismg",
"weapon_raycarbine",
"weapon_pumpshotgun",
"weapon_pumpshotgun_mk2",
"weapon_sawnoffshotgun",
"weapon_assaultshotgun",
"weapon_bullpupshotgun",
"weapon_musket",
"weapon_heavyshotgun",
"weapon_dbshotgun",
"weapon_autoshotgun",
"weapon_assaultrifle",
"weapon_assaultrifle_mk2",
"weapon_carbinerifle",
"weapon_carbinerifle_mk2",
"weapon_advancedrifle",
"weapon_specialcarbine",
"weapon_specialcarbine_mk2",
"weapon_bullpuprifle",
"weapon_bullpuprifle_mk2",
"weapon_compactrifle",
"weapon_mg",
"weapon_combatmg",
"weapon_combatmg_mk2",
"weapon_gusenberg",
"weapon_sniperrifle",
"weapon_heavysniper",
"weapon_heavysniper_mk2",
"weapon_marksmanrifle",
"weapon_marksmanrifle_mk2",
"weapon_rpg",
"weapon_stinger",
"weapon_grenadelauncher",
"weapon_grenadelauncher_smoke",
"weapon_minigun",
"weapon_firework",
"weapon_railgun",
"weapon_hominglauncher",
"weapon_compactlauncher",
"weapon_rayminigun",
"weapon_grenade",
"weapon_bzgas",
"weapon_smokegrenade",
"weapon_flare",
"weapon_molotov",
"weapon_stickybomb",
"weapon_proxmine",
"weapon_snowball",
"weapon_pipebomb",
"weapon_ball",
"weapon_petrolcan",
"weapon_fireextinguisher",
"weapon_parachute",
"weapon_hazardcan"};



bool BeginCentered(const char* name) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoResize;
	return ImGui::Begin(name, nullptr, flags);
}



ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions;
static int FilterInputToHex(ImGuiInputTextCallbackData* data) {
	if (data->EventChar < 256 && strchr("0x123456789ABCDEF", (char)data->EventChar))
		return 0;

	return 1;
};

struct CustomConstraints // Helper functions to demonstrate programmatic constraints
{
	static void Square(ImGuiSizeCallbackData* data) {
		data->DesiredSize = ImVec2(IM_MAX(data->DesiredSize.x, data->DesiredSize.y), IM_MAX(data->DesiredSize.x, data->DesiredSize.y));
	}
	static void Step(ImGuiSizeCallbackData* data) {
		float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
	}
};

struct ExampleAppLog {
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                AutoScroll;
	bool                ScrollToBottom;

	ExampleAppLog() {
		AutoScroll = true;
		ScrollToBottom = false;
		Clear();
	}

	void    Clear() {
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2) {
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
		if (AutoScroll)
			ScrollToBottom = true;
	}

	void    Draw(const char* title, bool* p_open = NULL) {
		if (!ImGui::Begin(title, p_open)) {
			ImGui::End();
			return;
		}

		// Options menu
		if (ImGui::BeginPopup("Options")) {
			if (ImGui::Checkbox("Auto-scroll", &AutoScroll))
				if (AutoScroll)
					ScrollToBottom = true;
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive()) {
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have a random access on the result on our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
			// especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++) {
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		} else {
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
			// Here we instead demonstrate using the clipper to only process lines that are within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
			// Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
			// Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step()) {
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
	}
};


namespace ui {

	const char* const KeyNames[] = {"Unknown","VK_LBUTTON","VK_RBUTTON","VK_CANCEL","VK_MBUTTON","VK_XBUTTON1","VK_XBUTTON2","Unknown","VK_BACK","VK_TAB","Unknown","Unknown","VK_CLEAR","VK_RETURN","Unknown","Unknown","VK_SHIFT","VK_CONTROL","VK_MENU","VK_PAUSE","VK_CAPITAL","VK_KANA","Unknown","VK_JUNJA","VK_FINAL","VK_KANJI","Unknown","VK_ESCAPE","VK_CONVERT","VK_NONCONVERT","VK_ACCEPT","VK_MODECHANGE","VK_SPACE","VK_PRIOR","VK_NEXT","VK_END","VK_HOME","VK_LEFT","VK_UP","VK_RIGHT","VK_DOWN","VK_SELECT","VK_PRINT","VK_EXECUTE","VK_SNAPSHOT","VK_INSERT","VK_DELETE","VK_HELP","0","1","2","3","4","5","6","7","8","9","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","VK_LWIN","VK_RWIN","VK_APPS","Unknown","VK_SLEEP","VK_NUMPAD0","VK_NUMPAD1","VK_NUMPAD2","VK_NUMPAD3","VK_NUMPAD4","VK_NUMPAD5","VK_NUMPAD6","VK_NUMPAD7","VK_NUMPAD8","VK_NUMPAD9","VK_MULTIPLY","VK_ADD","VK_SEPARATOR","VK_SUBTRACT","VK_DECIMAL","VK_DIVIDE","VK_F1","VK_F2","VK_F3","VK_F4","VK_F5","VK_F6","VK_F7","VK_F8","VK_F9","VK_F10","VK_F11","VK_F12","VK_F13","VK_F14","VK_F15","VK_F16","VK_F17","VK_F18","VK_F19","VK_F20","VK_F21","VK_F22","VK_F23","VK_F24","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","VK_NUMLOCK","VK_SCROLL","VK_OEM_NEC_EQUAL","VK_OEM_FJ_MASSHOU","VK_OEM_FJ_TOUROKU","VK_OEM_FJ_LOYA","VK_OEM_FJ_ROYA","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","VK_LSHIFT","VK_RSHIFT","VK_LCONTROL","VK_RCONTROL","VK_LMENU","VK_RMENU"};
	bool hotkey(const char* label, int* k, const ImVec2& size_arg, ImColor backgroundColor = ImColor(50, 50, 50, 200)) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
		const ImRect frame_bb(ImVec2(label_size.x + style.ItemInnerSpacing.x + window->DC.CursorPos.x, 0.0f + window->DC.CursorPos.y), ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y));
		const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
			return false;

		const bool hovered = ImGui::ItemHoverable(frame_bb, id);

		if (hovered) {
			ImGui::SetHoveredID(id);
			g.MouseCursor = ImGuiMouseCursor_TextInput;
		}

		const bool user_clicked = hovered && io.MouseClicked[0];
		const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);

		if (focus_requested || user_clicked) {
			if (g.ActiveId != id) {
				// Start edition
				memset(io.MouseDown, 0, sizeof(io.MouseDown));
				memset(io.KeysDown, 0, sizeof(io.KeysDown));
				*k = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		} else if (io.MouseClicked[0]) {
			// Release focus when we click outside
			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		bool value_changed = false;
		int key = *k;

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io.MouseDown[i]) {
					switch (i) {
						case 0:
							key = VK_LBUTTON;
							break;
						case 1:
							key = VK_RBUTTON;
							break;
						case 2:
							key = VK_MBUTTON;
							break;
						case 3:
							key = VK_XBUTTON1;
							break;
						case 4:
							key = VK_XBUTTON2;
							break;
					}
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
			if (!value_changed) {
				for (auto i = VK_BACK; i <= VK_RMENU; i++) {
					if (io.KeysDown[i]) {
						key = i;
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}

				if (GetAsyncKeyState(VK_XBUTTON1)) {
					key = VK_XBUTTON1;
					value_changed = true;
					ImGui::ClearActiveID();
				} else if (GetAsyncKeyState(VK_XBUTTON2)) {
					key = VK_XBUTTON2;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}

			if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
				*k = 0;
				ImGui::ClearActiveID();
			} else {
				*k = key;
			}
		}

		// Render
		// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

		char buf_display[64] = "None";

		//ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 1.0f)), true, style.FrameRounding);
		ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, backgroundColor, true, style.FrameRounding);

		if (*k != 0 && g.ActiveId != id) {
			strcpy_s(buf_display, KeyNames[*k]);
		} else if (g.ActiveId == id) {
			strcpy_s(buf_display, "Press any Key");
		}

		const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
		ImVec2 render_pos = ImVec2(frame_bb.Min.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y);
		ImGui::RenderTextClipped(ImVec2(frame_bb.Min.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), ImVec2(frame_bb.Max.x - style.FramePadding.x, frame_bb.Max.y - style.FramePadding.y), buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);

		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

		return value_changed;
	}

	static void Helper(const char* desc) {

		ImGui::SameLine(0.f, 5.f);
		ImGui::Text("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
			ImGui::PopStyleVar();
		}
	}


	static ExampleAppLog DEBUG;
	static void showDebugLog() {

		// For the demo: add a debug button _BEFORE_ the normal log window contents
		// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
		// Most of the contents of the window will be added by the log.Draw() call.
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		DEBUG.Draw("log");
	}


	static char toAddHash[64] = "";
	static char toAddHashVeh[64] = "";
	static char toAddHashSets[128];


	//map<DWORD, string> customObject;


	int menuWidth = 800;
	int menuHeight = 500;

	int currentTab = 4;
	int currentSubTab = 0;
	void changeTab(int i) {
		currentTab = i;
	}
	void changeSubTab(int i) {
		currentSubTab = i;
	}


	void render() {
		ImColor MenuColor1 = ImColor(25, 25, 25, 220);
		ImVec4 MenuColor = ImVec4(MenuColor1.Value.x, MenuColor1.Value.y, MenuColor1.Value.z, MenuColor1.Value.w);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, MenuColor);

		if (config::get("hack", "debug", 0)) {
			showDebugLog();
		}


		bool group_window = config::get("hack", "group_window", 0);
		if (group_window) {
			try {

				map<string, string> dummy_group;
				json e;
				e["g"] = "f";
				dummy_group.insert(make_pair("f", e.dump()));


				ImGui::Begin("Group Editor", &group_window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


				config::update(group_window, "hack", "group_window", 0);


				ImGui::SetWindowSize(ImVec2(500.0f, 300.0f), ImGuiCond_Always);
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

				static char toAddNameSet[128] = "";
				static char toSearchInGroups[128] = "";


				ImGui::PushItemWidth(50.0f);
				ImGui::Text("Style ID");
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(150.0f);
				//toAddHashSets will be filled by clicking
				ImGui::InputText("##CHash", toAddHashSets, 64, ImGuiInputTextFlags_ReadOnly);
				//ImGui::InputText("##CHash", toAddHashSets, 64, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(50.0f);
				ImGui::Text("Name");
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(150.f);
				ImGui::InputText("##Name", toAddNameSet, 128, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();

				ImGui::SameLine();

				if (toAddHashSets != "") {
					if (toAddNameSet != "") {
						if (ImGui::Button("Add", ImVec2(100, 0))) {
							string toAddHashSetsID(toAddHashSets);
							string toAddNameString(toAddNameSet);


							map<string, string> group_list = config::get("hack", "group_list", dummy_group);

							if (group_list.find(toAddHashSetsID) == group_list.end()) {



								json e;
								e["cid"] = toAddHashSetsID;
								e["name"] = toAddNameString;
								e["visible_color_r"] = config::get("visual", "visible_r", 0.f);
								e["visible_color_g"] = config::get("visual", "visible_g", 1.f);
								e["visible_color_b"] = config::get("visual", "visible_b", 0.f);
								e["visible_color_a"] = config::get("visual", "visible_a", 1.f);

								e["invisible_color_r"] = config::get("visual", "invisible_r", 1.f);
								e["invisible_color_g"] = config::get("visual", "invisible_g", 0.f);
								e["invisible_color_b"] = config::get("visual", "invisible_b", 0.f);
								e["invisible_color_a"] = config::get("visual", "invisible_a", 1.f);

								e["ally"] = false;

								group_list.insert(make_pair(toAddHashSetsID, e.dump()));

								config::update(group_list, "hack", "group_list", dummy_group);
							}
						}

					}
				}


				ImGui::PushItemWidth(50.0f);
				ImGui::Text("Search");
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(200.f);
				ImGui::InputText("###Search", toSearchInGroups, 128, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();


				ImGui::Separator();

				ImGui::Columns(1, NULL, false);
				ImGui::BeginChild("##CPedGroups", ImVec2(500.0f, 210.0f), false);
				int iterator = 0;



				map<string, string> group_list = config::get("hack", "group_list", dummy_group);

				map<string, string>::iterator it;
				for (it = group_list.begin(); it != group_list.end(); it++) {


					try {
						string cid = it->first;
						string dataString = it->second;
						if (cid == "") continue;
						if (cid.size() == 0) continue;
						if (dataString == "") continue;
						if (dataString.size() == 0) continue;
						json data = json::parse(dataString);
						string name = data["name"];

						if (toSearchInGroups != "") {
							if (name.find(toSearchInGroups) == std::string::npos) {
								continue;
							}
						}

						bool tRemove = true;
						if (ImGui::CollapsingHeader((name + "##" + to_string(iterator)).c_str(), &tRemove)) {

							ImGui::Indent(5.f);
							ImGui::Text(("Name: " + name).c_str());
							ImGui::Separator();

							ImGui::Text("Visual");

							ImVec4 ColorVisible = ImVec4(data["visible_color_r"], data["visible_color_g"], data["visible_color_b"], data["visible_color_a"]);

							ImGui::ColorEdit4(("##visible" + to_string(iterator)).c_str(), (float*)&ColorVisible, flags);

							data["visible_color_r"] = ColorVisible.x;
							data["visible_color_g"] = ColorVisible.y;
							data["visible_color_b"] = ColorVisible.z;
							data["visible_color_a"] = ColorVisible.w;


							ImGui::SameLine();
							ImGui::Text((name + " Visible").c_str());




							ImVec4 ColorInvisible = ImVec4(data["invisible_color_r"], data["invisible_color_g"], data["invisible_color_b"], data["invisible_color_a"]);

							ImGui::ColorEdit4(("##invisible" + to_string(iterator)).c_str(), (float*)&ColorInvisible, flags);

							data["invisible_color_r"] = ColorInvisible.x;
							data["invisible_color_g"] = ColorInvisible.y;
							data["invisible_color_b"] = ColorInvisible.z;
							data["invisible_color_a"] = ColorInvisible.w;
							ImGui::SameLine();
							ImGui::Text((name + " Invisible").c_str());


							ImGui::Separator();
							ImGui::Text("Aim");
							bool ally = data["ally"];
							ImGui::Checkbox(("Ally##allycheck" + to_string(iterator)).c_str(), (bool*)&ally);
							data["ally"] = ally;

							ImGui::Unindent(5.f);

						}

						if (tRemove == false) {
							if (it != group_list.end()) {
								group_list.erase(it);

								//config::set("hack", "group_list", group_list);
							}
						} else {
							it->second = data.dump();
						}
					} catch (const std::exception& e) {
						printf("error group %s \n", e.what());
					}

					iterator++;
				}

				config::update(group_list, "hack", "group_list", dummy_group);


			} catch (const std::exception& e) {
				printf("err %s", e.what());
			}

			ImGui::EndChild();
			ImGui::End();



		}


		ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight));


		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(enc("##immuntiy"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::PopStyleColor();
		//
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->PathClear();
		drawList->PathLineTo(ImVec2(p.x, p.y));
		drawList->PathLineTo(ImVec2(p.x + 185, p.y));
		drawList->PathLineTo(ImVec2(p.x + 245, p.y + 55));
		drawList->PathLineTo(ImVec2(p.x, p.y + 55));
		drawList->PathFillConvex(ImColor(25, 25, 25, 220));



		drawList->PathClear();
		drawList->PathLineTo(ImVec2(p.x, p.y + 55));
		drawList->PathLineTo(ImVec2(p.x + menuWidth, p.y + 55));
		drawList->PathLineTo(ImVec2(p.x + menuWidth, p.y + menuHeight));
		drawList->PathLineTo(ImVec2(p.x, p.y + menuHeight));
		drawList->PathLineTo(ImVec2(p.x, p.y + 45));


		drawList->PathFillConvex(ImColor(25, 25, 25, 220));

		//drawList->PathStroke(OutlineColor, false, thickness + 2);
		//drawList->PathStroke(color, false, thickness);

		//PathFillConvex(ImU32 col)



		ImGui::SetCursorPos(ImVec2(15, 10));
		ImGui::Image((PVOID)immunity_image, ImVec2(160, 45));
		ImGui::SetCursorPos(ImVec2(190, 0));

		//->AddLine(p, ImVec2(p.x + 190, p.y + 45), IM_COL32(255, 0, 0, 255), 3.0f);

		ImGui::SetCursorPos(ImVec2(0, 65));
		float button_size = 120.f;
		float button_height = 45.f;
		float menuWidth0 = menuWidth - button_size + 5.f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::BeginChild("##child_menu", ImVec2(button_size, menuHeight - 65));

		if (ImmunityUI::Button("Aim", ImVec2(button_size, button_height))) {
			changeTab(0);
		}
		if (ImmunityUI::Button("ESP", ImVec2(button_size, button_height))) {
			changeTab(1);
		}
		//ImGui::SameLine(0.f, 0.f);
		if (ImmunityUI::Button("Misc", ImVec2(button_size, button_height))) {
			changeTab(2);
		}
		//ImGui::SameLine(0.f, 0.f);
		if (ImmunityUI::Button("Hacks", ImVec2(button_size, button_height))) {
			changeTab(3);
		}
		//ImGui::SameLine(0.f, 0.f);
		if (ImmunityUI::Button("Settings", ImVec2(button_size, button_height))) {
			changeTab(4);
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
		//ImGui::SetCursorPos(ImVec2(button_size, 65));
		ImGui::SameLine();

		ImGui::BeginChild("##content_menu", ImVec2(menuWidth0, menuHeight - 65));

		if (currentTab == 0) {
			ImGui::Columns(3, "aimbot", false);
			ImGui::SetColumnWidth(0, menuWidth0 / 3);
			ImGui::SetColumnWidth(1, menuWidth0 / 3);
			ImGui::SetColumnWidth(2, menuWidth0 / 3);
			ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);

			ImGui::Indent(5.f);

			bool cur_enable_aim = config::get("aimbot", "enable", 0);
			ImmunityUI::Checkbox("Enable Aimbot", &cur_enable_aim);
			config::update(cur_enable_aim, "aimbot", "enable", 0);

			bool AimShowAimradius = config::get("aimbot", "show_aim_radius", 0);
			ImmunityUI::Checkbox("Show Aim Radius", &AimShowAimradius);
			config::update(AimShowAimradius, "aimbot", "show_aim_radius", 0);

			ImGui::Text("Aim Radius");
			Helper("Target needs to be within this radius to be viable");
			float cur_aim_radius = config::get("aimbot", "aim_radius", 50.f);

			ImmunityUI::SliderFloatCustom("##Aim Radius", &cur_aim_radius, 20.0f, 200.0f, "%.3f", 1);
			//ImGui::SliderFloat("##Aim Radius", &cur_aim_radius, 20.0f, 200.0f, "%.3f", 1);
			config::update(cur_aim_radius, "aimbot", "aim_radius", 50.f);


			ImGui::Text("Aim Stop Radius");
			Helper("Target needs to be outside of this radius to be viable");
			float aim_stop_radius = config::get("aimbot", "aim_stop_radius", 0.f);
			if (aim_stop_radius > cur_aim_radius) aim_stop_radius = cur_aim_radius;
			ImmunityUI::SliderFloatCustom("##Aim Stop Radius", &aim_stop_radius, 0.0f, cur_aim_radius, "%.3f", 1);
			config::update(aim_stop_radius, "aimbot", "aim_stop_radius", 0.f);



			bool only_visible = config::get("aimbot", "only_visible", 0);
			ImmunityUI::Checkbox("Only Visible", &only_visible);
			config::update(only_visible, "aimbot", "only_visible", 0);




			ImGui::Text("Aim Mode");
			Helper("Mode of aimbot\nRaw = Direct Lockon\nNormal = Normal Smooth aimbot\nHorizontal = Smooth aimbot, but only horizontal");
			int aim_mode = config::get("aimbot", "aim_mode", 0);
			ImGui::Combo("##Aim Mode", &aim_mode, AimModes, IM_ARRAYSIZE(AimModes));
			config::update(aim_mode, "aimbot", "aim_mode", 0);


			if (aim_mode != 0) {
				ImGui::Text("Aim Strength");
				Helper("Strength of the aimbot");
				float aim_strength = config::get("aimbot", "aim_strength", 0.f);
				//ImGui::SliderFloat("##Smooth Strength", &aim_strength, 10.0f, 1200.0f, "%.3f", 1);
				ImmunityUI::SliderFloatCustom("##Smooth Strength", &aim_strength, 1.0f, 1000.0f, "%.3f", 1);
				config::update(aim_strength, "aimbot", "aim_strength", 0.f);
			}


			ImGui::Text("Aim Bone");
			Helper("Bone which the aimbot targets");
			int aim_bone = config::get("aimbot", "aim_bone", 0);
			ImGui::Combo("##Aim Target Bone", &aim_bone, AimBones, IM_ARRAYSIZE(AimBones));
			config::update(aim_bone, "aimbot", "aim_bone", 0);
			//}

			if (aim_mode != 3) {
				int aim_key = config::get("aimbot", "aim_key", 0);
				hotkey("Aim Key", &aim_key, ImVec2(200.f, 25.f));
				config::update(aim_key, "aimbot", "aim_key", 0);
			}


			if (aim_mode == 3) {
				bool silent_aim_auto = config::get("aimbot", "silent_aim_auto", 0);
				ImmunityUI::Checkbox("Auto Fire", &silent_aim_auto);
				config::update(silent_aim_auto, "aimbot", "silent_aim_auto", 0);
			}

			ImGui::NextColumn();
			ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);

			bool aim_movement_prediction = config::get("aimbot", "aim_movement_prediction", 0);
			ImmunityUI::Checkbox("Enable Aim Prediction", &aim_movement_prediction);
			Helper("Aimbot tries to predict the Targets next position (using Velocity vector)");
			config::update(aim_movement_prediction, "aimbot", "aim_movement_prediction", 0);
			if (aim_movement_prediction) {
				ImGui::Text("Prediction Distance");
				Helper("Distance the aimbot tries to predict (good for lag compensation)");

				float aim_movement_prediction_distance = config::get("aimbot", "aim_movement_prediction_distance", 0.f);
				ImmunityUI::SliderFloatCustom("##Prediction distance", &aim_movement_prediction_distance, 0.0f, 0.5f, "%.3f", 1);
				config::update(aim_movement_prediction_distance, "aimbot", "aim_movement_prediction_distance", 0.f);
			}

			ImGui::Text("Target Find Timeout");
			Helper("Time before the Aimbot searches for a new Target (in milliseconds)");
			float target_timeout = config::get("aimbot", "target_timeout", 0.f);
			ImmunityUI::SliderFloatCustom("##Target Find Timeout", &target_timeout, 0.0f, 500.0f, "%.3f", 1);
			config::update(target_timeout, "aimbot", "target_timeout", 0.f);

			bool group_check = config::get("aimbot", "group_check", 0);
			ImmunityUI::Checkbox("Group Check", &group_check);
			Helper("Check if the Target is in a\n specific Group and apply set Group settings");
			config::update(group_check, "aimbot", "group_check", 0);


			if (group_check == true) {

				bool group_window = config::get("hack", "group_window", 0);
				if (ImGui::Button("Edit Groups", ImVec2(140, 0))) {
					group_window = 1;
				}
				config::update(group_window, "hack", "group_window", 0);



				bool aim_only_groups = config::get("aimbot", "aim_only_groups", 0);
				ImmunityUI::Checkbox("Aim only Groups", &aim_only_groups);
				Helper("ONLY aim on targets that are in Groups (and marked as Enemy)");
				config::update(aim_only_groups, "aimbot", "aim_only_groups", 0);
			}


			ImGui::NextColumn();
			ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);

			ImGui::Text("Aim Offset");
			Helper("Offset from the Aimtarget position");
			bool aim_offset = config::get("aimbot", "aim_offset", 0);
			ImmunityUI::Checkbox("Enable Aim Offset", &aim_offset);
			config::update(aim_offset, "aimbot", "aim_offset", 0);
			if (aim_offset) {
				bool aim_offset_range = config::get("aimbot", "aim_offset_range", 0);
				ImmunityUI::Checkbox("Offset in Range", &aim_offset_range);
				Helper("Applies RANDOM Offset\n within a given range per target");
				config::update(aim_offset_range, "aimbot", "aim_offset_range", 0);
				bool aim_offset_always_renew = config::get("aimbot", "aim_offset_always_renew", 0);
				if (aim_offset_range) {
					ImmunityUI::Checkbox("Always renew Offset", &aim_offset_always_renew);
					Helper("Renews the RANDOM Offset every tick\ninstead of every target");
				} else {
					aim_offset_always_renew = 0;
				}
				config::update(aim_offset_always_renew, "aimbot", "aim_offset_always_renew", 0);

				if (!aim_offset_range) {
					ImGui::Text("Aim Offset X");
					float aim_target_offset_static_x = config::get("aimbot", "aim_target_offset_static_x", 0.f);

					ImGui::SliderFloat("##aimoffsetx", &aim_target_offset_static_x, -1.0f, 1.0f, "%.3f", 1);


					config::update(aim_target_offset_static_x, "aimbot", "aim_target_offset_static_x", 0.f);


					ImGui::Text("Aim Offset Y");
					float aim_target_offset_static_y = config::get("aimbot", "aim_target_offset_static_y", 0.f);
					ImGui::SliderFloat("##Aim Offset Y", &aim_target_offset_static_y, -1.0f, 1.0f, "%.3f", 1);
					config::update(aim_target_offset_static_y, "aimbot", "aim_target_offset_static_y", 0.f);


					ImGui::Text("Aim Offset Z");
					float aim_target_offset_static_z = config::get("aimbot", "aim_target_offset_static_z", 0.f);
					ImGui::SliderFloat("##Aim Offset Z", &aim_target_offset_static_z, -1.0f, 1.0f, "%.3f", 1);
					config::update(aim_target_offset_static_z, "aimbot", "aim_target_offset_static_z", 0.f);

				} else {
					ImGui::Text("Aim Offset Range X");
					Helper("Range for the Random Offset (min, max)");

					float aim_target_offset_range_x_min = config::get("aimbot", "aim_target_offset_range_x_min", 0.f);
					float aim_target_offset_range_x_max = config::get("aimbot", "aim_target_offset_range_x_max", 0.f);
					ImGui::DragFloatRange2("##AimOffsetXRange", &aim_target_offset_range_x_min, &aim_target_offset_range_x_max, 0.01f, -1.0f, 1.0f, "Min: %.01f", "Max: %.01f");


					config::update(aim_target_offset_range_x_min, "aimbot", "aim_target_offset_range_x_min", 0.f);
					config::update(aim_target_offset_range_x_max, "aimbot", "aim_target_offset_range_x_max", 0.f);



					ImGui::Text("Aim Offset Range Y");
					Helper("Range for the Random Offset (min, max)");

					float aim_target_offset_range_y_min = config::get("aimbot", "aim_target_offset_range_y_min", 0.f);
					float aim_target_offset_range_y_max = config::get("aimbot", "aim_target_offset_range_y_max", 0.f);
					ImGui::DragFloatRange2("##AimOffsetYRange", &aim_target_offset_range_y_min, &aim_target_offset_range_y_max, 0.01f, -1.0f, 1.0f, "Min: %.01f", "Max: %.01f");


					config::update(aim_target_offset_range_y_min, "aimbot", "aim_target_offset_range_y_min", 0.f);
					config::update(aim_target_offset_range_y_max, "aimbot", "aim_target_offset_range_y_max", 0.f);


					ImGui::Text("Aim Offset Range Z");
					Helper("Range for the Random Offset (min, max)");
					float aim_target_offset_range_z_min = config::get("aimbot", "aim_target_offset_range_z_min", 0.f);
					float aim_target_offset_range_z_max = config::get("aimbot", "aim_target_offset_range_z_max", 0.f);
					ImGui::DragFloatRange2("##AimOffsetZRange", &aim_target_offset_range_z_min, &aim_target_offset_range_z_max, 0.01f, -1.0f, 1.0f, "Min: %.01f", "Max: %.01f");


					config::update(aim_target_offset_range_z_min, "aimbot", "aim_target_offset_range_z_min", 0.f);
					config::update(aim_target_offset_range_z_max, "aimbot", "aim_target_offset_range_z_max", 0.f);
				}
			}
			ImGui::Columns();

			ImGui::Unindent(5.f);
		}
		if (currentTab == 1) {
			// ESP

			ImGui::Columns(2, "visual", false);
			ImGui::SetColumnWidth(0, menuWidth0 / 2);
			ImGui::SetColumnWidth(1, menuWidth0 / 2);
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


			ImGui::Indent(5.f);
			bool visuals_enable = config::get("visual", "enable", 0);
			ImmunityUI::Checkbox("Enable Visuals", &visuals_enable);
			config::update(visuals_enable, "visual", "enable", 0);


			bool toggle_key = config::get("visual", "toggle_key", 0);
			ImmunityUI::Checkbox("Toggle Key", &toggle_key);
			config::update(toggle_key, "visual", "toggle_key", 0);

			if (toggle_key) {
				int toggle_key_num = config::get("visual", "toggle_key_num", 1);
				hotkey("Toggle Key##esp", &toggle_key_num, ImVec2(200.f, 25.f));
				config::update(toggle_key_num, "visual", "toggle_key_num", 1);
			}



			ImColor VisibleColor = ImColor(
				config::get("visual", "visible_r", 0.f),
				config::get("visual", "visible_g", 1.f),
				config::get("visual", "visible_b", 0.f),
				config::get("visual", "visible_a", 1.f)
			);
			ImColor InvisibleColor = ImColor(
				config::get("visual", "invisible_r", 1.f),
				config::get("visual", "invisible_g", 0.f),
				config::get("visual", "invisible_b", 0.f),
				config::get("visual", "invisible_a", 1.f)
			);

			//ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
			ImGui::ColorEdit4("##Player Visible ##1", (float*)&VisibleColor, flags);
			config::update(VisibleColor.Value.x, "visual", "visible_r", 0.f);
			config::update(VisibleColor.Value.y, "visual", "visible_g", 1.f);
			config::update(VisibleColor.Value.z, "visual", "visible_b", 0.f);
			config::update(VisibleColor.Value.w, "visual", "visible_a", 1.f);



			ImGui::SameLine();
			ImGui::Text("Player Visible Color");
			Helper("color the player has when visible");


			ImGui::ColorEdit4("Player Invisible ##1", (float*)&InvisibleColor, flags);


			config::update(InvisibleColor.Value.x, "visual", "invisible_r", 1.f);
			config::update(InvisibleColor.Value.y, "visual", "invisible_g", 0.f);
			config::update(InvisibleColor.Value.z, "visual", "invisible_b", 0.f);
			config::update(InvisibleColor.Value.w, "visual", "invisible_a", 1.f);

			ImGui::SameLine();
			ImGui::Text("Player Invisible Color");
			Helper("color the player has when visible");



			bool draw_aim_dot = config::get("visual", "draw_aim_dot", 0);
			ImmunityUI::Checkbox("Aim Dot", &draw_aim_dot);
			config::update(draw_aim_dot, "visual", "draw_aim_dot", 0);

			bool draw_skeleton = config::get("visual", "draw_skeleton", 0);
			ImmunityUI::Checkbox("Skeleton", &draw_skeleton);
			config::update(draw_skeleton, "visual", "draw_skeleton", 0);

			if (draw_skeleton) {
				ImGui::Text("Skeleton thickness");
				float skeleton_thickness = config::get("visual", "skeleton_thickness", 0.f);
				ImmunityUI::SliderFloatCustom("##SkeletonThickness", &skeleton_thickness, 0.0f, 3.0f, "%.3f", 1);
				config::update(skeleton_thickness, "visual", "skeleton_thickness", 0.f);
			}

			bool draw_healthbar = config::get("visual", "draw_healthbar", 0);
			ImmunityUI::Checkbox("Health Bar", &draw_healthbar);
			config::update(draw_healthbar, "visual", "draw_healthbar", 0);

			if (draw_healthbar) {

				int esp_bar_position = config::get("visual", "esp_bar_position", 0);
				ImGui::Combo("##esp_bar_position", &esp_bar_position, ESPBarModes, IM_ARRAYSIZE(ESPBarModes));
				config::update(esp_bar_position, "visual", "esp_bar_position", 0);

			}


			bool draw_weapons = config::get("visual", "draw_weapons", 0);
			ImmunityUI::Checkbox("Weapons", &draw_weapons);
			config::update(draw_weapons, "visual", "draw_weapons", 0);


			bool draw_distance = config::get("visual", "draw_distance", 0);
			ImmunityUI::Checkbox("Distance", &draw_distance);
			config::update(draw_distance, "visual", "draw_distance", 0);


			bool draw_healthtext = config::get("visual", "draw_healthtext", 0);
			ImmunityUI::Checkbox("Health Text", &draw_healthtext);
			config::update(draw_healthtext, "visual", "draw_healthtext", 0);



			bool draw_box = config::get("visual", "draw_box", 0);
			ImmunityUI::Checkbox("Box", &draw_box);
			config::update(draw_box, "visual", "draw_box", 0);
			if (draw_box) {
				int draw_box_style = config::get("visual", "draw_box_style", 0);
				ImGui::Combo("##draw_box_style", &draw_box_style, ESPBoxModes, IM_ARRAYSIZE(ESPBoxModes));
				config::update(draw_box_style, "visual", "draw_box_style", 0);

				if ((draw_box_style == 2) || (draw_box_style == 3)) {


					ImColor box_fill_color = ImColor(
						config::get("visual", "draw_box_fill_color_r", 0.2f),
						config::get("visual", "draw_box_fill_color_g", 0.2f),
						config::get("visual", "draw_box_fill_color_b", 0.2f),
						config::get("visual", "draw_box_fill_color_a", 0.2f)
					);

					ImGui::ColorEdit4("##BoxFillColor", (float*)&box_fill_color, flags);
					config::update(box_fill_color.Value.x, "visual", "draw_box_fill_color_r", 1.f);
					config::update(box_fill_color.Value.y, "visual", "draw_box_fill_color_g", 0.f);
					config::update(box_fill_color.Value.z, "visual", "draw_box_fill_color_b", 0.f);
					config::update(box_fill_color.Value.w, "visual", "draw_box_fill_color_a", 1.f);


					ImGui::SameLine();
					ImGui::Text("Fill Color");
				}

				ImGui::Text("Box thickness");
				float box_thickness = config::get("visual", "box_thickness", 0.f);
				ImmunityUI::SliderFloatCustom("##boxthickness", &box_thickness, 0.0f, 3.0f, "%.3f", 1);
				config::update(box_thickness, "visual", "box_thickness", 0.f);
			}

			ImGui::NextColumn();
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);

			ImGui::Text("Groups");
			bool display_groups = config::get("visual", "display_groups", 0);
			ImmunityUI::Checkbox("Display Groups", &display_groups);
			config::update(display_groups, "visual", "display_groups", 0);



			bool radar = config::get("visual", "radar", 0);
			ImmunityUI::Checkbox("Enable Radar", &radar);
			config::update(radar, "visual", "radar", 0);
			if (radar) {

				ImGui::Text("Radar Zoom");

				float radar_zoom = config::get("visual", "radar_zoom", 1.f);
				ImmunityUI::SliderFloatCustom("##radar_zoom", &radar_zoom, 0.01f, 3.0f, "%.3f", 1);
				config::update(radar_zoom, "visual", "radar_zoom", 1.f);

			}


			bool visual_crosshair = config::get("visual", "visual_crosshair", 0);
			ImmunityUI::Checkbox("Enable Crosshair", &visual_crosshair);
			config::update(visual_crosshair, "visual", "visual_crosshair", 0);
			if (visual_crosshair) {

				ImColor Crosshair_color = ImColor(
					config::get("visual", "crosshair_color_r", 1.f),
					config::get("visual", "crosshair_color_g", 0.f),
					config::get("visual", "crosshair_color_b", 0.f),
					config::get("visual", "crosshair_color_a", 1.f)
				);

				//ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
				ImGui::ColorEdit4("##CrosshairColor", (float*)&Crosshair_color, flags);
				config::update(Crosshair_color.Value.x, "visual", "crosshair_color_r", 1.f);
				config::update(Crosshair_color.Value.y, "visual", "crosshair_color_g", 0.f);
				config::update(Crosshair_color.Value.z, "visual", "crosshair_color_b", 0.f);
				config::update(Crosshair_color.Value.w, "visual", "crosshair_color_a", 1.f);
				ImGui::SameLine();
				ImGui::Text("Crosshair Color");

				bool visual_crosshair_border = config::get("visual", "visual_crosshair_border", 0);
				ImmunityUI::Checkbox("Enable Crosshair Border", &visual_crosshair_border);
				config::update(visual_crosshair_border, "visual", "visual_crosshair_border", 0);

				ImGui::Text("Crosshair Gap");
				float visual_crosshair_gap = config::get("visual", "visual_crosshair_gap", 0.f);
				ImGui::SliderFloat("##visual_crosshair_gap", &visual_crosshair_gap, 0.0f, 50.0f, "%.3f", 1);
				config::update(visual_crosshair_gap, "visual", "visual_crosshair_gap", 0.f);



				ImGui::Text("Crosshair Thickness");
				float visual_crosshair_thickness = config::get("visual", "visual_crosshair_thickness", 0.f);
				ImGui::SliderFloat("##visual_crosshair_thickness", &visual_crosshair_thickness, 0.0f, 10.0f, "%.3f", 1);
				config::update(visual_crosshair_thickness, "visual", "visual_crosshair_thickness", 0.f);


				ImGui::Text("Crosshair Width");
				float visual_crosshair_width = config::get("visual", "visual_crosshair_width", 0.f);
				ImGui::SliderFloat("##visual_crosshair_width", &visual_crosshair_width, 0.0f, 100.0f, "%.3f", 1);
				config::update(visual_crosshair_width, "visual", "visual_crosshair_width", 0.f);
			}


			bool esp_self = config::get("visual", "esp_self", 0);
			ImmunityUI::Checkbox("draw local player", &esp_self);
			config::update(esp_self, "visual", "esp_self", 0);

			ImGui::Unindent(5.f);






			ImGui::Columns();

		}
		if (currentTab == 2) {
			// MISC

			ImGui::Columns(2, "misc", false);
			ImGui::SetColumnWidth(0, menuWidth0 / 2);
			ImGui::SetColumnWidth(1, menuWidth0 / 2);
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);

			ImGui::Indent(5.f);


			ImGui::Text("Visibility");

			bool misc_include_vehicles = config::get("misc", "misc_include_vehicles", 0);
			ImmunityUI::Checkbox("Include Vehicles in Check", &misc_include_vehicles);
			config::update(misc_include_vehicles, "misc", "misc_include_vehicles", 0);

			bool misc_include_vegetation = config::get("misc", "misc_include_vegetation", 1);
			ImmunityUI::Checkbox("Include Vegetation in Check", &misc_include_vegetation);
			config::update(misc_include_vegetation, "misc", "misc_include_vegetation", 1);

			bool misc_include_objects = config::get("misc", "misc_include_objects", 0);
			ImmunityUI::Checkbox("Include Objects in Check", &misc_include_objects);
			config::update(misc_include_objects, "misc", "misc_include_objects", 0);



			bool visibility_update_enable = config::get("misc", "visibility_update_enable", 0);
			ImGui::Checkbox("Change Update Time", &visibility_update_enable);
			config::update(visibility_update_enable, "misc", "visibility_update_enable", 0);
			if (visibility_update_enable) {
				ImGui::Text("Update Speed");
				int visibility_update_time = config::get("misc", "visibility_update_time", 0);
				ImGui::SliderInt("###Update Speed", &visibility_update_time, 0, 100, "%i");
				config::update(visibility_update_time, "misc", "visibility_update_time", 0);
			}







			ImGui::Text("Groups");
			Helper("Groups can be used to categorize peds by clothing");
			bool group_window = config::get("hack", "group_window", 0.f);
			if (ImGui::Button("Edit Groups", ImVec2(140, 0))) {
				group_window = 1;
			}
			config::update(group_window, "hack", "group_window", 0);





			ImGui::Text("Ped and NPCs");


			bool misc_show_npcs = config::get("misc", "misc_show_npcs", 0);
			ImmunityUI::Checkbox("Include NPCs", &misc_show_npcs);
			config::update(misc_show_npcs, "misc", "misc_show_npcs", 0);

			bool misc_npc_only = config::get("misc", "misc_npc_only", 0);
			ImmunityUI::Checkbox("Only NPCs", &misc_npc_only);
			config::update(misc_npc_only, "misc", "misc_npc_only", 0);


			ImGui::Text("Object ESP");
			bool misc_object_esp = config::get("misc", "object_esp", 0);
			ImmunityUI::Checkbox("Object ESP", &misc_object_esp);
			config::update(misc_object_esp, "misc", "object_esp", 0);

			if (misc_object_esp) {


				ImColor EntityColor = ImColor(
					config::get("misc", "object_esp_color_r", 1.f),
					config::get("misc", "object_esp_color_g", 1.f),
					config::get("misc", "object_esp_color_b", 1.f),
					config::get("misc", "object_esp_color_a", 1.f)
				);

				//ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
				ImGui::ColorEdit4("##OBjectColor##1", (float*)&EntityColor, flags);

				ImGui::SameLine();
				ImGui::Text("Object Color");


				config::update(EntityColor.Value.x, "misc", "object_esp_color_r", 1.f);
				config::update(EntityColor.Value.y, "misc", "object_esp_color_g", 1.f);
				config::update(EntityColor.Value.z, "misc", "object_esp_color_b", 1.f);
				config::update(EntityColor.Value.w, "misc", "object_esp_color_a", 1.f);



				bool misc_object_esp_all = config::get("misc", "object_esp_all", 0);
				ImmunityUI::Checkbox("Show All Objects", &misc_object_esp_all);
				config::update(misc_object_esp_all, "misc", "object_esp_all", 0);



				ImGui::SetNextWindowSize(ImVec2(390.0f, 210.0f));

				ImGui::Begin("Object ESP", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::Indent(5.f);
				ImGui::Columns(2, NULL, false);
				ImGui::Text("Hash");
				ImGui::NextColumn();
				ImGui::Text("Name");
				ImGui::Separator();
				ImGui::Columns();


				ImGui::BeginChild("#717", ImVec2(390.0f, 100.0f));
				ImGui::Columns(2, NULL, false);

				//map<DWORD, string> object_list;
				map<DWORD, string>m;

				map<DWORD, string> object_list = config::get("misc", "object_esp_object_list", m);



				static DWORD selected_object = -1;
				map<DWORD, string>::iterator it;
				for (it = object_list.begin(); it != object_list.end(); it++) {


					DWORD dHash = it->first;
					string dName = it->second;
					char label[32];
					sprintf(label, "0x%X\n", dHash);
					if (ImGui::Selectable(label, selected_object == dHash, ImGuiSelectableFlags_SpanAllColumns)) {
						selected_object = dHash;
					}
					ImGui::NextColumn();
					ImGui::Text(dName.c_str());
					ImGui::NextColumn();

				}

				ImGui::Columns();
				ImGui::EndChild();



				ImGui::Columns(2, NULL, false);

				static char toAddName[64] = "";
				if (ImGui::Button("Add", ImVec2(140, 0))) {
					DWORD toAddInt;
					string toAddNameString1(toAddName);

					sscanf_s(toAddHash, "%x", &toAddInt);



					it = object_list.find(toAddInt);
					if (it == object_list.end()) {

						object_list.insert(make_pair(toAddInt, toAddNameString1));
						config::set("misc", "object_esp_object_list", object_list);
					}

				}

				if (selected_object != -1) {

					char label[32];
					sprintf(label, "Remove 0x%X\n", selected_object);
					if (ImGui::Button(label, ImVec2(140, 0))) {
						it = object_list.find(selected_object);
						if (it != object_list.end()) {

							object_list.erase(it);

							config::set("misc", "object_esp_object_list", object_list);
							selected_object = -1;
						}
					}
				}



				ImGui::NextColumn();


				ImGui::Text("Hash");
				ImGui::SameLine();
				ImGui::PushItemWidth(120.0f);
				ImGui::InputText("##HashObject", toAddHash, 64, ImGuiInputTextFlags_CharsUppercase, FilterInputToHex);
				ImGui::PopItemWidth();

				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::PushItemWidth(120.0f);
				ImGui::InputText("##NameObject", toAddName, 64, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();

				ImGui::Unindent(5.f);
				ImGui::End();
			}



			ImGui::Text("Vehicle ESP");
			bool misc_vehicle_esp = config::get("misc", "misc_vehicle_esp", 0);
			ImmunityUI::Checkbox("Vehicle ESP", &misc_vehicle_esp);
			config::update(misc_vehicle_esp, "misc", "misc_vehicle_esp", 0);

			if (misc_vehicle_esp) {


				ImColor VehicleColor = ImColor(
					config::get("misc", "vehicle_esp_color_r", 1.f),
					config::get("misc", "vehicle_esp_color_g", 1.f),
					config::get("misc", "vehicle_esp_color_b", 1.f),
					config::get("misc", "vehicle_esp_color_a", 1.f)
				);

				//ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
				ImGui::ColorEdit4("##VehicleColor##1", (float*)&VehicleColor, flags);

				ImGui::SameLine();
				ImGui::Text("Vehicle Color");


				config::update(VehicleColor.Value.x, "misc", "vehicle_esp_color_r", 1.f);
				config::update(VehicleColor.Value.y, "misc", "vehicle_esp_color_g", 1.f);
				config::update(VehicleColor.Value.z, "misc", "vehicle_esp_color_b", 1.f);
				config::update(VehicleColor.Value.w, "misc", "vehicle_esp_color_a", 1.f);



				bool misc_vehicle_esp_all = config::get("misc", "vehicle_esp_all", 0);
				ImmunityUI::Checkbox("Show All Vehicles", &misc_vehicle_esp_all);
				config::update(misc_vehicle_esp_all, "misc", "vehicle_esp_all", 0);



				ImGui::SetNextWindowSize(ImVec2(390.0f, 210.0f));

				ImGui::Begin("Vehicle ESP", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::Indent(5.f);
				ImGui::Columns(2, NULL, false);
				ImGui::Text("Hash");
				ImGui::NextColumn();
				ImGui::Text("Name");
				ImGui::Separator();
				ImGui::Columns();


				ImGui::BeginChild("#718", ImVec2(390.0f, 100.0f));
				ImGui::Columns(2, NULL, false);

				//map<DWORD, string> object_list;
				map<DWORD, string>m;

				map<DWORD, string> veh_list = config::get("misc", "vehicle_esp_vehicle_list", m);


				static DWORD selected_vehicle = -1;
				map<DWORD, string>::iterator it;
				for (it = veh_list.begin(); it != veh_list.end(); it++) {


					DWORD dHash = it->first;
					string dName = it->second;
					char label[32];
					sprintf(label, "0x%X\n", dHash);
					if (ImGui::Selectable(label, selected_vehicle == dHash, ImGuiSelectableFlags_SpanAllColumns)) {
						selected_vehicle = dHash;
					}
					ImGui::NextColumn();
					ImGui::Text(dName.c_str());
					ImGui::NextColumn();

				}

				ImGui::Columns();
				ImGui::EndChild();



				ImGui::Columns(2, NULL, false);

				static char toAddNameVeh[64] = "";
				if (ImGui::Button("Add", ImVec2(140, 0))) {
					DWORD toAddInt1;
					string toAddNameString1(toAddNameVeh);

					sscanf_s(toAddHashVeh, "%x", &toAddInt1);



					it = veh_list.find(toAddInt1);
					if (it == veh_list.end()) {

						veh_list.insert(make_pair(toAddInt1, toAddNameString1));
						config::set("misc", "vehicle_esp_vehicle_list", veh_list);
					}

				}

				if (selected_vehicle != -1) {

					char label[32];
					sprintf(label, "Remove 0x%X\n", selected_vehicle);
					if (ImGui::Button(label, ImVec2(140, 0))) {
						it = veh_list.find(selected_vehicle);
						if (it != veh_list.end()) {

							veh_list.erase(it);

							config::set("misc", "vehicle_esp_vehicle_list", veh_list);
							selected_vehicle = -1;
						}
					}
				}



				ImGui::NextColumn();


				ImGui::Text("Hash");
				ImGui::SameLine();
				ImGui::PushItemWidth(120.0f);
				ImGui::InputText("##HashVeh", toAddHashVeh, 64, ImGuiInputTextFlags_CharsUppercase, FilterInputToHex);
				ImGui::PopItemWidth();

				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::PushItemWidth(120.0f);
				ImGui::InputText("##NameVeh", toAddNameVeh, 64, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();

				ImGui::Unindent(5.f);
				ImGui::End();
			}



			ImGui::NextColumn();
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


			//Wasypoints

			ImGui::Text("Waypoints");
			bool waypoints_enable = config::get("misc", "waypoints_enable", 0);
			ImmunityUI::Checkbox("Custom Waypoints", &waypoints_enable);
			config::update(waypoints_enable, "misc", "waypoints_enable", 0);



			if (waypoints_enable) {


				bool waypoints_draw = config::get("misc", "waypoints_draw", 0);
				ImmunityUI::Checkbox("Draw Waypoints", &waypoints_draw);
				config::update(waypoints_draw, "misc", "waypoints_draw", 0);

				ImColor WaypointColor = ImColor(
					config::get("misc", "waypoint_color_r", 1.f),
					config::get("misc", "waypoint_color_g", 1.f),
					config::get("misc", "waypoint_color_b", 1.f),
					config::get("misc", "waypoint_color_a", 1.f)
				);

				//ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
				ImGui::ColorEdit4("##waypointcolor##1", (float*)&WaypointColor, flags);

				ImGui::SameLine();
				ImGui::Text("Waypoint Color");


				config::update(WaypointColor.Value.x, "misc", "waypoint_color_r", 1.f);
				config::update(WaypointColor.Value.y, "misc", "waypoint_color_g", 1.f);
				config::update(WaypointColor.Value.z, "misc", "waypoint_color_b", 1.f);
				config::update(WaypointColor.Value.w, "misc", "waypoint_color_a", 1.f);

				ImGui::SetNextWindowSize(ImVec2(390.0f, 210.0f));

				ImGui::Begin("Waypoints", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::Indent(5.f);
				ImGui::Columns(2, NULL, false);
				ImGui::Text("Name");
				ImGui::NextColumn();
				ImGui::Text("Position");
				ImGui::Separator();
				ImGui::Columns();


				ImGui::BeginChild("#123asd", ImVec2(390.0f, 100.0f));
				ImGui::Columns(2, NULL, false);

				map<string, Vector3>m;
				map<string, Vector3> waypont_list = config::get("misc", "waypoint_list", m);


				static string selected_waypoint = "";
				map<string, Vector3>::iterator it;
				for (it = waypont_list.begin(); it != waypont_list.end(); it++) {


					string name = it->first;
					Vector3 vec = it->second;
					if (ImGui::Selectable(name.c_str(), selected_waypoint == name, ImGuiSelectableFlags_SpanAllColumns)) {
						selected_waypoint = name;
					}
					ImGui::NextColumn();


					ImGui::Text((to_string(vec.x) + "," + to_string(vec.y) + "," + to_string(vec.z)).c_str());
					ImGui::NextColumn();

				}

				ImGui::Columns();
				ImGui::EndChild();



				ImGui::Columns(2, NULL, false);

				static char toAddWaypointName[64] = "";
				if (ImGui::Button("Add", ImVec2(140, 0))) {
					string toAddNameString1(toAddWaypointName);
					if (toAddNameString1 != "") {

						it = waypont_list.find(toAddNameString1);
						if (it == waypont_list.end()) {
							if (IsValidPtr(local.player)) {
								waypont_list.insert(make_pair(toAddNameString1, local.player->fPosition));
								config::set("misc", "waypoint_list", waypont_list);
							}
						}
					}

				}

				if (selected_waypoint != "") {

					char label[32];
					sprintf(label, "Remove %s\n", selected_waypoint);
					if (ImGui::Button(label, ImVec2(140, 0))) {
						it = waypont_list.find(selected_waypoint);
						if (it != waypont_list.end()) {

							waypont_list.erase(it);

							config::set("misc", "waypoint_list", waypont_list);
							selected_waypoint = -1;
						}
					}
				}

				ImGui::NextColumn();

				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::PushItemWidth(120.0f);
				ImGui::InputText("##NameWaypoint", toAddWaypointName, 64, ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PopItemWidth();

				ImGui::PushItemWidth(120.0f);
				if (ImGui::Button("Teleport", ImVec2(140, 0))) {
					if (selected_waypoint != "") {
						it = waypont_list.find(selected_waypoint);
						if (it != waypont_list.end()) {
							Vector3 pos = it->second;
							tick::thread_invoker::queue([pos] {
								native::type::any entity;
								bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
								if (is_in_vehicle) {
									entity = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
								} else {
									entity = native::player::get_player_ped(-1);
								}
								if (entity) {
									native::entity::set_entity_coords_no_offset(entity, pos.x, pos.y, pos.z, true, true, true);
								}
								});
						}

					}
				}
				ImGui::PopItemWidth();




				ImGui::Unindent(5.f);
				ImGui::End();
			}


			ImGui::Text("Anti-Cheat");
			Helper("Bypass specific 'Anti-Cheat' measurements");


			bool ignore_invisible = config::get("misc", "ignore_invisible", 0);
			ImmunityUI::Checkbox("Ignore Invisible Players", &ignore_invisible);
			Helper("Ignore a Player if his 'alpha' is below a threshold");
			config::update(ignore_invisible, "misc", "ignore_invisible", 0);


			bool force_visible = config::get("misc", "force_visible", 0);
			ImmunityUI::Checkbox("Force Visiblity", &force_visible);
			Helper("Set 'alpha' to a specific value, if its below the threshold");
			config::update(force_visible, "misc", "force_visible", 0);
			if (ignore_invisible || force_visible) {
				ImGui::Text("Alpha threshold");
				float alpha_threshold = config::get("misc", "alpha_threshold", 0.f);
				ImGui::SliderFloat("##alpha_threshold", &alpha_threshold, 1.0f, 255.0f, "%.3f", 1);
				config::update(alpha_threshold, "misc", "alpha_threshold", 0.f);
			}


			bool hide_anticheat_npcs = config::get("misc", "hide_anticheat_npcs", 0);
			ImmunityUI::Checkbox("Hide Anticheat NPCs", &hide_anticheat_npcs);
			Helper("Hide NPCs that are trying to irritate you");
			config::update(hide_anticheat_npcs, "misc", "hide_anticheat_npcs", 0);

			bool no_force_exit = config::get("misc", "no_force_exit", 0);
			ImmunityUI::Checkbox("Disable Forceful Vehicle Exit", &no_force_exit);
			Helper("Disables Vehicle Exit Tasks");
			config::update(no_force_exit, "misc", "no_force_exit", 0);


			ImGui::Columns();
		}
		if (currentTab == 3) {
			// Hacks

			ImGui::Indent(5.f);



			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("##taaaab", tab_bar_flags)) {
				if (ImGui::BeginTabItem("General")) {



					ImGui::Columns(2, "##playerst", false);
					ImGui::SetColumnWidth(0, menuWidth0 / 2);
					ImGui::SetColumnWidth(1, menuWidth0 / 2);
					ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);

					ImGui::Text("General Options");

					bool noclip = config::get("hacks", "noclip", 0);
					ImmunityUI::Checkbox("Noclip", &noclip);
					config::update(noclip, "hacks", "noclip", 0);
					bool noclip_toggle_key = config::get("hacks", "noclip_toggle_key", 0);
					if (noclip || noclip_toggle_key) {
						ImmunityUI::Checkbox("Toggle Key", &noclip_toggle_key);
						config::update(noclip_toggle_key, "hacks", "noclip_toggle_key", 0);
						if (noclip_toggle_key) {
							int noclip_toggle_key_num = config::get("hacks", "noclip_toggle_key_num", 1);
							hotkey("Toggle Key##Noclip", &noclip_toggle_key_num, ImVec2(200.f, 25.f));
							config::update(noclip_toggle_key_num, "hacks", "noclip_toggle_key_num", 1);
						}

						ImGui::Text("Noclip Speed");
						float noclip_speed = config::get("hacks", "noclip_speed", 1.f);
						ImmunityUI::SliderFloatCustom("##noclip_speed", &noclip_speed, 0.0f, 10.f, "%.1f", 1);
						config::update(noclip_speed, "hacks", "noclip_speed", 0.f);
					}

					

					ImGui::Text("Teleport");
					if (ImGui::Button("Teleport to Waypoint", ImVec2((menuWidth0 / 2), 25.f))) {
						Vector3 pos;
						tick::thread_invoker::queue([] {
							//set_ped_shoots_at_coord
							native::type::any entity;
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) {
								entity = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
							} else {
								entity = native::player::get_player_ped(-1);
							}
							native::type::any blip = native::ui::get_first_blip_info_id(8);
							if (native::ui::does_blip_exist(blip)) {
								auto tpos = native::ui::get_blip_coords(blip);
								float z = 0.0f;
								native::entity::set_entity_coords_no_offset(entity, tpos.x, tpos.y, tpos.z, true, true, true);

								if (native::gameplay::get_ground_z_for_3d_coord(tpos.x, tpos.y, tpos.z + 1000, &z, false)) {
									native::entity::set_entity_coords_no_offset(entity, tpos.x, tpos.y, z, true, true, true);
								}
							}
							});
					}


					ImGui::NextColumn();
					ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


					ImGui::Columns();


					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Player")) {
					ImGui::Text("Player Options");
					bool godmode = config::get("hacks", "godmode", 0);
					ImmunityUI::Checkbox("Godmode", &godmode);
					config::update(godmode, "hacks", "godmode", 0);

					if (godmode) {
						ImGui::SameLine();

						ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
						int godmode_mode = config::get("hacks", "godmode_mode", 0);
						//const char* HackRestoreModes[] = { "Full", "Auto", "On Key" };
						ImGui::Combo("##godmodeMode", &godmode_mode, HackRestoreModes, IM_ARRAYSIZE(HackRestoreModes));
						config::update(godmode_mode, "hacks", "godmode_mode", 0);
						ImGui::PopItemWidth();


						if (godmode_mode == 0) {
							ImGui::Text("Freeze Health");
							Helper("Freeze health to set value");

							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float godmode_val = config::get("hacks", "godmode_val", 0.f);
							ImmunityUI::SliderFloatCustom("##godmode_val", &godmode_val, 1.0f, 100.f, "%.3f", 1);
							config::update(godmode_val, "hacks", "godmode_val", 0.f);
							ImGui::PopItemWidth();
						}
						if (godmode_mode == 1) {
							ImGui::Text("Heal when below");
							Helper("set health to value when below.");
							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float godmode_val = config::get("hacks", "godmode_val", 0.f);
							ImmunityUI::SliderFloatCustom("##godmode_val", &godmode_val, 1.0f, 100.f, "%.3f", 1);
							config::update(godmode_val, "hacks", "godmode_val", 0.f);
							ImGui::PopItemWidth();
						}
						if (godmode_mode == 2) {
							ImGui::Text("Heal on Key");
							Helper("Set Health to value when Key is pressed");
							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float godmode_val = config::get("hacks", "godmode_val", 0.f);
							ImmunityUI::SliderFloatCustom("##godmode_val", &godmode_val, 1.0f, 100.f, "%.3f", 1);
							config::update(godmode_val, "hacks", "godmode_val", 0.f);
							ImGui::PopItemWidth();

							int heal_key = config::get("hacks", "heal_key", 1);
							hotkey("Heal Key", &heal_key, ImVec2(200.f, 25.f));
							config::update(heal_key, "hacks", "heal_key", 1);
						}
					}


					ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
					bool auto_armor = config::get("hacks", "auto_armor", 0);
					ImmunityUI::Checkbox("Full Armor", &auto_armor);
					config::update(auto_armor, "hacks", "auto_armor", 0);
					ImGui::PopItemWidth();


					if (auto_armor) {
						ImGui::SameLine();

						ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
						int armormode_mode = config::get("hacks", "autoarmor_mode", 0);
						ImGui::Combo("##armormode_mode", &armormode_mode, HackRestoreModes, IM_ARRAYSIZE(HackRestoreModes));
						config::update(armormode_mode, "hacks", "autoarmor_mode", 0);
						ImGui::PopItemWidth();

						if (armormode_mode == 0) {
							ImGui::Text("Freeze Armor");
							Helper("Freeze health to set value");

							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float auto_armor_val = config::get("hacks", "auto_armor_val", 0.f);
							ImmunityUI::SliderFloatCustom("##auto_armor_val", &auto_armor_val, 1.0f, 100.f, "%.3f", 1);
							config::update(auto_armor_val, "hacks", "auto_armor_val", 0.f);
							ImGui::PopItemWidth();
						}
						if (armormode_mode == 1) {
							ImGui::Text("Auto-Armor when below");
							Helper("set armor to value when below.");
							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float auto_armor_val = config::get("hacks", "auto_armor_val", 0.f);
							ImmunityUI::SliderFloatCustom("##auto_armor_val", &auto_armor_val, 1.0f, 100.f, "%.3f", 1);
							config::update(auto_armor_val, "hacks", "auto_armor_val", 0.f);
							ImGui::PopItemWidth();
						}
						if (armormode_mode == 2) {
							ImGui::Text("Heal on Key");
							Helper("Set Health to value when Key is pressed");
							ImGui::PushItemWidth((menuWidth0 / 3) - 20.f);
							float auto_armor_val = config::get("hacks", "auto_armor_val", 0.f);
							ImmunityUI::SliderFloatCustom("##auto_armor_val", &auto_armor_val, 1.0f, 100.f, "%.3f", 1);
							config::update(auto_armor_val, "hacks", "auto_armor_val", 0.f);
							ImGui::PopItemWidth();

							int armor_key = config::get("hacks", "armor_key", 1);
							hotkey("Armor Key", &armor_key, ImVec2(200.f, 25.f));
							config::update(armor_key, "hacks", "armor_key", 1);
						}
					}

					bool custom_fov = config::get("hacks", "custom_fov", 0);
					ImmunityUI::Checkbox("Custom FOV", &custom_fov);
					config::update(custom_fov, "hacks", "custom_fov", 0);

					if (custom_fov) {
						float custom_fov_value = config::get("hacks", "custom_fov_value", 50.f);
						ImmunityUI::SliderFloatCustom("##custom_fov", &custom_fov_value, 50.0f, 120.f, "%.3f", 1);
						config::update(custom_fov_value, "hacks", "custom_fov_value", 0.f);
					}




					bool nocollision = config::get("hacks", "no_collision_ped", 0);
					ImmunityUI::Checkbox("Disable Collision", &nocollision);
					config::update(nocollision, "hacks", "no_collision_ped", 0);

					bool no_ragdoll = config::get("hacks", "no_ragdoll", 0);
					ImmunityUI::Checkbox("No Ragdoll", &no_ragdoll);
					config::update(no_ragdoll, "hacks", "no_ragdoll", 0);

					ImGui::Text("Player Tasks");
					Helper("Clear Tasks to allow you to abort interactions/escape from situations");


					if (ImGui::Button("Clear Tasks", ImVec2((menuWidth0 / 2), 25.f))) {
						Game.allowClearTasks = true;
						tick::thread_invoker::queue([] {
							if (IsValidPtr(local.player)) {

								native::brain::clear_ped_tasks_immediately(pointer_to_handle((uintptr_t)local.player));
								return;
							}
							});
					}
					bool clear_task_key = config::get("hacks", "clear_task_key", 0);
					ImmunityUI::Checkbox("Clear Task Bind", &clear_task_key);
					config::update(clear_task_key, "hacks", "clear_task_key", 0);
					if (clear_task_key) {
						int clear_task_key_key_num = config::get("hacks", "clear_task_key_key_num", 1);
						hotkey("Key##cleartask", &clear_task_key_key_num, ImVec2(200.f, 25.f));
						config::update(clear_task_key_key_num, "hacks", "clear_task_key_key_num", 1);
					}


					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Vehicle")) {
					ImGui::Text("Vehicle Options");

					ImGui::Columns(2, "hacks", false);
					ImGui::SetColumnWidth(0, menuWidth0 / 2);
					ImGui::SetColumnWidth(1, menuWidth0 / 2);
					ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);



					bool vehicle_god = config::get("hacks", "vehicle_godmode", 0);
					ImmunityUI::Checkbox("Godmode", &vehicle_god);
					config::update(vehicle_god, "hacks", "vehicle_godmode", 0);

					bool no_deformations = config::get("hacks", "no_deformations", 0);
					ImmunityUI::Checkbox("No Deformations", &no_deformations);
					config::update(no_deformations, "hacks", "no_deformations", 0);


					bool unlock_nearby = config::get("hacks", "unlock_nearby", 0);
					ImmunityUI::Checkbox("Unlock nearby Vehicles", &unlock_nearby);
					config::update(unlock_nearby, "hacks", "unlock_nearby", 0);

					bool force_engine = config::get("hacks", "force_engine", 0);
					ImmunityUI::Checkbox("Force Engine on", &force_engine);
					config::update(force_engine, "hacks", "force_engine", 0);


					bool no_collision_veh = config::get("hacks", "no_collision_veh", 0);
					ImmunityUI::Checkbox("Disable Collision##veh", &no_collision_veh);
					config::update(no_collision_veh, "hacks", "no_collision_veh", 0);

					bool vehicle_gravity = config::get("hacks", "vehicle_gravity", 0);
					ImmunityUI::Checkbox("Vehicle gravity", &vehicle_gravity);
					config::update(vehicle_gravity, "hacks", "vehicle_gravity", 0);

					if (vehicle_gravity) {
						float vehicle_gravity_val = config::get("hacks", "vehicle_gravity_val", 9.8f);
						ImmunityUI::SliderFloatCustom("##vehicle_gravity_val", &vehicle_gravity_val, 00.0f, 15.f, "%.3f", 1);
						config::update(vehicle_gravity_val, "hacks", "vehicle_gravity_val", 9.8f);
					}
					//}
					bool vehicle_acceleration = config::get("hacks", "vehicle_acceleration", 0);
					ImmunityUI::Checkbox("Vehicle acceleration", &vehicle_acceleration);
					config::update(vehicle_acceleration, "hacks", "vehicle_acceleration", 0);

					if (vehicle_acceleration) {
						float vehicle_acceleration_val = config::get("hacks", "vehicle_acceleration_val", 1.f);
						ImmunityUI::SliderFloatCustom("##vehicle_acceleration_val", &vehicle_acceleration_val, 00.0f, 10.f, "%.3f", 1);
						config::update(vehicle_acceleration_val, "hacks", "vehicle_acceleration_val", 1.f);
					}


					ImGui::NextColumn();
					ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


					bool veh_boost_enable = config::get("hacks", "veh_boost_enabled", 0);
					ImmunityUI::Checkbox("Vehicle Boost", &veh_boost_enable);
					config::update(veh_boost_enable, "hacks", "veh_boost_enabled", 0);
					if (veh_boost_enable) {
						int veh_boost_key = config::get("hacks", "veh_boost_key", 1);
						hotkey("Boost Key##veh_boost_key", &veh_boost_key, ImVec2(200.f, 25.f));
						config::update(veh_boost_key, "hacks", "veh_boost_key", 1);

						ImGui::Text("Boost Speed");
						float veh_boost_speed = config::get("hacks", "veh_boost_speed", 1.f);
						ImmunityUI::SliderFloatCustom("##veh_boost_speed", &veh_boost_speed, 0.0f, 100.f, "%.1f", 1);
						config::update(veh_boost_speed, "hacks", "veh_boost_speed", 0.f);
					}


					if (ImGui::Button("Fix Vehicle", ImVec2((menuWidth0 / 2), 25.f))) {
						tick::thread_invoker::queue([] {
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) {
								native::type::any entity = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
								if (entity) {
									native::vehicle::set_vehicle_fixed(entity);
								}
							}
							});
					}


					if (ImGui::Button("Hijack Vehicle", ImVec2((menuWidth0 / 2), 25.f))) {
						tick::thread_invoker::queue([] {
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) return;
							float max_dist = 9999.f;
							CVehicle* target_veh = nullptr;

							for (int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
								CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));

								if (IsValidPtr(veh)) {
									float dist = get_distance(local.player->fPosition, veh->fPosition);
									if (dist > 1000.f) continue;

									if (dist < max_dist) {
										max_dist = dist;
										target_veh = veh;
									}
								}
							}
							if ((target_veh != nullptr)) {
								if (IsValidPtr(target_veh)) {
									auto veh_handle = pointer_to_handle((uintptr_t)target_veh);
									auto driver = native::vehicle::get_ped_in_vehicle_seat(veh_handle, -1);
									if (driver) {
										native::entity::set_entity_coords_no_offset(driver, 0, 0, 0, false, false, false);
									}
									native::ped::set_ped_into_vehicle(native::player::player_ped_id(), veh_handle, -1);
								}
							}
							}
						);
					}


					if (ImGui::Button("Teleport nearest Vehicle", ImVec2((menuWidth0 / 2), 25.f))) {
						tick::thread_invoker::queue([] {
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) return;

							float max_dist = 9999.f;
							CVehicle* target_veh = nullptr;

							for (int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
								CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));

								if (IsValidPtr(veh)) {
									float dist = get_distance(local.player->fPosition, veh->fPosition);
									if (dist > 1000.f) continue;

									if (dist < max_dist) {
										max_dist = dist;
										target_veh = veh;
									}
								}
							}
							if ((target_veh != nullptr)) {
								if (IsValidPtr(target_veh)) {
									auto veh_handle = pointer_to_handle((uintptr_t)target_veh);
									auto driver = native::vehicle::get_ped_in_vehicle_seat(veh_handle, -1);
									if (driver) {
										native::entity::set_entity_coords_no_offset(driver, 0, 0, 0, false, false, false);
									}
									auto pos = local.player->fPosition;
									native::entity::set_entity_coords_no_offset(veh_handle, pos.x, pos.y, pos.z, false, false, false);
								}
							}
							}
						);
					}


					if (ImGui::Button("Teleport and Hijack Vehicle", ImVec2((menuWidth0 / 2), 25.f))) {
						tick::thread_invoker::queue([] {
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) return;

							float max_dist = 9999.f;
							CVehicle* target_veh = nullptr;

							for (int i = 0; i < Game.ReplayInterface->vehicle_interface->max_vehicles; i++) {
								CVehicle* veh = reinterpret_cast<CVehicle*>(Game.ReplayInterface->vehicle_interface->get_vehicle(i));

								if (IsValidPtr(veh)) {
									float dist = get_distance(local.player->fPosition, veh->fPosition);
									if (dist > 1000.f) continue;

									if (dist < max_dist) {
										max_dist = dist;
										target_veh = veh;
									}
								}
							}
							if ((target_veh != nullptr)) {
								if (IsValidPtr(target_veh)) {
									auto veh_handle = pointer_to_handle((uintptr_t)target_veh);
									auto driver = native::vehicle::get_ped_in_vehicle_seat(veh_handle, -1);
									if (driver) {
										native::entity::set_entity_coords_no_offset(driver, 0, 0, 0, false, false, false);
									}
									auto pos = local.player->fPosition;
									native::entity::set_entity_coords_no_offset(veh_handle, pos.x, pos.y, pos.z, false, false, false);
									native::ped::set_ped_into_vehicle(native::player::player_ped_id(), veh_handle, -1);
								}
							}
							}
						);
					}


					if (ImGui::Button("Clean Vehicle", ImVec2((menuWidth0 / 2), 25.f))) {
						tick::thread_invoker::queue([] {
							//set_ped_shoots_at_coord
							bool is_in_vehicle = native::ped::is_ped_in_any_vehicle(native::player::player_ped_id(), false);
							if (is_in_vehicle) {
								native::type::any entity = native::ped::get_vehicle_ped_is_in(native::player::get_player_ped(-1), false);
								if (entity) {
									native::vehicle::set_vehicle_dirt_level(entity,0.f);
								}
							}
							});
					}


					ImGui::Columns();

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Weapons")) {
					ImGui::Text("Weapons Hacks");
					Helper("Change weapon attributes");

					bool extend_range = config::get("hacks", "extend_range", 0);
					ImmunityUI::Checkbox("Extend Weapon Range", &extend_range);
					config::update(extend_range, "hacks", "extend_range", 0);

					if (extend_range) {
						ImGui::Text("Weapon Range:");
						float extended_range = config::get("hacks", "extended_range", 0.f);
						ImmunityUI::SliderFloatCustom("##extended_range", &extended_range, 10.0f, 1750.f, "%.3f", 1);
						config::update(extended_range, "hacks", "extended_range", 0.f);
					}

					bool remove_recoil = config::get("hacks", "remove_recoil", 0);
					ImmunityUI::Checkbox("Remove Weapon Recoil", &remove_recoil);
					config::update(remove_recoil, "hacks", "remove_recoil", 0);
					if (remove_recoil) {
						ImGui::Text("Recoil (%%):");
						float recoil_val = config::get("hacks", "remove_recoil_val", 0.f);
						ImmunityUI::SliderFloatCustom("##recoil_val", &recoil_val, 0.0f, 100.f, "%.1f", 1);
						config::update(recoil_val, "hacks", "remove_recoil_val", 0.f);
					}




					bool remove_spread = config::get("hacks", "remove_spread", 0);
					ImmunityUI::Checkbox("Remove Weapon Spread", &remove_spread);
					config::update(remove_spread, "hacks", "remove_spread", 0);
					if (remove_spread) {
						ImGui::Text("Spread (%%):");
						float spread_val = config::get("hacks", "remove_spread_val", 0.f);
						ImmunityUI::SliderFloatCustom("##spread_val", &spread_val, 0.0f, 100.f, "%.1f", 1);
						config::update(spread_val, "hacks", "remove_spread_val", 0.f);
					}


					bool weapon_damage = config::get("hacks", "weapon_damage", 0);
					ImmunityUI::Checkbox("Edit Weapon Damage", &weapon_damage);
					config::update(weapon_damage, "hacks", "weapon_damage", 0);

					if (weapon_damage) {
						ImGui::Text("Weapon Damage:");
						float weapon_damage_value = config::get("hacks", "weapon_damage_value", 0.f);
						ImmunityUI::SliderFloatCustom("##weapon_damage_value", &weapon_damage_value, 00.0f, 1000.f, "%.3f", 1);
						config::update(weapon_damage_value, "hacks", "weapon_damage_value", 0.f);
					}



					ImGui::Text("Give Weapons");
					Helper("Gives you a specific weapon directly");

					static int weapon_to_give = -1;

					ImGui::Combo("##toeqweapon", &weapon_to_give, GameWeapons, IM_ARRAYSIZE(GameWeapons));
					static int ammoToGive = 100;
					static bool toEquip = 0;
					if (weapon_to_give > -1) {
						ImGui::Text("Ammo");
						ImGui::SliderInt("##Ammo", &ammoToGive, 1, 9999, "%.1f");
						ImmunityUI::Checkbox("Equip", &toEquip);
					}

					if (ImGui::Button(weapon_to_give != -1 ? ("Give " + (string)GameWeapons[weapon_to_give]).c_str() : "Give None", ImVec2((menuWidth0 / 2), 25.f))) {

						if (weapon_to_give > -1) {
							give_weapon_delayed(pointer_to_handle((uintptr_t)local.player), rage_joaat(GameWeapons[weapon_to_give]), ammoToGive, toEquip);
						}
					}

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::Columns();

			ImGui::Unindent(5.f);
		}
		if (currentTab == 4) {
			ImGui::Indent(5.f);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));


			ImGui::Columns(2, "settings", false);
			ImGui::SetColumnWidth(0, menuWidth0 / 2);
			ImGui::SetColumnWidth(1, menuWidth0 / 2);
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


			ImGui::Text("Max Range");
			float max_range = config::get("hack", "max_range", 1000.f);
			ImmunityUI::SliderFloatCustom("##range", &max_range, 0.0f, 2000.0f, "%.3f", 1);
			config::update(max_range, "hack", "max_range", 1000.f);
			//ImGui::Text("game ticks %i", game::game_ticks);

			int menu_key = config::get("menu", "menu_key", 35);
			hotkey("Menu Key", &menu_key, ImVec2(200.f, 25.f));
			config::update(menu_key, "menu", "menu_key", 35);

			bool wtm = config::get("hack", "watermark", 1);
			ImmunityUI::Checkbox("Watermark", &wtm);
			config::update(wtm, "hack", "watermark", 1);

			bool video_warning = config::get("hack", "screenshot_warning", 1);
			ImmunityUI::Checkbox("Screen Warning", &video_warning);
			config::update(video_warning, "hack", "screenshot_warning", 1);

			bool openOnLoad = config::get("menu", "openOnLoad", 1);
			ImmunityUI::Checkbox("Open Menu on Load", &openOnLoad);
			config::update(openOnLoad, "menu", "openOnLoad", 1);


			int unload_key = config::get("menu", "unload_key", VK_PAUSE);
			hotkey("Unload Key", &unload_key, ImVec2(200.f, 25.f));
			config::update(unload_key, "menu", "unload_key", VK_PAUSE);
			if (ImGui::Button("Unload", ImVec2(100.f, 25.f))) {
				Game.running = false;
			}
			char versionText[100];
			std::snprintf(versionText, sizeof(versionText), "version %s", version);
			ImGui::Text(versionText);

			//ImGui::Text(("build userid:" + (string)to_string(Game.UserID)).c_str());
			//ImGui::Text(("mod:" + (string)(Game.game_mod_base == 0 ? "gta" : Game.game_mod_base == 1 ? "ragemp" : "altv")).c_str());
			//ImGui::Text(("renderer:" + (string)(Game.renderMethod == 0 ? "steam overlay" : Game.renderMethod == 1 ? "discord overlay" : Game.renderMethod == 3 ? "nvidia" : "own (not record save)")).c_str());

			ImGui::NextColumn();
			ImGui::PushItemWidth((menuWidth0 / 2) - 20.f);


			ImGui::Text("Config Manager");
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::BeginChild("#546", ImVec2(380.0f, 100.0f));

			ImGui::Columns(2, NULL, false);
			static string selected_config = "";
			map<string, string>::iterator it;
			for (it = config::config_list.begin(); it != config::config_list.end(); it++) {
				string filename = it->first;
				string path = it->second;
				if (ImGui::Selectable(filename.c_str(), selected_config == path, ImGuiSelectableFlags_SpanAllColumns)) {
					selected_config = path;
				}
				ImGui::NextColumn();
				ImGui::Text(config::active_config == path ? "active" : "");
				ImGui::NextColumn();
			}
			ImGui::Columns();
			ImGui::EndChild();

			if (ImGui::Button("Refresh", ImVec2(((menuWidth0 / 2) - 20.f) / 4, 0))) {
				config::list_configs();
			}
			ImGui::SameLine(0.f, 0.f);
			if (ImGui::Button("Load", ImVec2(((menuWidth0 / 2) - 20.f) / 4, 0))) {
				config::load_config(selected_config);
			}
			ImGui::SameLine(0.f, 0.f);
			static bool show_input_new_modal = false;

			if (show_input_new_modal) {
				if (BeginCentered("New config name")) {
					static char str0[128] = "";
					ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
					if (ImGui::Button("create config", ImVec2(200, 0))) {
						config::new_config(string(str0));
						show_input_new_modal = false;
					}
					ImGui::End();
				}
			}

			if (ImGui::Button("New", ImVec2(((menuWidth0 / 2) - 20.f) / 4, 0))) {
				show_input_new_modal = true;
			}
			ImGui::SameLine(0.f, 0.f);


			static bool show_input_rename_modal = false;

			if (show_input_rename_modal) {
				if (BeginCentered("Rename config to")) {
					static char str0[128] = "";
					ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
					if (ImGui::Button("rename config", ImVec2(200, 0))) {
						config::rename_config(string(str0), selected_config);
						show_input_rename_modal = false;
					}
					ImGui::End();
				}
			}
			if (ImGui::Button("Rename", ImVec2(((menuWidth0 / 2) - 20.f) / 4, 0))) {
				if (selected_config != "") {
					show_input_rename_modal = true;
				}
			}

			bool dbg = config::get("hack", "debug", 0);
			ImGui::Checkbox("Enable Debug Mode", &dbg);
			config::update(dbg, "hack", "debug", 0);
			if (dbg) {

				if (ImGui::Button("Dump Entrypoints", ImVec2(((menuWidth0 / 2) - 20.f) / 4, 0))) {
					native::invoker::dump_natives();
				}
			}



			ImGui::Columns();
			ImGui::Unindent(5.f);
		}


		ImGui::EndChild();

		ImGui::End();

		ImGui::PopStyleColor(1);
	}
}
