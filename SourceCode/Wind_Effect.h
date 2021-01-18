#pragma once
#include "all.h"
#include "Map.h"

class Wind_Effect{
public:
	static Wind_Effect* GetInstance() {
		static Wind_Effect instance;
		return &instance;
	}


	void Init();
	void Update();
	void Render();

private:
	struct Wind_Prop {
		VECTOR2 position;
		VECTOR2 speed;

		float alpha;
	};

	std::shared_ptr<GameLib::Sprite> eff_spr;
	
	std::vector<Wind_Prop> effs;
	int delta_time;
};