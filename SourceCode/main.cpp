// Including the needed files
#include "all.h"

int curScene = SCENES::NONE;
int nextScene = SCENES::STAGE_SELECT;
#include "TestMap.h"
#include "Stage Select.h"
// WinMain
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	// Initializing the Game Library
	// (L"[something]") is the name displayed on the toolbar, after that, those 2 numbers are the ratio
	GameLib::init(L"自分でプロジェクトを作成", SCREEN_W, SCREEN_H, true);

	GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);

	// GameLoop
	while (GameLib::gameLoop())
	{
		// Scene change process
		if (curScene != nextScene)
		{
			// The process following the current scene
			switch (curScene)
			{
			case SCENES::TITLE:
				test_deinit();
				break;
			case SCENES::STAGE1:
				test_deinit();
				break;
			}
			// Initialization following the next scene
			switch (nextScene)
			{
			case SCENES::TITLE:
				break;
			case SCENES::STAGE_SELECT:
				stSel_Init();
				break;
			default:
				test_init();
				break;
			}
			// Making curScene equal to nextScene
			curScene = nextScene;
		}
		
		// Input update
		input::update();

		//// Title update
		//title_update();
		//
		//// Title render
		//title_render();
		
		// Rendering and updating according to the current scene
		switch (curScene)
		{
		case SCENES::TITLE: break;
		case SCENES::STAGE_SELECT:
			stSel_Update();
			stSel_Render();
			break;
		default:
			test_update();
			test_render();
		//	break;
		}
		// debug display
		debug::display(1, 1, 1, 1, 1);
		// Display
		GameLib::present(1, 0);

	}
	// Deinitialization process
	switch (curScene)
	{
	case SCENE_TITLE:
		test_deinit();
		break;
	//case SCENE_GAME:
	//	game_deinit();
	//	break;
	}
	// GameLib end process

	music::clear();
	GameLib::uninit();
	// Return value to 0
	return 0;
}