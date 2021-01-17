#pragma once
#include "all.h"

#define ROBOTWALK 0

class Sound {
public:
	Sound() {
		music::load(ROBOTWALK, L"./Data/Musics/game_maoudamashii_5_village10.wav");
	}

	~Sound() {
		music::clear();
	}

	static Sound* GetInstance() {
		static Sound instance;
		return &instance;
	}
};