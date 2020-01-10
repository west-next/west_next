#pragma once
#include "cocos2d.h"
#include "World_1.h"
#include "Pause_Quit_Game.h"
#include "ui/CocosGUI.h"
#include "GlobalVariable.h"

using namespace cocos2d;

class Quit_Game :public Pause_Quit_Game
{
public:
	static Scene* CreateScene();
	virtual bool init();
	CREATE_FUNC(Quit_Game);

private:
	void Create_Dialog();                                                                /* 创建对话框 */
	Global* global = Global::getInstance();
};