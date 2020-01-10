#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class Jalapeno :public Plants
{
public:
	static Jalapeno* getInstance();
	void Initialization();                                                                                                         /* 初始化 */
	void CreateJalapenoImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateJalapenoAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateJalapenoListener(Node* node);

public:

	/* 火爆辣椒基本属性 */
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
	}_JalapenoAnimation;
	std::map<int, _JalapenoAnimation>JalapenoAnimation;
	std::map<int, _JalapenoAnimation>::iterator JalapenoIter;

	std::list<int>JalapenoReady;

private:
	static Jalapeno* _instance;    /* 单例 */
	Sprite* JalapenoImage;
	Zombie* zombie = Zombie::getInstance();

private:
	int SetZorder(const float& position);                                                            /* 设置贴图顺序 */
	void Update(float Time, Node* node);
	void UpdateJalapenoReady();                                                                      /* 更新樱桃炸弹准备 */
	void JudgeJalapenoAndZombiesPosition(Node* node);                                                /* 火爆辣椒与僵尸位置判断 */
	void PlayJalapenoExplode(Node* node, map<int, _JalapenoAnimation>::iterator JalapenoIter);       /* 播放火爆辣椒爆炸动画 */
	void JalapenoExplodeKillZombies(Node* node,
		map<int, _JalapenoAnimation>::iterator JalapenoIter,
		map<int, Global::_Zombies>::iterator Iter);                                                  /* 判断火爆辣椒杀死僵尸 */
};