#include "Player.h"
#include "Animation.h"
#include "Switch_Manage.h"

GameLib::Sprite* playerSpr;
extern Map Collision;
extern WindMap WindM;
extern std::vector<Fan>fans;
extern std::vector<Fan>wind;
extern std::vector<Fan>dist;
Player player;


std::vector<Fan*> slip_fan;
Player_Animation animation;
Fan* pushedFan;


float tar_pos_x = -1;
void FanColl(Player* p, Fan* f)
{
    VECTOR2 p_tl = p->pos - p->pivot;
    VECTOR2 p_br = p->pos + VECTOR2(p->pivot.x, 0);
    VECTOR2 f_tl = { f->pos.x - 26.0f, f->pos.y - 26.0f };
    VECTOR2 f_br = { f->pos.x + 26.0f, f->pos.y + 26.0f };
    if (p_tl.x > f_br.x)
        return;
    if (p_tl.y > f_br.y)
        return;
    if (p_br.x < f_tl.x)
        return;
    if (p_br.y < f_tl.y)
        return;
    if (p->pos.y >= f->pos.y - 27.0f && abs(p->pos.x - f->pos.x) <= 26) {
        p->pos.y = f->pos.y - 27.0f;
        p->onGround = true;
    }
    if (p->pos.y > f->pos.y && p->pos.x > f->pos.x)
        p->pos.x = f->pos.x + 54.0f;
    else if (p->pos.y > f->pos.y && p->pos.x < f->pos.x)
        p->pos.x = f->pos.x - 54.0f;
    //if (p->pos.y < f->pos.y + 54.0f)
    //    p->pos.y = f->pos.y + 54.0f;

}




using namespace GameLib::input;
void Player::Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
{
    playerSpr = GameLib::sprite_load(L"./Data/Images/player_.png");
    Object::Initialize(playerSpr, p, s, tp, ts);

    pushedFan = nullptr;
    tar_pos_x = -1;
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

void CheckSwitchCollision(Switch_Manage* switch_manage) {
    std::tuple<int, int> switch_pos = switch_manage->CheckCollision(&player);
    int x = std::get<0>(switch_pos);
    int y = std::get<1>(switch_pos);
    if (x != -1 && y != -1) 
        switch_manage->TriggerSwitch(x, y);
}
#define XSIZE 70
#define YSIZE 108

bool isJump = false;
void Player::Update()
{
    debug::setString("%d", onGround);
    speed.x = {};
    const VECTOR2 tpos{ pos };
    if (pushedFan && (TRG(0) & PAD_TRG1))
    {
        if (pushedFan->dir == Fan::Direction::LEFT)
            pushedFan->dir = Fan::Direction::RIGHT;
        else if (pushedFan->dir == Fan::Direction::RIGHT)
            pushedFan->dir = Fan::Direction::LEFT;
    }
    if (pushedFan)
    {
        for (auto& a : fans)
        {
            if (pushedFan->x == a.x && pushedFan->y == a.y + 1)
                a.dir = pushedFan->dir;
        }
    }
    if (STATE(0) & PAD_LEFT) {
        if (!isJump) {
            player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::WALK);
            player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);
        }

        player.size.x = -1;
        speed.x = -6;
    }
    else if (STATE(0) & PAD_RIGHT) {
        if (!isJump) {
            player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::WALK);
            player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);
        }

        player.size.x = 1;
        speed.x = 6;
    }
    else {
        if (!isJump) {
            player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::IDLE);
            player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);
        }
    }
    if (pushedFan)
    {
        FanColl(this, pushedFan);
    }
    
    if (!onGround) {
        if (isJump) {
            player.tPos = animation.GetAnimation_Offset(Player_Animation::STATE::JUMP);
            player.tPos = VECTOR2(player.tPos.x * XSIZE, player.tPos.y * YSIZE);
        }
        if(speed.y < 25)
            speed.y += 1;
    }
    else if (onGround && speed.y > 0) {
        isJump = false;
        speed.y = 0;
    }

    Wind();

    if (TRG(0) & PAD_L1 && onGround)
    {
        speed.y = -16.0f;
        onGround = false;
        isJump = true;
    }

    if (TopChipCheck(&player, &Collision)) {
        if (speed.y < 0) {
            pos.y = std::roundf(pos.y / 54) * 54;
            speed.y = 0;
        }
    }
    pos += speed;

    CheckSwitchCollision(ShutterSwitch_Manage::GetInstance());
    CheckSwitchCollision(FanSwitch_Manage::GetInstance());

   /* if (HoriChipCheck(&player, &Collision) || ShutterManage::GetInstance()->CheckCollision(&player))
    {
        pos.x = std::round(tpos.x / 54) * 54.0f;
    }*/
    
    if (VertiChipCheck(&player, &Collision)) {
        if (speed.y > 0) {
            pos.y = std::ceil(pos.y / 54) * 54 - 28;
            onGround = true;
        }
    }
    else {
        onGround = false;
    }
    if (pushedFan)
    {
        bool fanCheck{};
        float a{ roundf(pos.x / 54) };

        bool isignore = false;
        debug::setString("fan : %d", pushedFan->x);
        for (int alpha = 0; alpha < fans.size(); ++alpha)
        {
            if (pushedFan == &fans[alpha])
                continue;
            if (pushedFan->y == fans[alpha].y && (pushedFan->x - 1 == fans[alpha].x || pushedFan->x + 1 == fans[alpha].x))
            {
                fanCheck = true;
                isignore = true;
                break;
            }
        }
        for (int y = 0; y < MAP_Y; ++y)
        {
            for (int x = 0; x < MAP_X; ++x)
            {
                if ((speed.x > 0 && Collision.getChip(pushedFan->pos + VECTOR2{ 27, 0 })) || (speed.x < 0 && Collision.getChip(pushedFan->pos - VECTOR2{ 27, 0 })))
                    fanCheck = true;
            }
        }
        
        if (!fanCheck && onGround && (pushedFan->pos.x < player.pos.x && speed.x < 0 || pushedFan->pos.x > player.pos.x && speed.x > 0)) {
            pushedFan->pos.x += speed.x;
            speed.x *= 0.3f;
            if (pushedFan->pos.x < player.pos.x) {
                tar_pos_x = std::floorf(pushedFan->pos.x / 54) * 54;
            }
            else {
                tar_pos_x = std::ceilf(pushedFan->pos.x / 54) * 54;
            }
            slip_fan.clear();
        }
        else if(!isignore){
            slip_fan.push_back(pushedFan);
            pushedFan = nullptr;
        }
        
    }
    if (tar_pos_x != -1) {
        for (int i = 0; i < slip_fan.size(); i++) {
            slip_fan[i]->pos.x += (tar_pos_x - slip_fan[i]->pos.x) * 0.15f;

            if (abs(slip_fan[i]->pos.x - tar_pos_x) <= 1) {
                slip_fan[i]->pos.x = tar_pos_x;
                slip_fan.erase(slip_fan.begin() + i);

                if (slip_fan.size() == 0)
                    tar_pos_x = -1;
            }
        }
    }

    //Check collsion after update all position parameter
    //If not, incorrect parameter (position value from previous frame) may cause miscalculation
    for (auto& fan : fans) {
        FanColl(this, &fan);
    }
    //Comment this to see what is
}

void player_init(std::string map_name)
{
    player.Initialize(playerSpr, { 120, 100 }, { 1, 1 }, { 0, 0 }, { 70, 108 });
    player.pivot = { 27, 108 };
    player.speed = { 0, 0 };

    music::load(ROBOTWALK, L"./Data/Sound/robot_walk.wav");
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

bool WindCollisionCheck(VECTOR2 player_pos, VECTOR2 wind_cell) {
    VECTOR2 player_cell = { std::roundf(player_pos.x / 54) * 1.0f,  std::roundf(player_pos.y / 54) * 1.0f };

    return player_cell == wind_cell;
}

void Player::Wind()
{

    for (int alpha = 0; alpha < wind.size(); ++alpha)
    {
        if (WindCollisionCheck(player.pos - VECTOR2(0, 54), { wind[alpha].x * 1.0f, wind[alpha].y * 1.0f }))
        {
            if (wind[alpha].dir == Fan::Direction::LEFT)
            {
                player.speed.x -= 1.0f;
            }
            else if (wind[alpha].dir == Fan::Direction::RIGHT)
            {
                player.speed.x += 1.0f;
            }
            else if (wind[alpha].dir == Fan::Direction::UP || wind[alpha].dir == Fan::Direction::RISE)
            {
                if (player.speed.y > -5)
                {
                    player.speed.y -= 2.4f;
                    onGround = true;
                }
                //if(speed.y > 0)
            }
        }
    }
}

void Player::PushFan(Fan* fan)
{
    if ((player.size.x > 0 && fan->pos.x > player.pos.x) || (player.size.x < 0 && fan->pos.x < player.pos.x)) {
        pushedFan = fan;
        ++timer;
    }
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