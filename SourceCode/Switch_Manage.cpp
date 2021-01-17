#include "Switch_Manage.h"

extern GameLib::Sprite* sprTest;

void Switch_Manage::Init(std::string file_path) {
	LoadCSV(file_path, switch_map);
}

void Switch_Manage::Render() {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			if(switch_map[y][x] == -1) continue;

			int offset_x = switch_map[y][x] % 12;
			int offset_y = switch_map[y][x] / 12;

			sprite_render(sprTest, x * 54, y * 54, 1, 1, offset_x, offset_y, 54, 54, 27, 27, 0, 1, 1, 1, 1);
		}
	}
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

void Switch_Manage::AlternateMode(int x, int y) {
	int index = switch_map[y][x];

	if (index == 10 || index == 34) {
		switch_map[y][x + 1] += 12;
		switch_map[y][x] += 12;
	} else if(index == 22 || index == 46) {
		switch_map[y][x + 1] -= 12;
		switch_map[y][x] -= 12;
	} else if (index == 11 || index == 35) {
		switch_map[y][x - 1] += 12;
		switch_map[y][x] += 12;
	} else if (index == 23 || index == 47) {
		switch_map[y][x + 1] -= 12;
		switch_map[y][x] -= 12;
	}
	
	
	else if (index == 59 || index == 83) {
		switch_map[y + 1][x] -= 1;
		switch_map[y][x] -= 1;
	} else if (index == 58 || index == 82) {
		switch_map[y + 1][x] += 1;
		switch_map[y][x] += 1;
	} else if (index == 71 || index == 95) {
		switch_map[y - 1][x] -= 1;
		switch_map[y][x] -= 1;
	} else if (index == 70 || index == 94) {
		switch_map[y - 1][x] += 1;
		switch_map[y][x] += 1;
	} 
	
}

bool Switch_Manage::GetSwitchMode(int x, int y) {
	int index = switch_map[y][x];

	return index == 10 || index == 11 || index == 46 || index == 47 || index == 59 || index == 71 || index == 82 || index == 94;
}

void Switch_Manage::TriggerSwitch(int x, int y) {
	if (GetSwitchMode(x, y)) {
		for (int _y = 0; _y < MAP_Y; _y++) {
			for (int _x = 0; _x < MAP_X; _x++) {
				if (!GetSwitchMode(_x, _y)) {
					AlternateMode(_x, _y);
				}
			}
		}
		AlternateMode(x, y);
	}
}