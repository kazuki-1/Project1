#include "Player.h"
#include "Animation.h"

GameLib::Sprite* playerSpr;
extern Map test;
extern WindMap WindM;
extern std::vector<Fan>fans;
extern std::vector<Fan>wind;
extern std::vector<Fan>dist;
Player player;

Player_Animation animation;
Fan* pushedFan;

using namespace GameLib::input;
void Player::Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
{
    GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);

    playerSpr = GameLib::sprite_load(L"./Data/Images/animation sheet.png");
    Object::Initialize(playerSpr, p, s, tp, ts);
}

bool FanCollision(VECTOR2 player_pos, VECTOR2 fan_pos)
{
    VECTOR2 temp{ roundf(player.pos.x / 54.0f), roundf(player.pos.y / 54) };
    return temp == fan_pos;
}

int sign(int val) {
    if (val < 0) return -1;
    else if (val > 0) return 1;
    return 0;
}

#define XSIZE 54
#define YSIZE 108
void Player::Update()
{
    speed.x = {};
    const VECTOR2 tpos{ pos };
    if (STATE(0) & PAD_LEFT) { 
        player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::WALK);
        player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);

        player.size.x = -1;
        speed.x = -6;
    } else if (STATE(0) & PAD_RIGHT) {
        player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::WALK);
        player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);


        player.size.x = 1;
        speed.x = 6;
    } else {
        player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::IDLE);
        player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);
    }

    if (!onGround)
        speed.y += 1;
    else if (onGround)
        speed.y = 0;

    Wind();

    if (TRG(0) & PAD_L1 && onGround)
    {
        speed.y = -25.0f;
        onGround = false;
    }

    if (TopChipCheck(&player, &test)) {
        if (speed.y < 0) {
            pos.y = std::roundf(pos.y / 54) * 54;
            speed.y = 0;
        }
    }
    pos += speed;

    if (HoriChipCheck(&player, &test))
    {
        pos.x = std::round(tpos.x / 54) * 54.0f;
    }
    
    if (VertiChipCheck(&player, &test)) {
        pos.y = std::ceil(pos.y / 54) * 54 - 28;
        onGround = true;
    }
    else if (!VertiChipCheck(&player, &test))
    {
        onGround = false;
    }
    
    if (pushedFan) {
        if (pushedFan->pos.x < player.pos.x && speed.x < 0 || pushedFan->pos.x > player.pos.x && speed.x > 0) {
            pushedFan->pos.x += speed.x;
            speed *= 0.3f;
        }
        else {
            pushedFan = nullptr;
        }

        if(speed.x == 0) pushedFan = nullptr;
    }
}

void player_init()
{
    player.Initialize(playerSpr, { 120, 100 }, { 1, 1 }, { 0, 0 }, { 54, 108 });
    player.pivot = { 27, 108 };
    player.speed = { 0, 0 };
}

void player_render()
{
    player.Draw();
    GameLib::primitive::circle(player.pos, 10.0f, { 1, 1 }, 0.0f, { 1, 0, 0, 1 });
}

void player_update()
{
    player.Update();
}

bool windCollisionCheck(VECTOR2 player_pos, VECTOR2 wind_cell) { 
    VECTOR2 player_cell = { std::roundf(player_pos.x / 54) * 1.0f,  std::roundf(player_pos.y / 54) * 1.0f };

    return player_cell == wind_cell;
}

void Player::Wind()
{

    for (int alpha = 0; alpha < wind.size(); ++alpha)
    {
        if (windCollisionCheck(player.pos, { wind[alpha].x * 1.0f, wind[alpha].y * 1.0f }))
        {
            if (wind[alpha].dir == Fan::Direction::LEFT)
            {
                player.speed.x -= 1.0f;
            }
            else if (wind[alpha].dir == Fan::Direction::RIGHT)
            {
                player.speed.x += 1.0f;
            }
            else if (wind[alpha].dir == Fan::Direction::UP)
            {
                if(player.speed.y > -5)
                    player.speed.y -= 1.2f;
                //if(speed.y > 0)
            }
        }
    }
}

void Player::PushFan(Fan* fan)
{
    pushedFan = fan;
    ++timer;
    //switch (state)
    //{
    //case 1:
    //    speed = { 0, 0 };
    //    ++timer;
    //    if (timer > 30)
    //        ++state;
    //    break;
    //case 2:
    //    speed.x *= 0.3f;
    //    fan->pos.x += speed.x;
    //    break;

    //}

}