#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class PeaShooter :public Plants
{
public:

	void Initialization();                                                                                                         /* 初始化 */
	void CreatePeaShooterImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreatePeaShooterAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreatePeaShooterListener(Node* node);
	static PeaShooter* getInstance();

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
	}_PeaShooterAnimation;
	std::map<int, _PeaShooterAnimation>PeaShooterAnimations;
	std::map<int, _PeaShooterAnimation>::iterator iter;

	/* 豌豆属性 */
	typedef struct
	{
		SkeletonAnimation* Pea;       /* 动画 */
		int tag;                      /* 标记 */
		int FireNumbers = 0;          /* 燃烧次数 */
		int TorchwoodTag = 0;         /* 火炬树桩标记 */
		float Attack = 20;            /* 攻击力 */
		bool IsUsed = false;          /* 是否使用 */
		bool IsFire = false;          /* 是否燃烧 */
	}_Pea;
	std::map<int, _Pea>Peas;
	std::map<int, _Pea>::iterator PeaIter;

public:
	int PeaTag = 0;                  /* 豌豆标记 */

private:
	static PeaShooter* _instance;    /* 单例 */
	Sprite* PeaShooterImage;
	Zombie* zombie = Zombie::getInstance();

private:
	void Update(float Time,Node* node);
	void CreatePea(const Vec2& position, Node* node);                                 /* 创建豌豆 */
	void CreatePeaExplode(const Vec2& position, Node* node, const int& ID = 0);       /* 创建豌豆爆炸动画 */
	float SetZOrder(const float& pos_y);                                              /* 设置豌豆绘制次序 */

	void PlayPeaExplodeMusic(const int& MusicType);               /* 播放豌豆爆炸声音 */
	void JudgePeaShooterAndZombiesPosition(Node* node);           /* 豌豆射手与僵尸位置判断 */
	void JudgePeaAndZombiesPosition(Node* node);                  /* 豌豆与僵尸位置判断 */

};
