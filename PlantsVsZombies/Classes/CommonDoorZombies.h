#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "AudioEngine.h"
#include "CommonZombies.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class CommonDoorZombies :public CommonZombies
{
public:
	/* 单例模式 */
	static CommonDoorZombies* getInstance();
	void CreateCommonDoorZombies(Node* node, const Vec2& Position, const int& tag, const int& AnimationID, const bool& IsReadyScene = false);
	void CreateCommonDoorZombiesAniamton(map<int, Global::_Zombies>::iterator iter, Node* node);
	void ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node);

private:
	static CommonDoorZombies* _instance;

private:
	void SetShieldPrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node);                   /* 设置护盾一级损伤 */
	void SetShieldSecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node);                 /* 设置护盾二级损伤 */
	void SetShieldThirdInjure(map<int, Global::_Zombies>::iterator iter, Node* node);                     /* 设置护盾三级损伤 */
};