#pragma once
#include "all.h"
#include "Object.h"

#include "Map.h"

class Switch_Manage {
public:
	static Switch_Manage* GetInstance() {
		static Switch_Manage instance;
		return &instance;
	}

	void Init(std::string file_path);
	void Render();

	std::tuple<int,int> CheckCollision(Object* obj);
	void TriggerSwitch(int x, int y);
	void AlternateMode(int x, int y);
	bool GetSwitchMode(int x, int y);
private:
	int GetMapIndexByPosition(VECTOR2 position);
	int switch_map[MAP_Y][MAP_X];
};