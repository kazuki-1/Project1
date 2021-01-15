#include "UI.h"

bool isPause;

void UI_BTN::SetClickEvent(onClickEvent _event) {
	_onClickEvent = _event;
}

void UI_BTN::Update() {
	if (!isActive) return;
	using namespace input;

	VECTOR2 cursor_pos = { getCursorPosX() * 1.0f, getCursorPosY() * 1.0f };
	VECTOR2 dif_pos = cursor_pos - (position + VECTOR2(size.x * scale.x, size.y * scale.y) / 2);



	debug::setString("%f", cursor_pos.x);
	debug::setString("%f", position.x);

	if (std::abs(dif_pos.x) <= size.x * scale.x && std::abs(dif_pos.y) <= size.y * scale.y) {
		if (TRG_RELEASE(0) & PAD_START) {
			if (_onClickEvent) _onClickEvent();
		} 
		color_value += (6 - color_value) * 0.01f;
	} else {
		color_value += (1 - color_value) * 0.01f;
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
								0, 0, 0, color_value, color_value, color_value, color_value);
}

void UI_GP_Manage::Init() {
	std::shared_ptr<Sprite> gear_ui		(sprite_load(L"./Data/Images/MENU ICON.png"));
	std::shared_ptr<Sprite> title_ui	(sprite_load(L"./Data/Images/TITLE_BTN.png"));

	buttons.insert({ "PAUSE_BTN", UI_BTN({ 1700, 25 }, { 0.6f, 0.6f }, { 256, 256 }, gear_ui, [] {
		isPause = !isPause;
	})});

	buttons.insert({ "TITLE_BTN", UI_BTN({ SCREEN_W / 2 - 300, SCREEN_H / 2}, { 0.6f, 0.6f }, { 761, 154 }, title_ui, [] {
	isPause = !isPause;
}) });

	buttons["TITLE_BTN"].isActive = false;
}

void UI_GP_Manage::Update() {
	for (auto& btn : buttons) {
		btn.second.Update();
	}

	buttons["TITLE_BTN"].isActive = isPause;
}

void UI_GP_Manage::Render() {
	for (auto& btn : buttons) {
		btn.second.Render();
	}
}