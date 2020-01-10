#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class CabbagePult :public Plants
{
public:
	static CabbagePult* getInstance();
	void Initialization();                                                                                                         /* 初始化 */
	void CreateCabbagePultImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateCabbagePultAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateCabbagePultListener(Node* node);

public:
	/* 卷心菜投手基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		bool IsChanged = false;       /* 是否改变 */
		bool IsCreateCabbage = false; /* 是否创建卷心菜 */
		int tag;                      /* 标记 */
	}_CabbagePultAnimation;
	std::map<int, _CabbagePultAnimation>CabbagePultAnimations;
	std::map<int, _CabbagePultAnimation>::iterator CabbagePultIter;

	/* 卷心菜属性 */
	typedef struct
	{
		SkeletonAnimation* CabbageBullet;   /* 图像 */
		int tag;                            /* 标记 */
		bool IsUse;                         /* 是否使用 */
		float Attack = 40;                  /* 攻击力 */
		float PositionY;                    /* 坐标 */
	}_CabbageBullet;
	std::map<int, _CabbageBullet>CabbageBullet;
	std::map<int, _CabbageBullet>::iterator CabbageBulletIter;

public:
	int CabbageBulletTag = 0;               /* 卷心菜子弹标记 */

private:
	static CabbagePult* _instance;          /* 单例 */
	Sprite* CabbagePultImage;
	Zombie* zombie = Zombie::getInstance();
	Global* global = Global::getInstance();
	 
private:

	void Update(float Time, Node* node);
	void JudgeCabbagePultAndZombiesPosition(Node* node);
	void JudgeCabbageBulletAndZombiesPosition(Node* node);                                                                                /* 豌豆与僵尸位置判断 */
	void CreateCabbage(const Vec2& CabbagePosition, const Vec2& ZombiesPosition, Node* node, map<int, Global::_Zombies>::iterator Iter);
	void CreateCabbageExplode(const Vec2& position, Node* node);                                                                          /* 创建卷心菜爆炸动画 */
	float SetZOrder(const float& pos_y);                                                                                                  /* 设置豌豆绘制次序 */
	void PlayPeaExplodeMusic(const int& MusicType);                                                                                       /* 播放卷心菜爆炸声音 */
};