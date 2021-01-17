#include "UI.h"

bool isPause;

void UI_BTN::SetClickEvent(onClickEvent _event) {
	_onClickEvent = _event;
}

void UI_BTN::Update() {
	if (!isActive) return;
	using namespace input;

	VECTOR2 cursor_pos = { (float)getCursorPosX(), (float)getCursorPosY()};
	VECTOR2 dif_pos = cursor_pos - position;



	debug::setString("%f", cursor_pos.y);
	debug::setString("%f", position.y);

	if (std::abs(dif_pos.x) <= size.x * 0.5f * scale.x && std::abs(dif_pos.y) <= size.y * 0.5f * scale.y) {
		if (TRG_RELEASE(0) & PAD_START) {
			if (_onClickEvent) _onClickEvent();
		} 
		color_value += (0.5f - color_value) * 0.1f;
	} else {
		color_value += (1 - color_value) * 0.1f;
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
								size.x / 2, size.y / 2, 0, color_value, color_value, color_value, color_value);
}

void UI_GP_Manage::Init() {
	std::shared_ptr<Sprite> gear_ui		(sprite_load(L"./Data/Images/MENU ICON.png"));
	std::shared_ptr<Sprite> title_ui	(sprite_load(L"./Data/Images/TITLE_BTN.png"));
	std::shared_ptr<Sprite> stage_ui	(sprite_load(L"./Data/Images/STAGE_BTN.png"));
	std::shared_ptr<Sprite> reset_ui	(sprite_load(L"./Data/Images/RESET_BTN.png"));

	buttons.insert({ "PAUSE_BTN", UI_BTN({ 1750, 125 }, { 0.6f, 0.6f }, { 256, 256 }, gear_ui, [] {
		isPause = !isPause;
	})});

	buttons.insert({ "TITLE_BTN", UI_BTN({ SCREEN_W / 2, SCREEN_H / 2 - 100}, { 0.6f, 0.6f }, { 761, 154 }, title_ui, [] {
		
	}) });

	buttons.insert({ "STAGE_BTN", UI_BTN({ SCREEN_W / 2, SCREEN_H / 2}, { 0.6f, 0.6f }, { 761, 154 }, stage_ui, [] {
		nextScene = STAGE_SELECT;
	}) });

	buttons.insert({ "RESET_BTN", UI_BTN({ SCREEN_W / 2, SCREEN_H / 2 + 100}, { 0.6f, 0.6f }, { 761, 154 }, reset_ui, [] {
		nextScene = STAGE1;
	}) });

	buttons["TITLE_BTN"].isActive = false;
	buttons["RESET_BTN"].isActive = false;
	buttons["STAGE_BTN"].isActive = false;
}

void UI_GP_Manage::Update() {
	for (auto& btn : buttons) {
		btn.second.Update();
	}

	buttons["TITLE_BTN"].isActive = isPause;
	buttons["RESET_BTN"].isActive = isPause;
	buttons["STAGE_BTN"].isActive = isPause;
}

void UI_GP_Manage::Render() {
	for (auto& btn : buttons) {
		btn.second.Render();
	}
}