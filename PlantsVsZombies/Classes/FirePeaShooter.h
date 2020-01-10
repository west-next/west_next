#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class FirePeaShooter :public Plants
{
public:

	static FirePeaShooter* getInstance();
	void Initialization();                                                                                                         /* 初始化 */
	void CreateFirePeaShooterImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateFirePeaShooterAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateFirePeaShooterListener(Node* node);

public:
	/* 豌豆射手基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		bool IsChanged = false;       /* 是否改变 */
		bool IsCreatePea = false;     /* 是否创建豌豆 */
		int tag;                      /* 标记 */
	}_FirePeaShooterAnimation;
	std::map<int, _FirePeaShooterAnimation>FirePeaShooterAnimations;
	std::map<int, _FirePeaShooterAnimation>::iterator FirePeaShooterIter;

	/* 豌豆属性 */
	typedef struct
	{
		SkeletonAnimation* Pea;       /* 动画 */
		int tag;                      /* 标记 */
		float Attack = 40;            /* 攻击力 */
		bool IsUsed = false;          /* 是否使用 */
	}_FirePea;
	std::map<int, _FirePea>FirePeas;
	std::map<int, _FirePea>::iterator FirePeaIter;

public:
	int FirePeaTag = 0;                  /* 豌豆标记 */

private:
	static FirePeaShooter* _instance;    /* 单例 */
	Sprite* FirePeaShooterImage;
	Zombie* zombie = Zombie::getInstance();

private:
	void Update(float Time, Node* node);
	void CreatePea(const Vec2& position, Node* node);                                 /* 创建豌豆 */
	void CreatePeaExplode(const Vec2& position, Node* node, const int& ID = 0);       /* 创建豌豆爆炸动画 */
	float SetZOrder(const float& pos_y);                                              /* 设置豌豆绘制次序 */

	void JudgeFirePeaShooterAndZombiesPosition(Node* node);           /* 豌豆射手与僵尸位置判断 */
	void JudgeFirePeaAndZombiesPosition(Node* node);                  /* 豌豆与僵尸位置判断 */

};