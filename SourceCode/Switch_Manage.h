#pragma once
#include "all.h"

#include "Object.h"
#include "Map.h"

class Switch_Manage {
public:
	virtual void Init(std::string file_path);
	void Render();

	std::tuple<int,int> CheckCollision(Object* obj);
	virtual void TriggerSwitch(int x, int y) { }
	virtual void AlternateMode(int x, int y) { }
	virtual bool GetSwitchMode(int x, int y) { return false; }
protected:
	int GetMapIndexByPosition(VECTOR2 position);
	int switch_map[MAP_Y][MAP_X];
};

class FanSwitch_Manage : public Switch_Manage {
public:
	static FanSwitch_Manage* GetInstance() {
		static FanSwitch_Manage instance;
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

	void Init(std::string file_path) { 
		isActive = false;
		Switch_Manage::Init(file_path);  
	}

	void TriggerSwitch(int x, int y);
	void AlternateMode(int x, int y);
	bool GetSwitchMode(int x, int y);

	bool isActive;
};

class ShutterManage {
public:
	struct Shutter {
		VECTOR2 position;
		int index;
	};
	static ShutterManage* GetInstance() {
		static ShutterManage instance;
		return &instance;
	}
	void Init(std::string file_path);
	void Render();

	bool CheckCollision(Object* obj);
	void Active(bool isActive);
private:
	int GetMapIndexByPosition(VECTOR2 position);
	std::vector<Shutter> shutter_map;
};