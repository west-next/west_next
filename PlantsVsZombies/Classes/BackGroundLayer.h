#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class BackGroundLayer :public Layer
{
public:
	virtual bool init();                     /* ≥ı ºªØ */
	CREATE_FUNC(BackGroundLayer);
	void addScene(Node* node);
	Sprite* CreateBackGroundEffect(Node* node = nullptr, const int& ID = 0);

private:

	void CreateFireEffect(Sprite* name);
	void CreateLightWallEffect(Node* name);

private:
	Global* global = Global::getInstance();
};