#pragma once
#include <map>
#include <vector>
#include <string>
#include "cocos2d.h"
#include "Zombies.h"
#include "LevelData.h"
#include "cocos-ext.h"
#include "AudioEngine.h"
#include "spine/spine.h"
#include "ui/CocosGUI.h"
#include "CommonZombies.h"
#include "GlobalVariable.h"
#include "UserWinRequirement.h"
#include "spine\spine-cocos2dx.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class SelectPlantsGameReadyScene :public Scene
{
public:
	static Scene* createScene();            /* 创建场景 */
	virtual bool init();                    /* 初始化 */
	CREATE_FUNC(SelectPlantsGameReadyScene);
	
	SelectPlantsGameReadyScene()
	{
		
	}

public:
	/* 预览僵尸 */
	typedef struct
	{
		SkeletonAnimation* PreviewZombie;
		float PositionY;
	}_PreViewZombies;
	std::vector< _PreViewZombies> PreviewZombies;

private:

	Sprite* SeedChooser;                    /* 卡牌选择栏 */
	extension::ScrollView* scrollView;      /* 滚动视图 */
	SkeletonAnimation* PlantsAnimation;     /* 植物动画 */
	ui::Text* AnimationText[15];            /* 动画说明 */

	ui::ScrollView* PlantCardScrollView;    /* 植物卡牌滚动视图 */
	ui::ScrollView* PlantCardTextScrollView;/* 植物卡牌滚动视图 */
	float PlantCardScrollViewPercent;       /* 植物卡牌滚动视图滚动距离 */
	float PlantCardScroll;                  /* 计算鼠标滚动距离 */
	float PlantCardTextScroll;              /* 计算鼠标滚动距离 */

	Global* global = Global::getInstance();
	Director* director = Director::getInstance();
	Zombie* zombies = Zombie::getInstance();
	OpenLevelData* LevelData = OpenLevelData::getInstance();

	typedef struct
	{
		/* 卡牌按钮 */
		ui::Button* button;
		/* 卡牌数量 */
		int size = 0;
		/* 卡牌的编号 */
		int ID;
	}SeedBankButton;
	SeedBankButton seedbankbutton[10];

protected:
	/* 创建背景僵尸 */
	void CreatePreviewZombies(Sprite* name);
	/* 创建火焰粒子动画 */
	void CreateFire(Sprite* sprite);
	/* 创建菜单按钮 */
	void CreateMenubutton(const int& ID);
    /* 显示植物与需要的阳光数 */
	ui::Text* ShowPlantsAndNeedSunNumbers(ui::Button* button,const int& ID);

private:
	/* 定时函数 */
	void Update(float Time);
	void Update1(float Time);
	void ScrollViewUpdate(float Time);
	
    /* 为植物卡牌排序 */
	void SortPlantsCard(const int& ID);
	/* 为僵尸排序 */
	void SortZombies();
	
	/* 预加载字体 */
	void PreloadText();

private:

	/* 创建背景 */
	void CreateBackGround();                                                 
	
	/* 创建对话框 */
	void CreateDialogs();

	/* 创建用户信息 */
	void CreateUserName(const std::string &);

	/* 创建倒计时文字 */
	void CreateReadyText(const std::string&,const int &);

	/* 创建植物动画 */
	void CreatePlantsAnimation(const int& ID);
	void CreateAnimation(const std::string& filepath, const std::string& AnimationName, 
		const std::string &skin, Vec2& vec2, const float& scale = 1.0f);
	void CreateAnimationText(const int& ID, const std::string& name, const Vec2& vec2,
		const float& fontsize, Color3B color = Color3B::WHITE, bool AnchorPoint = true);

	/* 创建植物 */
	void CreateSprite(ui::Button* button, const std::string resource, const float& scale = 0.8f);

	/* 创建植物卡牌滑动视图 */
	void CreatePlantsCardScrollView();

	/* 创建卡牌按钮 */
	ui::Button* CreateButtons(const Vec2& vec2, const int& ID);

	/* 创建移动卡牌按钮 */
	void CreateMoveButton(ui::Button* button, const Vec2& vec2, const int& ID);

private:

	/* 删除植物卡牌回调函数 */
	void RemovePlantsCardCallBack(Node* node,const int& ID, ui::Button* button);
	
	/* 开始选植物回调 */
	void SelectPlantsCallBack(Node* node,const int& ID);   

	/* 准备游戏倒计时回调函数 */
	void ReadyGameCallBack(Node* node, const std::string &,const int &);

};
