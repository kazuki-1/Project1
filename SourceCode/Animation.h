#pragma once
#include "all.h"
#include "Object.h"
#include "common.h"

#define ROBOTWALK 0

class Player_Animation {
public:
	enum STATE { IDLE, WALK, JUMP, FALL };
	
	VECTOR2 GetAnimation_Offset(STATE state);
	STATE cur_state = IDLE;
private:
	int animation_time;
};