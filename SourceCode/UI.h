#pragma once
#include "all.h";
#include <map>

using namespace GameLib;

typedef void (*onClickEvent)();
class UI_BTN {
public:
	UI_BTN(VECTOR2 _position, VECTOR2 _scale, VECTOR2 _size, std::shared_ptr<Sprite> _texture, onClickEvent _event) {
		position = _position;
		scale = _scale;
		size = _size;

		texture = _texture;
		_onClickEvent = _event;
	}
	UI_BTN() : UI_BTN({ 0, 0 }, { 0, 0 }, { 0, 0 }, nullptr, nullptr) { }

	bool isActive = true;

	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 size;

	float color_value = 1.0f;

	std::shared_ptr<Sprite> texture;

	void SetClickEvent(onClickEvent _event);
	void Update();
	void Render();
private:
	onClickEvent _onClickEvent;
};

class UI_GP_Manage { //Gameplay screen UI
public:
	void Init();
	void Update();
	void Render();

	static UI_GP_Manage* GetInstance() {
		static UI_GP_Manage instance;
		return &instance;
	}
private:
	std::map<std::string, UI_BTN> buttons;
};