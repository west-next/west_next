#pragma once
#include "cocos2d.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

class Dialog :public Scene
{

protected:

	/* 加载背景 */                    
	virtual void CreateBackGroundImage();

	/* 创建触摸监听 */           
	virtual EventListenerTouchOneByOne* CreateTouchtListener(Sprite* sprite);

	/* 创建标签 */
	virtual Label* label(const std::string &name, const float& fontsize, Vec2 &vec2 = Vec2(0, 0), const float& space = 0, const Color3B& color = Color3B::GREEN, const float& scale = 1);

	/* 创建按钮上的标签 */
	virtual void CreateLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, Vec2 &vec2, float& fontsize, const float& space = 0, const Color3B& color = Color3B::GREEN);

protected:
	Global* global = Global::getInstance();

private:
	Vec2 PhasePosition; /* 相差位置 */
};
