#pragma once
#include "Plants.h"
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class WallNut :public Plants
{
public:

	void Initialization();                                                                                                      /* 初始化 */
	void CreateWallNutImage(const Vec2& position, Node* node);                                                                  /* 创建植物图片 */
	void CreateWallNutAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag);  /* 创建植物动画 */
	void CreateWallNutListener(Node* node);
	static WallNut* getInstance();

private:
	void Update(float Time, Node* node);
	void JudgeWallNutAndZombies(Node* node);

public:
	/* 坚果墙基本属性 */
	typedef struct
	{
		SkeletonAnimation* Animation; /* 动画 */
		float BloodVolume = 300;      /* 血量 */
		Vec2 position;                /* 位置 */
		Vec2 RowAndColumn;            /* 详细地址行列 */
		int skinID;                   /* 皮肤编号 */
		int tag;                      /* 标记 */
	}_WallNutAnimation;
	std::map<int, _WallNutAnimation>WallNutAnimations;
	std::map<int, _WallNutAnimation>::iterator WallNutIter;

private:
	static WallNut* _instance;
	Sprite* WallNutImage;
	Zombie* zombie = Zombie::getInstance();
};
