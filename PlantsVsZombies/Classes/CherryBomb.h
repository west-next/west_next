#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class CherryBomb:public Plants
{
public:

	void Initialization();                                                                                                         /* 初始化 */
	void CreateCherryBombImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateCherryBombAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateCherryBombListener(Node* node);
	static CherryBomb* getInstance();

public:

	/* 樱桃炸弹基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		int ExplosiveDamage;          /* 爆炸伤害 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		bool IsReadyExplode;          /* 是否准备爆炸 */
		bool IsExplode;               /* 是否爆炸 */
		int tag;                      /* 标记 */
	}_CherryBombAnimation;
	std::map<int, _CherryBombAnimation>CherryBombAnimation;
	std::map<int, _CherryBombAnimation>::iterator CherryBombIter;

	std::list<int>CherryBombReady;

private:
	static CherryBomb* _instance;    /* 单例 */
	Sprite* CherryBombImage;
	Zombie* zombie = Zombie::getInstance();

private:
	int SetZorder(const float& position);                                                            /* 设置贴图顺序 */
	void Update(float Time, Node* node);
	void UpdateCherryBombReady();                                                                    /* 更新樱桃炸弹准备 */
	void JudgeCherryBombAndZombiesPosition(Node* node);                                              /* 樱桃炸弹与僵尸位置判断 */
	void PlayCherryBombExplode(Node* node, map<int, _CherryBombAnimation>::iterator CherryBombIter); /* 播放樱桃炸弹爆炸动画 */
	void CherryBombExplodeKillZombies(Node* node, 
		map<int, _CherryBombAnimation>::iterator CherryBombIter, 
		map<int, Global::_Zombies>::iterator Iter);                                                  /* 判断樱桃炸弹杀死僵尸 */
};