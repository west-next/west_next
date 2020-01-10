#pragma once
#include "cocos2d.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"

using namespace cocos2d;
using namespace rapidjson;

class OpenLevelData
{
public:
	/* 单例模式 */
	static OpenLevelData* getInstance();
	void OpenLevelsData(const std::string& worlddata);
	void ReadLevelData(char* LevelName);
	void Decrypt(char* cSrc, char* cDest);                              /* 解密函数 */

private:
	typedef struct
	{
		int x, y;
	}_MyPoint;

public:
	enum  GameTypes
	{
		CreateWall,
		AtLeastSunNumbers,
		FlowerPosition,
		CarNumbers,
		UserPlantsNumbers,
		ZombiesInvisible,
		SmallZombies,
		BigZombies,
		NoPlants
	};

	typedef struct
	{
		bool IsEncryption;                                              /* 是否加密 */
		bool ZombiesIsVisible = false;                                  /* 僵尸是否隐身 */
		bool ZombiesIsSmall = false;                                    /* 是否是小僵尸 */
		bool ZombiesIsBig = false;                                      /* 是否是巨人僵尸 */
		bool IsNoPlants = false;                                        /* 是否不可种植 */
		int ZombiesFrequency;                                           /* 僵尸总波数 */
		int CoinNumbers;                                                /* 金币数 */
		int AtLeastSunNumbers = -1;                                     /* 至少产生的阳光数 */
		int FlowerPosition;                                             /* 花坛位置 */
		int CarNumbers;                                                 /* 小车数量 */
		int UsePlantsNumbers;                                           /* 使用植物数量 */
		float UserLose;                                                 /* 玩家失败 */
		std::vector<int>GameType;                                       /* 游戏类型 */
		std::vector<int>ZombiesType;                                    /* 僵尸类型 */
		std::vector<int>ZombiesNumbers;                                 /* 僵尸数 */
		std::vector<int>MunchZombiesFrequency;                          /* 多僵尸波数 */
		std::vector<std::vector<int> >ZombiesTypeProbabilityFrequency;  /* 每一波每种僵尸出现的概率 */
		std::vector<_MyPoint>NoPlantsPosition;                          /* 不可以种植的地方 */
		int FirstFrequencyTime;                                         /* 第一波僵尸出现时间 */
	}_LevelData;
	_LevelData LevelData;

private:
	static OpenLevelData* _instance;
	Document document;

	void SetInit();
};