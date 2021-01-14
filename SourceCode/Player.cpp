#include "Player.h"
GameLib::Sprite* playerSpr;
extern Map test;
extern WindMap WindM;
extern std::vector<Fan>fans;
extern std::vector<Fan>wind;
extern std::vector<Fan>dist;
Player player;
using namespace GameLib::input;
void Player::Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
{
    playerSpr = GameLib::sprite_load(L"./Data/Images/Player.png");
    Object::Initialize(playerSpr, p, s, tp, ts);
}

bool FanCollision(VECTOR2 player_pos, VECTOR2 fan_pos)
{
    VECTOR2 temp{ roundf(player.pos.x / 54.0f), roundf(player.pos.y / 54) };
    return temp == fan_pos;
}


void Player::Update()
{
    speed.x = {};
    const VECTOR2 tpos{ pos };
    if (STATE(0) & PAD_LEFT)
        speed.x = -6;
    if (STATE(0) & PAD_RIGHT)
        speed.x = 6;

    if (!onGround)
        speed.y += 1;
    else if (onGround)
        speed.y = 0;

    Wind();

    if (TRG(0) & PAD_L1 && onGround)
    {
        speed.y = -15.0f;
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

