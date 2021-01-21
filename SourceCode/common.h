#ifndef COMMON_H
#define COMMON_H

// Define statements
#define SCENE_NONE -1
#define SCENE_TITLE 0
#define SCENE_GAME 1
#define SCREEN_W 1920
#define SCREEN_H 940


// Declaration
extern int curScene, nextScene;
enum SCENES
{
    NONE =  -1, 
    STAGE_SELECT = 10,
    TITLE = 0,
    STAGE1, 
    STAGE2, 
    STAGE3, 
    STAGE4, 
    STAGE5, 
    STAGE6, 
    STAGE7, 
    STAGE8, 
    END
};

#define GETFOLDERNAME(index) ("Map" + std::to_string(index))
#define ROBOTWALK 0
#endif 