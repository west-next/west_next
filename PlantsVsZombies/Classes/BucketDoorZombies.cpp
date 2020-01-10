#include "BucketDoorZombies.h"

BucketDoorZombies* BucketDoorZombies::_instance = nullptr;

BucketDoorZombies* BucketDoorZombies::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)BucketDoorZombies;
	}
	return _instance;
}

void BucketDoorZombies::CreateBucketDoorZombies(Node* node, const Vec2& Position, const int& tag, const int& AnimationID, const bool& IsReadyScene)
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

	switch (IsReadyScene)
	{
	case true:
		animation->setAnimation(0, "Zombies_Door_Stand", true);
		break;
	case false:
		animation->setAnimation(0, "Zombies_Door_Walk", true);
		break;
	}
	
	animation->setAttachment("Zombie_bucket", "Zombie_bucket1");
	animation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");

	Global::_Zombies zombies;
	zombies.BloodVolume = 200;
	zombies.ShieldVolume = 1200;
	zombies.ShieldVolume1 = 1200;
	zombies._BloodVolume = 200;
	zombies._ShieldVolume = 1200;
	zombies._ShieldVolume1 = 1200;
	zombies.Speed = 30;
	zombies.CaveSpeed = 30;
	zombies.ZombiesAnimation = animation;
	zombies.ZombiesType = Global::ZombiesType::BucketDoorZombies;
	zombies.MusicType = 1;
	zombies.AnimationID = 1;
	zombies.IsHaveShield = true;
	zombies.IsEat = false;
	zombies.IsUseForGameType = false;
	zombies.Tag = tag;

	global->Zombies.insert(pair<int, Global::_Zombies>(tag, zombies));
}

void BucketDoorZombies::CreateBucketDoorZombiesAniamton(map<int, Global::_Zombies>::iterator Iter, Node* node)
{
	if (Iter->second.IsHaveShield) /* 如果有护盾 */
	{
		if (Iter->second.ShieldVolume1 <= Iter->second._ShieldVolume1 * 2.0f / 3) /* 护盾一级损伤 */
		{
			SetShield1PrimaryInjure(Iter, node);
		}
		if (Iter->second.ShieldVolume1 <= Iter->second._ShieldVolume1 * 1.0f / 3) /* 护盾二级损伤 */
		{
			SetShield1SecondaryInjure(Iter, node);
		}
		if (Iter->second.ShieldVolume1 <= 0)   /* 护盾消失 */
		{
			SetShield1ThirdInjure(Iter, node);
		}

		if (Iter->second.ShieldVolume <= Iter->second._ShieldVolume * 2.0f / 3) /* 护盾一级损伤 */
		{
			SetShieldPrimaryInjure(Iter, node);
		}
		if (Iter->second.ShieldVolume <= Iter->second._ShieldVolume * 1.0f / 3) /* 护盾二级损伤 */
		{
			SetShieldSecondaryInjure(Iter, node);
		}
		if (Iter->second.ShieldVolume <= 0)   /* 护盾消失 */
		{
			SetShieldThirdInjure(Iter, node);
		}
	}
	if (Iter->second.BloodVolume <= Iter->second._BloodVolume / 2.0f)  /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		SetZombiesPrimaryInjure(Iter, node);
	}
	if (Iter->second.BloodVolume <= 0)   /* 僵尸血量小于零，僵尸死亡 */
	{
		SetZombiesSecondaryInjure(Iter, node);
	}
}

void BucketDoorZombies::SetShieldPrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID <= 4 && iter->second.AnimationID >= 1) /* 护盾一级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket2");
		iter->second.AnimationID = 5;
	}
}

void BucketDoorZombies::SetShieldSecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 5) /* 护盾二级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket3");
		iter->second.AnimationID = 6;
	}
}

void BucketDoorZombies::SetShieldThirdInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 6) /* 护盾三级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		iter->second.AnimationID = 5;
		iter->second.IsHaveShield = false;
		iter->second.MusicType = 0;

		/* 僵尸掉护盾 */
		ZombiesLoseShieldAnimation(iter, node, "Zombie_bucket");
	}
}

void BucketDoorZombies::SetShield1PrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 1) /* 护盾1一级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor2");
		iter->second.AnimationID = 2;
	}
}

void BucketDoorZombies::SetShield1SecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 2) /* 护盾1二级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor3");
		iter->second.AnimationID = 3;
	}
}

void BucketDoorZombies::SetShield1ThirdInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 3) /* 护盾1三级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		iter->second.AnimationID = 4;

		iter->second.ZombiesAnimation->addAnimation(0, "Zombies_Walk", true);
		/* 僵尸掉护盾1 */
		//ZombiesLoseShieldAnimation(iter, node, "Zombie_screendoor3");
	}
}

void BucketDoorZombies::ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	iter->second.ZombiesAnimation->setVisible(false);


	/* 僵尸掉护盾 */
	if (iter->second.IsHaveShield)
	{
		ZombiesLoseShieldAnimation(iter, node, "Zombie_bucket");
	}

	/* 僵尸掉头 */
	ZombiesLoseHeadAnimation(iter, node, "CommonZombieHead");

	/* 僵尸掉胳膊 */
	ZombiesLoseArmAnimation(iter, node, "ZombieArm");
}
