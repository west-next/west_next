#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "PeaShooter.h"
#include "spine/spine.h"
#include "FirePeaShooter.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class Torchwood :public Plants
{
public:
	void Initialization();                                                                                                        /* 初始化 */
	void CreateTorchwoodImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateTorchwoodAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateTorchwoodListener(Node* node);
	static Torchwood* getInstance();

public:
	/* 火炬树桩基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		int tag;                      /* 标记 */
	}_TorchwoodAnimation;
	std::map<int, _TorchwoodAnimation>TorchwoodAnimation;
	std::map<int, _TorchwoodAnimation>::iterator TorchwoIter;

private:
	static Torchwood* _instance;    /* 单例 */
	Sprite* TorchwoodImage;
	PeaShooter* PeaShooters = PeaShooter::getInstance();
	FirePeaShooter* FirePeaShooters = FirePeaShooter::getInstance();
	Zombie* zombie = Zombie::getInstance();

private:
	void Update(float Time, Node* node);
	void JudgeTorchwoodAndZombiesPosition(Node* node); /* 僵尸与火炬树桩位置判断 */
	void JudgeTorchwoodAndPeaPosition(Node* node);     /* 豌豆与火炬树桩位置判断 */
};