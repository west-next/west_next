#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class SunFlower :public Plants
{
public:

	void Initialization();                                                                                                           /* 初始化 */
	void CreateSunFlowerImage(const Vec2& position, Node* node);                                                                     /* 创建植物图片 */
	void CreateSunFlowerAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);     /* 创建植物动画 */
	void CreateSunListener(Node* node);                                                                                              /* 创建阳光监听 */
	void ShowSunNumbers(Node* node);                                                                                                 /* 显示阳光数 */
	void ShowCoinNumbers(Node* node);                                                                                                /* 显示金币数 */
	void RandomCreateSuns(Node* node);                                                                                               /* 随机产生阳光 */
	void RandomCreateCoins(Node* node, const Vec2& position);                                                                        /* 随机产生金币 */
	void SunNumberTextRunAction(Node* node);                                                                                         /* 阳光数颜色变化 */
	void UpdateSunNumbers(Node* node = nullptr);                                                                                     /* 更新阳光数 */
	void UpdateCoinNumbers(Node* node);                                                                                              /* 更新金币数 */

	/* 单例模式 */
	static SunFlower* getInstance();

private:
	void Update(float Time, Node* node);                                                                   /* 更新阳光数 */
	void CreateSun(Node* node, SkeletonAnimation* animation, const Vec2& position, const int& ID);
	void RemoveSunCallBack(Node* node, Node* node1, const int& tag);                                       /* 删除阳光回调函数 */
	void JudgeSunFlowerAndZombies(Node* node);                                                             /* 判断向日葵与僵尸 */
	
public:
	
	/* 向日葵基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		int tag;                      /* 标记 */
	}_SunFlowerAnimation;
	std::map<int, _SunFlowerAnimation>SunFlowerAnimations;
	std::map<int, _SunFlowerAnimation>::iterator SunflowerIter;

	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		bool IsUsed = false;          /* 是否使用 */
	}_Suns;
	std::map<int, _Suns> Suns;                        /* 阳光存储 */
	std::map<int, _Suns>::iterator SunsIter;

	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		bool IsUsed = false;          /* 是否使用 */
	}_Coins;
	std::list<_Coins> Coins;                          /* 金币存储 */
	std::list<_Coins>::iterator CoinIter;

public:
	int suntag = 0;                                   /* 太阳标记 */
	int cointag = 10000;                              /* 银币标记 */

private:
	static SunFlower* _instance;
	Sprite* SunFlowerImage;
	ui::Text* SunNumbersText;
	ui::Text* CoinNumbersText;
	Zombie* zombie = Zombie::getInstance();

};
