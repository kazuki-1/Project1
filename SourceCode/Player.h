#ifndef PLAYER_H
#define PLAYER_H
#include "common.h"

#include "Object.h"
#include "Map.h"
class Fan;
enum PUSH_DIR
{
    LEFT,
    RIGHT,
};

class Player : public Object
{
public:
    VECTOR2 speed;
    int state;
    int anime;
    int animeTimer;
    int timer;
    bool onGround;
    bool Ceiling{};
    bool hitWind;
    bool WallFlag{};
    void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts) override;
    void Update() override;
    void Wind();
    void PushFan(Fan* fan);
};

void player_init();
void player_render();
void player_update();

#endif
