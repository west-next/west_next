#pragma once
#include "cocos2d.h"
#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "GlobalVariable.h"

using namespace cocos2d;

class HelpScene :public Scene
{
public:
	static Scene* createHelpScene();
	virtual bool init();
	CREATE_FUNC(HelpScene);
	
private:
	Global* global = Global::getInstance();
};
