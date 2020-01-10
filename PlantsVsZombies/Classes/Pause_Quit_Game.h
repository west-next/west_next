#pragma once
#include "cocos2d.h"
#include "World_1.h"
#include "ui/CocosGUI.h"
#include "OptionsSence.h"
#include "GlobalVariable.h"
#include "SelectPlantsGameReadyScene.h"

using namespace cocos2d;

class Pause_Quit_Game :public OptionsMenu
{
public:
	static Scene* CreateScene();
	virtual bool init();
	CREATE_FUNC(Pause_Quit_Game);

protected:
	Director* director = Director::getInstance();
	EventListenerTouchOneByOne* TouchListener;

protected:
	void CreateOwnButton(const Vec2& vec2, const std::string name, const int& ID);
	virtual void Create_Dialog();
};