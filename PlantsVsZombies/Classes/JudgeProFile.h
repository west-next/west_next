#pragma once
#include "Dialog.h"
#include "cocos2d.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UiImageText.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;

class JudgeProFile :public Dialog
{
public:
	static Scene* CreateScene();
	virtual bool init() override;
	CREATE_FUNC(JudgeProFile);
};