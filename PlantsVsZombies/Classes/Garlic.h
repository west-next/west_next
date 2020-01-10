#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class Garlic :public Plants
{
public:
	static Garlic* getInstance();
	void Initialization();                                                                                                         /* 初始化 */
	void CreateGarlicImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateGarlicAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateGarlicListener(Node* node);
	
public:
	/* 大蒜基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		int EatNumbers = 10;          /* 被吃的次数 */
		int SumEatNumbers = 10;       /* 可以被吃的次数 */
		float BloodVolume = 200000;   /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		int tag;                      /* 标记 */
		int AnimationID = 0;          /* 动画标记 */
	}_GarlicAnimation;
	std::map<int, _GarlicAnimation>GarlicAnimations;
	std::map<int, _GarlicAnimation>::iterator GarlicIter;

private:
	static Garlic* _instance;    /* 单例 */
	Sprite* GarlicImage;
	Zombie* zombie = Zombie::getInstance();

private:
	void Update(float Time, Node* node);
	void JudgeGarlicAndZombiesPosition(Node* node);           /* 大蒜与僵尸位置判断 */
};