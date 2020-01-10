#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class Spikeweed :public Plants
{
public:
	void Initialization();                                                                                                        /* 初始化 */
	void CreateSpikeweedImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateSpikeweedAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateSpikeweedListener(Node* node);
	static Spikeweed* getInstance();

public:
	/* 地刺基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		bool IsChanged = false;       /* 是否改变 */
		bool IsHaveZombies = false;   /* 是否有僵尸 */
		bool IsAttack = true;         /* 是否攻击 */
		int tag;                      /* 标记 */
		int Attack = 20;              /* 攻击力 */
	}_SpikeweedAnimation;
	std::map<int, _SpikeweedAnimation>SpikeweedAnimation;
	std::map<int, _SpikeweedAnimation>::iterator SpikeweedIter;

private:
	static Spikeweed* _instance;    /* 单例 */
	Sprite* SpikeweedImage;
	Zombie* zombie = Zombie::getInstance();

private:
	void Update(float Time, Node* node);
	void JudgePeaShooterAndZombiesPosition(Node* node);           /* 地刺与僵尸位置判断 */
	void PlayAttractMusic(const int& MusicType);               /* 播放地刺攻击声音 */
};