#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "Object.h"

class Player : public Object
{
public:
    VECTOR2 speed;
    int state;
    int anime;
    int animeTimer;
    bool onGround;
    bool Ceiling{};
    bool hitWind;
    void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts) override;
    void Update() override;
    void Wind();
};

void player_init();
void player_render();
void player_update();

#endif
