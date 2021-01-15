#include "UI.h"

bool isPause;

void UI_BTN::SetClickEvent(onClickEvent _event) {
	_onClickEvent = _event;
}

void UI_BTN::Update() {
	if (!isActive) return;
	using namespace input;
	
	VECTOR2 cursor_pos = { getCursorPosX() * 1.0f, getCursorPosY() * 1.0f };
	VECTOR2 dif_pos = cursor_pos - position;

	if (std::abs(dif_pos.x) <= size.x && std::abs(dif_pos.y) <= size.y) {
		if (_onClickEvent) _onClickEvent();
	}
}

void UI_BTN::Render() {
	if (!isActive) return;
	
	if(texture)
		GameLib::sprite_render(texture.get(), 
								position.x, position.y, 
								scale.x, scale.y, 
								0, 0, 
								size.x, size.y, 
								0, 0, 0, 1, 1, 1, 1);
}

void UI_GP_Manage::Init() {
	std::shared_ptr<Sprite> gear_ui (sprite_load(L"./Data/Images/MENU ICON.png"));

	buttons.insert({ "PAUSE_BTN", UI_BTN({ 1700, 25 }, { 0.6f, 0.6f }, { 256, 256 }, gear_ui, [] {
		isPause = !isPause;
	})});
}

void UI_GP_Manage::Update() {
	for (auto& btn : buttons) {
		btn.second.Update();
	}
}

void UI_GP_Manage::Render() {
	for (auto& btn : buttons) {
		btn.second.Render();
	}
}