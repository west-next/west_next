#pragma once
#include <map>
#include <list>
#include "AudioEngine.h"
#include "spine/spine.h"
#include "ui/CocosGUI.h"
#include "spine\spine-cocos2dx.h"

using namespace std;
using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Global
{
public:
	static Global* getInstance();
	int changeBgMusic(const std::string& _musicName,bool _loop);

public:
	/* 音乐音效变量 */
	struct Global_Music
	{
		float SoundEffect = 1;             /* 音效 */
		float BackGroundMusicVolume = 0.5f;
		list<int>BackGroundMusic;          /* 音乐 */
	};
	Global_Music GlobalMusic;

	/* 设置参数 */
	struct Global_Parameter
	{
		ui::CheckBox::EventType Global_Information = ui::CheckBox::EventType::SELECTED;
		ui::CheckBox::EventType Global_HighFPS = ui::CheckBox::EventType::UNSELECTED;
		ui::CheckBox::EventType Global_FullScreen = ui::CheckBox::EventType::UNSELECTED;
		ui::CheckBox::EventType Global_CursorNotHide = ui::CheckBox::EventType::UNSELECTED;
		ui::CheckBox::EventType Global_StretchingShow = ui::CheckBox::EventType::SELECTED;
	};
	Global_Parameter GlobalParameter;

	/* 用户名称 */
	struct User
	{
		std::string UserName = "愤怒的小僵尸"; /* 用户名字 */
		bool IsUpdate = false;
		std::string UserCaveFileName[8];
		std::string UserCaveFileNameKey[8] =
		{ {"USERNAMEDATA"},
		{"USERNAMEDATA_2"},
		{"USERNAMEDATA_3"},
		{"USERNAMEDATA_4"},
		{"USERNAMEDATA_5"},
		{"USERNAMEDATA_6"},
		{"USERNAMEDATA_7"},
		{"USERNAMEDATA_8"} };
	};
	User UserData;

	/* 用户选择的卡牌存储 */
	typedef struct
	{
		int CardNumbers = 0;  /* 卡片数量 */
		int CardID[10];       /* 卡片名字 */
	}userselectcard;
	userselectcard UserSelectCard;

	/* 背景枚举 */
	enum class _BackGroundImageName
	{
		Mordern = 0,
		DarkAges = 1,
		NeonMixtapeTour = 2,
		FrostibiteCaves = 3
	};
	_BackGroundImageName BackGroundImageName;

	/* 僵尸类型 */
	enum class ZombiesType
	{
		CommonZombies = 1,
		ConeZombies,
		BucketZombies,
		CommonDoorZombies,
		ConeDoorZombies,
		BucketDoorZombies,
		LmpZombies
	}; 
	
	/* 僵尸存储结构体 */
	typedef struct
	{
		SkeletonAnimation* ZombiesAnimation;   /* 动画 */
		ZombiesType ZombiesType;               /* 僵尸类型 */
		int MusicType;                         /* 音乐类型 */
		int AnimationID;                       /* 动画播放ID */
		int Tag;                               /* 标记 */
		float _BloodVolume;                    /* 血量 */
		float _ShieldVolume;                   /* 护盾血量 */
		float _ShieldVolume1;                  /* 护盾1血量 */
		float BloodVolume;                     /* 血量 */
		float ShieldVolume;                    /* 护盾血量 */
		float ShieldVolume1;                   /* 护盾1血量 */
		float Speed;                           /* 移动速度 */
		float CaveSpeed;                       /* 存储移动速度 */
		bool IsUseForGameType = false;         /* 是否被使用在游戏类型 */
		bool IsHaveShield;                     /* 是否有护盾 */
		bool IsEat;                            /* 是否在吃植物 */
		bool IsEatGarlic = false;              /* 是否吃大蒜 */
		bool IsShow = false;                   /* 是否现身 */
		bool IsCanDelete[2] = { false };       /* 是否可以删除 */
	}_Zombies;

	map<int, _Zombies>Zombies;
	map<int, _Zombies>::iterator Iter;

	/* 游戏地图 */
	typedef struct
	{
		int PlantsMap[5][9];      /* 植物种植地图 */
		int RowNumbers;           /* 行数 */
		int ColumnNumbers;        /* 列数 */
	}_MapSize;
	_MapSize MapSize;

	/* 当前关数 */
	typedef struct
	{
		int Levels = 1;  /* 当前关数 */
		bool IsBeginShowEggs = false;
	}_NowLevels;
	_NowLevels NowLevels[5];

	/* 当前玩的关数与世界 */
	typedef struct
	{
		Sprite* background;                              /* 当前世界地图 */
		char PlayWorldName[128];                         /* 当前世界名字 */
		int PlayLevels = 1;                              /* 当前关数 */
		int PlayWorlds = 1;                              /* 当前世界 */
		bool IsReadWoldInformation[13] = { false };      /* 是否读取世界信息 */
	}_NowPlayLevels;
	_NowPlayLevels NowPlayLevels;

	/* 用户游戏数据 */
	typedef struct
	{
		int KillAllZombies = 0;    /* 杀死僵尸总数 */
		int UsePlantsNumbers = 0;  /* 使用植物数量 */
		int BreakThrough = 0;      /* 闯关失败次数 */
		int UserDataNumber = -1;   /* 用户游戏存档编号 */
		string UserDataName[8] =   /* 用户存档名称 */
		{ {"WORLD_1_LEVELS"},
		{"WORLD_1_LEVELS_2"},
		{"WORLD_1_LEVELS_3"},
		{"WORLD_1_LEVELS_4"},
		{"WORLD_1_LEVELS_5"},
		{"WORLD_1_LEVELS_6"},
		{"WORLD_1_LEVELS_7"},
		{"WORLD_1_LEVELS_8"}
		};
	}_UserGameData;
	_UserGameData UserGameData;

	/* 场景存储 */
	typedef struct
	{
		map<int, Scene*> CaveScene;
		Scene* Temporary = nullptr;
		int Level = -1;
	}_CaveScene;
	_CaveScene CaveScene;

	/* 动画存储 */
	map<string, spSkeletonData*>GlobalAnimationData;

	/* 游戏文本 */
	map<string, string>GameText;

	/* 图片路径 */
	map<string, string>ImagesPath;

	/* 音乐路径 */
	map<string, string>MusicsPath;

	/* 动画路径 */
	map<string, string>AnimationPath;

	/* 文本路径 */
	map<string, string>TextPath;

	/* 阳光数 */
	int SunNumbers;

	/* 金币数 */
	int CoinNumbers;

	/* 当前fps */
	int NowFps;

	/* 标记主菜单到世界 */
	bool MainToWorld;

private:
	static Global* _instance;    /* 单例 */
};