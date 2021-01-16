#ifndef COMMON_H
#define COMMON_H

// Define statements
#define SCENE_NONE -1
#define SCENE_TITLE 0
#define SCENE_GAME 1
#define SCREEN_W 1920
#define SCREEN_H 1080
#define OUTER_HP_BAR_X SCREEN_W / 3
#define INNER_HP_BAR_X SCREEN_W / 3 + 5
#define OUTER_HP_BAR_Y 600
#define INNER_HP_BAR_Y 605
#define SCORE_X 1100
#define SCORE_Y 50
#define COMBO_X 550
#define COMBO_Y 50
#define COMBO_NO_X 600
#define COMBO_NO_Y 80
#define HITAREA_1_X 200
#define HITAREA_1_Y 250
#define HITAREA_2_X 200
#define HITAREA_2_Y 450

// Declaration
extern int curScene, nextScene;
enum SCENES
{
    NONE =  -1, 
    TITLE, 
    STAGE_SELECT, 
    STAGE1
};
#endif 