#include "Animation.h"

VECTOR2 Player_Animation::GetAnimation_Offset(STATE state) {
	if (state != cur_state) {
		animation_time = 0;
		cur_state = state;
	}

	if (state == STATE::IDLE)
		return { 16 + ((animation_time++ / 6) % 7) * 1.0f, 0.0f };
	else if (state == STATE::WALK)
		return { ((animation_time++ / 2) % 16) * 1.0f, 0.0f };

	return { 0, 0 };
}