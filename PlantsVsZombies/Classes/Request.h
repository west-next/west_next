#pragma once
#include "Dialog.h"
#include "cocos2d.h"
#include "LevelData.h"
#include "UserWinRequirement.h"

using namespace cocos2d;

class Request :public Dialog
{
public:
	static Scene* CreateScene();
	virtual bool init();
	CREATE_FUNC(Request);

	void JudgeZombiesWin(map<int, Global::_Zombies>::iterator Iter);
	int JudgeUserWin();

private:
	OpenLevelData* LevelData = OpenLevelData::getInstance();
};