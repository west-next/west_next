#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class PotatoMine :public Plants
{
public:
	static PotatoMine* getInstance();
	void Initialization();                                                                                                         /* 初始化 */
	void CreatePotatoMineImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreatePotatoMineAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreatePotatoMineListener(Node* node);

public:
	/* 土豆雷基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		bool IsReady = false;         /* 是否准备好 */
		bool IsExplode = false;       /* 是否爆炸 */
		bool IsFirst = false;         /* 是否第一次 */
		int ExplosiveDamage;          /* 土豆雷爆炸伤害 */
		int RecordSecond;             /* 记录秒数 */
		int BreakGround;              /* 破土时间 */
		int tag;                      /* 标记 */
	}_PotatoMineAnimation;
	std::map<int, _PotatoMineAnimation>PotatoMineAnimations;
	std::map<int, _PotatoMineAnimation>::iterator PotatoMineIter;

public:
	static PotatoMine* _instance;    /* 单例 */
	Sprite* PotatoMineImage;

private:
	void Update(float Time, Node* node);
	void UpdatePotateMine(float time);
	void JudgePeaShooterAndZombiesPosition(Node* node);                                                                                        /* 土豆雷与僵尸位置判断 */
	void PotatoMineExplodeKillZombies(Node* node, map<int, _PotatoMineAnimation>::iterator PotatoMineIter);                                    /* 土豆雷爆炸击杀僵尸 */
	void StrikeFlyZombies(Node* node, const string& NameHead = "");
};