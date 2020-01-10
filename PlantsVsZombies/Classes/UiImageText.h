#pragma once
#include "Car.h"
#include "cocos2d.h"
#include "LevelData.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;

class UiImageText
{
public:
	/* 单例模式 */
	static UiImageText* getInstance();
	ProgressTimer* CreateProgressBar(Node* UiLayer);
	void CreateSunBank(Node* UiLayer);
	void CreateSeedBank(Node* UiLayer);
	void CreateCoinbank(Node* UiLayer);
	void CreateShovelBank(Node* UiLayer,bool* IsSelectShovel);
	void CreateText(Node* UiLayer);
	void CreateZombiesDieBank(Node* UiLayer);
	void CreateCar(Node* node);
	void CreateControlSpeed(Node* node);
	void UpdateZombiesDieNumbers();
	void CreatePromptText(Node* node,const string& PromptText);
	void ShowIsResume(Node* node);

private:
	static UiImageText* _instance;
	Text* ZombiesDieText;
	Vec2 PhasePosition;                                                 /* 相差位置 */
	Global* global = Global::getInstance();
};