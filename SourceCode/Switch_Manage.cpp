#include "Switch_Manage.h"

extern GameLib::Sprite* sprMain;
extern std::vector<Fan>fans;

void Switch_Manage::Init(std::string file_path) {
	if (!std::ifstream(file_path)) return;

	LoadCSV(file_path, switch_map);
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			if (switch_map[y][x] == 0) continue;

			switch_map[y][x]--;
		}
	}
}

void Switch_Manage::Render() {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			if(switch_map[y][x] == 0) continue;

			int offset_x = switch_map[y][x] % 14;
			int offset_y = switch_map[y][x] / 14;

			sprite_render(sprMain, x * 54, y * 54, 1, 1, offset_x * 54, offset_y * 54, 54, 54, 27, 27, 0, 1, 1, 1, 1);
		}
	}
}

int Switch_Manage::GetMapIndexByPosition(VECTOR2 position) {
	int x = position.x / 54;
	int y = position.y / 54;

	return switch_map[y][x];
}

std::tuple<int, int> Switch_Manage::CheckCollision(Object* obj) {
	VECTOR2 left_bottom = { obj->pos.x - obj->pivot.x, obj->pos.y };
	VECTOR2 right_bottom = { obj->pos.x + obj->pivot.x, obj->pos.y };

	VECTOR2 left_up= { obj->pos.x - obj->pivot.x, obj->pos.y - obj->pivot.y };
	VECTOR2 right_up= { obj->pos.x + obj->pivot.x, obj->pos.y - obj->pivot.y };

	if (GetMapIndexByPosition(left_bottom))
		return { left_bottom.x / 54, left_bottom.y / 54 };
	if (GetMapIndexByPosition(right_bottom))
		return { right_bottom.x / 54, left_bottom.y / 54 };
	if (GetMapIndexByPosition(left_up))
		return { left_up.x / 54, left_up.y / 54 };
	if (GetMapIndexByPosition(right_up))
		return { right_up.x / 54, right_up.y / 54 };

	return { -1, -1 };
}

void FanSwitch_Manage::AlternateMode(int x, int y) {
	int index = switch_map[y][x];

	if (index == 10 || index == 52) {
		switch_map[y][x + 1] += 2;
		switch_map[y][x] += 2;
	} else if (index == 12 || index == 54) {
		switch_map[y][x + 1] -= 2;
		switch_map[y][x] -= 2;
	}

	else if (index == 11 || index == 53) {
		switch_map[y][x - 1] += 2;
		switch_map[y][x] += 2;
	}
	else if (index == 13 || index == 55) {
		switch_map[y][x - 1] -= 2;
		switch_map[y][x] -= 2;
	}
	
	
	else if (index == 67 || index == 94) {
		switch_map[y + 1][x] += 2;
		switch_map[y][x] += 2;
	} else if (index == 69 || index == 96) {
		switch_map[y + 1][x] -= 2;
		switch_map[y][x] -= 2;
	}
	
	
	else if (index == 81 || index == 108) {
		switch_map[y - 1][x] += 2;
		switch_map[y][x] += 2;
	} else if (index == 83 || index == 110) {
		switch_map[y - 1][x] -= 2;
		switch_map[y][x] -= 2;
	}
	
}

bool FanSwitch_Manage::GetSwitchMode(int x, int y) {
	int index = switch_map[y][x];

	return index == 10 || index == 11 || index == 52 || index == 53 || index == 67 || index == 81 || index == 94 || index == 108;
}

void FanSwitch_Manage::TriggerSwitch(int x, int y) {
	if (GetSwitchMode(x, y)) {
		for (int _y = 0; _y < MAP_Y; _y++) {
			for (int _x = 0; _x < MAP_X; _x++) {
				if (!GetSwitchMode(_x, _y)) {
					AlternateMode(_x, _y);
				}
			}
		}
		AlternateMode(x, y);
		
		for (auto& it : fans) 
			it.On = !it.On;
	}
}

void ShutterSwitch_Manage::AlternateMode(int x, int y) {
	int index = switch_map[y][x];

	if (index == 24 || index == 38) {
		switch_map[y][x + 1] += 2;
		switch_map[y][x] += 2;
	} else if (index == 25 || index == 39) {
		switch_map[y][x - 1] += 2;
		switch_map[y][x] += 2;
	}

	else if (index == 66 || index == 95) {
		switch_map[y + 1][x] += 2;
		switch_map[y][x] += 2;
	} else if (index == 80 || index == 109) {
		switch_map[y - 1][x] += 2;
		switch_map[y][x] += 2;
	}

}

bool ShutterSwitch_Manage::GetSwitchMode(int x, int y) {
	int index = switch_map[y][x];

	return index == 24 || index == 38 || index == 25 || index == 39 || index == 66 || index == 95 || index == 80 || index == 109;
}

void ShutterSwitch_Manage::TriggerSwitch(int x, int y) {
	if (isActive) return;
	
	if (GetSwitchMode(x, y)) {
		AlternateMode(x, y);
		isActive = true;
	}
}

int ShutterManage::GetMapIndexByPosition(VECTOR2 position) {
	int p_x = std::roundf(position.x / 54);
	int p_y = std::roundf(position.y / 54);

	for (auto it : shutter_map) {
		int s_x = it.position.x / 54;
		int s_y = it.position.y / 54;
	
		if (p_x == s_x && p_y == s_y) {
			return it.index;
		}
	}
	return -1;
}

void ShutterManage::Init(std::string file_path) {
	if (!std::ifstream(file_path)) return;

	int _shutter_map [MAP_Y][MAP_X];

	LoadCSV(file_path, _shutter_map);

	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			if (_shutter_map[y][x] == 0) continue;
			shutter_map.push_back({ {x * 54.0f, y * 54.0f}, --_shutter_map[y][x]});
		}
	}
}

bool ShutterManage::CheckCollision(Object* obj) {
	VECTOR2 left_bottom = { obj->pos.x - obj->pivot.x -1, obj->pos.y };
	VECTOR2 right_bottom = { obj->pos.x + obj->pivot.x + 1, obj->pos.y };

	return GetMapIndexByPosition(left_bottom) != -1 || GetMapIndexByPosition(right_bottom) != -1;
}

void ShutterManage::Active(bool isActive) {
	if (!isActive) return;
	static int tar_y = -1;
	for (auto& it : shutter_map) {
		if (tar_y == -1) tar_y = it.position.y - 54;
		it.position.y += (tar_y - it.position.y) * 0.1f;
	}
}

void ShutterManage::Render() {
	for (auto& it : shutter_map) {
		int offset_x = it.index  % 14;
		int offset_y = it.index  / 14;

		sprite_render(sprMain, it.position.x, it.position.y, 1, 1, offset_x * 54, offset_y * 54, 54, 54, 27, 27, 0, 1, 1, 1, 1);
	}
}