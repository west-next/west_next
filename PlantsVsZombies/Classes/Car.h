#pragma once
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;

class Car
{
public:
	static Car* getInstance();
	void Initialization();                                        /* 初始化 */
	void CreateCarAnimation(Node* node, const Vec2& position);
	void CreateCarListener(Node* node);
	float SetZOrder(const float& pos_y);                          /* 设置小车绘制次序 */

	/* 小车基本属性 */
	typedef struct
	{
		Sprite* Image;                /* 动画 */
		Vec2 position;                /* 位置 */
		bool IsLive;                  /* 是否激活 */
		int AnimationID;              /* 动画ID */
		int tag;                      /* 标记 */
	}_Car;
	std::map<int, _Car>Cars;
	std::map<int, _Car>::iterator cariter;

private:
	static Car* _instance;           /* 单例 */
	Global* global = Global::getInstance();

private:
	void Update(float Time, Node* node);
	void JudgeCarAndZombies(Node* node);
	void CarMove(Node* node);
};