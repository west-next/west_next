#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class SelectWorldScene :public Scene
{
public:
	static Scene* createScene();             /* 创建场景 */
	virtual bool init();                     /* 初始化 */
	CREATE_FUNC(SelectWorldScene);

protected:
	void CreateGoBack();

private:
	Button* World[13];
	Global* global = Global::getInstance();
};