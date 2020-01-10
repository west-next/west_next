#pragma once
#include "cocos2d.h"
#include "UserLose.h"
#include "LevelData.h"
#include "GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;

class GameType
{
public:
	/* 单例模式 */
	static GameType* getInstance();

	void CreateGameType(Node* node);                                  /* 创建游戏类型 */
	void Initialization();                                            /* 初始化 */
	void UpdateRequirementNumbers(Node* node,const std::string name); /* 更新数量 */
	void WaringPlantsNull();                                          /* 警告植物使用完 */
	//void JudgeZombiesWin(map<int, Global::_Zombies>::iterator Iter);

public:
	typedef struct
	{
		ui::Text* AllSunNumbersText;                 /* 显示阳光数字 */
		int AtLeastSunNumbers;                       /* 至少需要的阳光数 */
		int AllSunNumbers = 0;                       /* 阳光数 */
		bool IsHaveSunNumberRequriement = false;     /* 是否有阳光要求 */
		bool IsUpdateImage = false;                  /* 图片是否更新 */
	}_SunNumberRequriement;
	_SunNumberRequriement SunNumberRequriement;

	typedef struct
	{
		ui::Text* PalntsNumbersText;                 /* 显示植物数量 */
		int UserPlantsNumbers = -1;                  /* 可以使用的植物数量 */
		int SurPlusPlantsNumbers = -1;               /* 剩余植物数量 */
		bool IsHavePlantsRequriement = false;        /* 是否有植物要求 */
		bool IsUpdateImage = false;                  /* 图片是否更新 */
	}_PlantsRequriement;
	_PlantsRequriement PlantsRequriement;

private:
	static GameType* _instance;
	Global* global = Global::getInstance();
	void ShowNumbers(Node* node, const int& ID);     /* 显示数量 */
	void UpdateNumbers(Node* node, const int& ID);   /* 更新数量 */
};