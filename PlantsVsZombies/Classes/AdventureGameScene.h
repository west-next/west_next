#pragma once
#include "Car.h"
#include "Garlic.h"
#include "cocos2d.h"
#include "Zombies.h"
#include "WallNut.h"
#include "Jalapeno.h"
#include "UserLose.h"
#include "GameType.h"
#include "Spikeweed.h"
#include "LevelData.h"
#include "SunFlower.h"
#include "Torchwood.h"
#include "PotatoMine.h"
#include "PeaShooter.h"
#include "CherryBomb.h"
#include "CabbagePult.h"
#include "UiImageText.h"
#include "ui/CocosGUI.h"
#include "spine/spine.h"
#include "CommonZombies.h"
#include "GameEasterEggs.h"
#include "FirePeaShooter.h"
#include "GlobalVariable.h"
#include "BackGroundLayer.h"
#include "UserWinRequirement.h"
#include "spine\spine-cocos2dx.h"
#include "SelectPlantsGameReadyScene.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::extension;

constexpr auto NOPLANTS = -1;
constexpr auto GrassPosition = 570;

class AdventrueGameScene :public SelectPlantsGameReadyScene
{
public:
	static Scene* createScene();             /* 创建场景 */
	virtual bool init();                     /* 初始化 */
	CREATE_FUNC(AdventrueGameScene);
	Global* global = Global::getInstance();

	AdventrueGameScene()
	{
		ZombiesTag = 0; /* 僵尸标记初始化 */
		/* 初始化地图 */
		global->MapSize.RowNumbers = 5;
		global->MapSize.ColumnNumbers = 9;
		for (int i = 0; i < global->MapSize.RowNumbers; i++)
		{
			for (int j = 0; j < global->MapSize.ColumnNumbers; j++)
			{
				global->MapSize.PlantsMap[i][j] = NOPLANTS;
			}
		}

		/* 初始化是否有不可种植的地方 */
		if (OpenLevelData::getInstance()->LevelData.IsNoPlants)
		{
			for (auto sp : OpenLevelData::getInstance()->LevelData.NoPlantsPosition)
			{
				global->MapSize.PlantsMap[sp.y][sp.x] = 10000;
			}
		}

		GameType::getInstance()->Initialization(); /* 游戏类型初始化 */
		SunFlowers->Initialization();              /* 向日葵初始化 */
		PeaShooters->Initialization();             /* 豌豆射手初始化 */
		CherryBombs->Initialization();             /* 樱桃炸弹初始化 */
		WallNuts->Initialization();                /* 坚果墙初始化 */
		PotatoMines->Initialization();             /* 土豆雷初始化 */
		CabbagePults->Initialization();            /* 卷心菜投手初始化 */
		Torchwoods->Initialization();              /* 火炬树桩初始化 */
		Spikeweeds->Initialization();              /* 地刺初始化 */
		Garlics->Initialization();                 /* 大蒜初始化 */
		FirePeaShooters->Initialization();         /* 火豌豆射手初始化 */
		Jalapenos->Initialization();               /* 火爆辣椒初始化 */
		Cars->Initialization();                    /* 小车初始化 */
		global->Zombies.clear();                   /* 清空僵尸 */
	}
	~AdventrueGameScene()
	{
		delete request;
		this->removeAllChildrenWithCleanup(true);
	}

private:
	/* 存储按钮以及按钮倒计时 */
	typedef struct
	{
		ui::Button* Card[20];                 /* 卡牌 */
		ui::Text* CardText[20];               /* 卡牌文字 */
		ProgressTimer* TimerBar[20];          /* 倒计时 */
		int PlantsNeedSuns[20];               /* 所需阳光 */
		bool TimeBarFinished[20] = { 0 };     /* 倒计时是否完成 */
	}_PlantsButton;
	_PlantsButton PlantsButton;

	/* 鼠标选择的植物 */
	typedef struct
	{
		Sprite* Iamge = nullptr;          /* 图片 */
		int SelectPlantsID;               /* 所选植物编号 */
		bool IsSelectPlants = false;      /* 是否选择了植物 */
		bool IsSelectShovel = false;      /* 是否选择了铲子 */
	}_MouseSelectImage;
	_MouseSelectImage MouseSelectImage;

	/* 植物种植坐标 */
	typedef struct
	{
		int Plants_x;
		int Plants_y;
	}_PlantPlantsPosition;
	_PlantPlantsPosition PlantPlantsPosition;

	/* 僵尸出现波数控制 */
	typedef struct
	{
		int ZombiesPosition[5] = { 130,268,406,544,682 }; /* 僵尸位置 */
		int ZombiesAppearFrequency = 0;                   /* 僵尸出现次数 */
		bool LastFrequencyZombiesWasDeath = false;        /* 上一波僵尸已阵亡 */
		bool IsBegin = false;                             /* 计时是否开始 */
		bool IsShowWords = false;                         /* 是否显示文字 */
		double Time = 0;                                  /* 计时 */
	}_ZombiesAppearControl;
	_ZombiesAppearControl ZombiesAppearControl;

private:
	int ZombiesTag;                                                        /* 僵尸标记 */
	Vec2 Cur;                                                              /* 鼠标坐标 */
	float LevelLastPrecent = 0;                                            /* 关卡上一次进度进度 */
	SkeletonAnimation* SelectPlantsAnimation;                              /* 植物动画 */
	ProgressTimer* progressbar;
	float PlantsCoolTime[15] = 
	{
		7.5f,7.5f,30,35,30,7.5f,7.5f,7.5f,10,10,35
	};                                                                     /* 植物冷却时间 */
	
	int PlantsNeedSuns[15] =
	{
		50,100,50,150,25,100,175,100,50,200,150
	};                                                                     /* 植物所需阳光 */


private:
	/* 创建layer */
	Layer* UiLayer = Layer::create();
	Layer* SunLayer = Layer::create();
	Layer* PeaShooterLayer = Layer::create();
	Layer* WallNutLayer = Layer::create();
	Layer* CherryBombLayer = Layer::create();
	Layer* PotatpMineLayer = Layer::create();
	Layer* CabbageLayer = Layer::create();
	Layer* TorchwoodLayer = Layer::create();
	Layer* SpikeweedLayer = Layer::create();
	Layer* GarlicLayer = Layer::create();
	Layer* FirePeaShooterLayer = Layer::create();
	Layer* JalapenoLayer = Layer::create();
	Layer* ZombiesLayer = Layer::create();
	Layer* CarLayer = Layer::create();

	/* 获取植物单例对象 */
	Director* director = Director::getInstance();
	SunFlower* SunFlowers = SunFlower::getInstance();
	PeaShooter* PeaShooters = PeaShooter::getInstance();
	WallNut* WallNuts = WallNut::getInstance();
	CherryBomb* CherryBombs = CherryBomb::getInstance();
	PotatoMine* PotatoMines = PotatoMine::getInstance();
	CabbagePult* CabbagePults = CabbagePult::getInstance();
	Torchwood* Torchwoods = Torchwood::getInstance();
	Spikeweed* Spikeweeds = Spikeweed::getInstance();
	FirePeaShooter* FirePeaShooters = FirePeaShooter::getInstance();
	Garlic* Garlics = Garlic::getInstance();
	Jalapeno* Jalapenos = Jalapeno::getInstance();

	/* 获取僵尸单例对象 */
	Zombie* zombies = Zombie::getInstance();

	/* 关卡数据单例 */
	OpenLevelData* LevelData = OpenLevelData::getInstance();

	/* 小车 */
	Car* Cars = Car::getInstance();

	Request* request = new Request();

private:
	void Update(float Time);
	void ZombiesUpdateTimeing(float Time);                                                      /* 僵尸更新计时 */
	void CreateLayer(const int& ID);                                                            /* 创建层 */
	void CreateCardButton();                                                                    /* 创建卡牌按钮 */
	void CreateMouse();                                                                         /* 创建鼠标 */
	void CreateBank();                                                                          /* 创建卡槽 */
	void CreateCoin(ui::Button* button);                                                        /* 创建金币 */
	void CoinMove(const Vec2& position, const int& ID);                                         /* 金币移动 */
	void SetTimerBar(ui::Button* Card, const float& Time, const int& ID);                       /* 设置倒计时时间条 */
	void CreateMovePlantsImage(const int& ID);                                                  /* 创建跟随鼠标移动植物 */
	void CalculatePlantPosition();                                                              /* 计算植物种植位置 */
	void ControlCardEnabled();                                                                  /* 控制卡牌是否可用 */
	void MouseMoveControl();                                                                    /* 鼠标移动控制 */
	void MouseDownControl(EventMouse* eventmouse = nullptr);                                    /* 鼠标按下控制 */
	void PlantPlants(const int& ID);                                                            /* 种植植物 */
	void SelectPalntsImage(const int& ID);                                                      /* 选择植物图片 */
	void CreateSelectPlantsSkill(const int& ID);                                                /* 创建选择植物的技能 */
	void DeletePlants(const int& ID, const int& tag);                                           /* 删除植物 */
	void TimeBarFinishedCallBack(Node* node, ui::Button* Card, const int& ID);                  /* 倒计时完成回调函数 */
	void CreateGameWords(const string& name);                                                   /* 创建游戏文字 */
	void CreateZombies();                                                                       /* 创建僵尸 */
	void UserWin();                                                                             /* 用户赢 */
	void SetStop();                                                                             /* 设置停止 */
	bool IsRefurbishZombies();                                                                  /* 是否刷新僵尸 */
	//int JudgeUserIsWin();                                                                       /* 判断玩家是否赢 */
	void GameOver();                                                                            /* 游戏结束 */
	void sliderAction(Ref* sender, Control::EventType);
};