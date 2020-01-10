#pragma once
#include "cocos2d.h"
#include "World_1.h"
#include "GlobalVariable.h"

using namespace cocos2d;

class UserLose :public Scene
{
public:
	static Scene* createScene();             /* 创建场景 */
	virtual bool init();                     /* 初始化 */
	CREATE_FUNC(UserLose);

private:
	Global* global = Global::getInstance();
};