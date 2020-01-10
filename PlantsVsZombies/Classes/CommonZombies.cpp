#include "CommonZombies.h"

CommonZombies* CommonZombies::_instance = nullptr;

CommonZombies* CommonZombies::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)CommonZombies;
	}
	return _instance;
}

void CommonZombies::CreateCommonZombies(Node* node, const Vec2& Position, const int& AnimationID, const int& tag)
{
	static string AnimationName[] = { {"Zombies_Stand"},{"Zombies_Stand1"} ,{"Zombies_Walk"},{"Zombies_Walk2"},{"Zombies_Walk2"} };

	auto animation = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("Zombies")->second);
	animation->setPosition(Position);
	animation->setContentSize(Size(40, 40));
	animation->setAnchorPoint(Vec2(0, 0));
	animation->setTag(tag);
	animation->setTimeScale(0.8f);
	animation->addEmptyAnimation(0, rand() % 2, rand() % 2);
	animation->addAnimation(0, AnimationName[AnimationID], true);
	animation->setGlobalZOrder(SetZOrder(Position.y));
	animation->setOpacity(0);
	node->addChild(animation);

	/* 创建僵尸影子 */
	auto shadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
	shadow->setScale(2.0f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, 10));
	shadow->setOpacity(0);
	animation->addChild(shadow, -1);

	animation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");

	Global::_Zombies zombies;
	zombies.BloodVolume = 200;
	zombies._BloodVolume = 200;
	zombies.Speed = 30;
	zombies.CaveSpeed = 30;
	zombies.ZombiesAnimation = animation;
	zombies.ShieldVolume = 0;
	zombies.ShieldVolume1 = 0;
	zombies.ZombiesType = Global::ZombiesType::CommonZombies;
	zombies.MusicType = 0;
	zombies.AnimationID = 5;
	zombies.IsHaveShield = false;
	zombies.IsEat = false;
	zombies.IsUseForGameType = false;
	zombies.Tag = tag;

	global->Zombies.insert(pair<int, Global::_Zombies>(tag, zombies));
}

void CommonZombies::CreateCommonZombiesAnimation(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.BloodVolume <= iter->second._BloodVolume / 2.0f)  /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		SetZombiesPrimaryInjure(iter, node);
	}
	if (iter->second.BloodVolume <= 0)   /* 僵尸血量小于零，僵尸死亡 */
	{
		SetZombiesSecondaryInjure(iter, node);
	}
}

void CommonZombies::ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	iter->second.ZombiesAnimation->setVisible(false);

	/* 僵尸掉头 */
	ZombiesLoseHeadAnimation(iter, node, "CommonZombieHead");

	/* 僵尸掉胳膊 */
	ZombiesLoseArmAnimation(iter, node, "ZombieArm");

	/* 僵尸掉身体 */
	//ZombiesBodyAnimation(iter, node, "ZombiesBody");
}
