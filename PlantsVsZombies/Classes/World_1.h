#pragma once
#include "cocos2d.h"
#include "LevelData.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"
#include "SelectPlantsGameReadyScene.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class World_1 :public Scene
{
public:
	static Scene* createScene();             /* 创建场景 */
	virtual bool init();                     /* 初始化 */
	CREATE_FUNC(World_1);

private:
	Size backgroundSize = Director::getInstance()->getWinSize();
	Global* global = Global::getInstance();
	ParallaxNode* Parallax;
	ui::ScrollView* scrollView;
	int Level = 0;
	float WorldPosition = 0;                 /* 世界初始位置 */
	std::list<ui::Button*> LevelButton;

private:
	void CreateGoBack();
	void CreateBackground();
	void CreateScrollView();
	void AddScrollView();
	void CreateButtonListener(ui::Button* button, const int& ID);
	void CreateButton(Node* node, const std::string& name, const Vec2& position);
	Sprite* CreateSprite(const std::string& name, const Vec2& position, const float& scale, const int& zorder, const float& speed, bool IsFlipped = false);
	Sprite* CreateSprite(Node* node, const std::string& name, const Vec2& position, const float& scale, const int& zorder,bool IsFlipped = false);

};