#include "Wind_Effect.h"

extern std::vector<Fan>fans;
extern std::vector<Fan>wind;

void Wind_Effect::Init() {
	if (!eff_spr) eff_spr = std::shared_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/dust.png"));

	delta_time = 0;
	effs.clear();

}

void Wind_Effect::Update() {
	if (delta_time <= 0) {
		for (auto& it : fans) {
			if (!it.On && !it.AlwaysOn) continue;
			VECTOR2 speed = { 0, 0 };

			if (it.dir == Fan::Direction::UP)			speed = { 0, -1 };
			else if (it.dir == Fan::Direction::DOWN)	speed = { 0, 1 };
			else if (it.dir == Fan::Direction::LEFT)	speed = { -1, 0 };
			else if (it.dir == Fan::Direction::RIGHT)	speed = { 1, 0 };
		
			effs.push_back({ it.pos + (speed * 55), speed * 5, 0.75f });
		}
		delta_time = 10;
	}

	for (auto& eff : effs) {
		for (auto& it : wind) {
			if (WindCollisionCheck(eff.position, { it.x * 1.0f, it.y * 1.0f })) {
				if (it.dir == Fan::Direction::LEFT) {
					eff.speed.x -= 1.0f;
					eff.speed.y *= 0.75f;
				} else if (it.dir == Fan::Direction::RIGHT) {
					eff.speed.x += 1.0f;
					eff.speed.y *= 0.75f;
				}
				else if (it.dir == Fan::Direction::UP) {
					eff.speed.y -= 1;
					eff.speed.x *= 0.75f;
				}
			}
		}
		eff.position += eff.speed * 0.25f;
		eff.alpha -= 0.01f;
	}
	delta_time--;
}

void Wind_Effect::Render() {
	for (auto& eff : effs) {
		sprite_render(eff_spr.get(), eff.position.x, eff.position.y, 0.7f * eff.alpha, 0.7f * eff.alpha, 0, 0, 108, 108, 54, 54, 0, 1, 1, 1, eff.alpha);
	}
}