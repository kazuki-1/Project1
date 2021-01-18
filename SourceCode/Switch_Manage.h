#pragma once
#include "all.h"
#include "Object.h"

#include "Map.h"

class Switch_Manage {
public:
	void Init(std::string file_path);
	void Render();

	std::tuple<int,int> CheckCollision(Object* obj);
	virtual void TriggerSwitch(int x, int y) { }
	virtual void AlternateMode(int x, int y) { }
	virtual bool GetSwitchMode(int x, int y) { }
protected:
	int GetMapIndexByPosition(VECTOR2 position);
	int switch_map[MAP_Y][MAP_X];
};

class FANSwitch_Manage : public Switch_Manage {
public:
	static FANSwitch_Manage* GetInstance() {
		static FANSwitch_Manage instance;
		return &instance;
	}
	
	void TriggerSwitch(int x, int y);
	void AlternateMode(int x, int y);
	bool GetSwitchMode(int x, int y);
};

class ShutterSwitch_Manage : public Switch_Manage {
public:
	static ShutterSwitch_Manage* GetInstance() {
		static ShutterSwitch_Manage instance;
		return &instance;
	}

	void TriggerSwitch(int x, int y);
	void AlternateMode(int x, int y);
	bool GetSwitchMode(int x, int y);

};

class ShutterManage {
public:
	static ShutterManage* GetInstance() {
		static ShutterManage instance;
		return &instance;
	}
	void Init();
	void Active();
};