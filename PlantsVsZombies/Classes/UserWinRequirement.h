#pragma once
#include "cocos2d.h"
#include "LevelData.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class UserWinRequirement :public Node
{
public:
	/* 单例模式 */
	static UserWinRequirement* getInstance();

	void CreateDialogBox(Node* node, const int& finishedid, const int& ID = 0);

private:
	void ShowRequirement(const int& finishedid);                         /* 显示要求 */
	void ShowText(const string& text, const int& ID, Color3B color = Color3B::BLACK);

private:
	static UserWinRequirement* _instance;
	ui::Scale9Sprite* LevelObjiectives = nullptr;
	Global* global = Global::getInstance();
	Vec2 PhasePosition;                                                 /* 相差位置 */

	Vec2 TextPosition[4][4] =                                           /* 文字位置 */
	{
		{Vec2(200,270),},
		{Vec2(200,220),Vec2(200,320),},
		{Vec2(200,180),Vec2(200,270),Vec2(200,360)},
		{Vec2(200,160),Vec2(200,230),Vec2(200,300),Vec2(200,370)}
	};
};