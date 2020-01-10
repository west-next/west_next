 #pragma once
#include "Zombies.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "AudioEngine.h"
#include "GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class CommonZombies :public Zombie
{
public:
	/* µ¥ÀýÄ£Ê½ */
	static CommonZombies* getInstance();
	void CreateCommonZombies(Node* node, const Vec2& Position, const int& AnimationID, const int& tag);
	void CreateCommonZombiesAnimation(map<int, Global::_Zombies>::iterator iter, Node* node);
	void ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node);
	
private:
	static CommonZombies* _instance;

};