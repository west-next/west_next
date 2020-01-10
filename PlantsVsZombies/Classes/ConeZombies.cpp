#include "ConeZombies.h"

ConeZombies* ConeZombies::_instance = nullptr;

ConeZombies* ConeZombies::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)ConeZombies;
	}
	return _instance;
}

void ConeZombies::CreateConeZombies(Node* node, const Vec2& Position, const int& AnimationID, const int& tag)
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

	animation->setAttachment("Zombie_bucket", "Zombie_cone1");

	Global::_Zombies zombies;
	zombies.BloodVolume = 200;
	zombies.ShieldVolume = 600;
	zombies._BloodVolume = 200;
	zombies._ShieldVolume = 600;
	zombies.Speed = 30;
	zombies.CaveSpeed = 30;
	zombies.ZombiesAnimation = animation;
	zombies.ShieldVolume1 = 0;
	zombies.ZombiesType = Global::ZombiesType::ConeZombies;
	zombies.MusicType = 0;
	zombies.AnimationID = 1;
	zombies.IsHaveShield = true;
	zombies.IsEat = false;
	zombies.IsUseForGameType = false;
	zombies.Tag = tag;

	global->Zombies.insert(pair<int, Global::_Zombies>(tag, zombies));
}

void ConeZombies::CreateConeZombiesAniamton(map<int, Global::_Zombies>::iterator Iter, Node* node)
{
	if (Iter->second.IsHaveShield) /* 如果有护盾 */
	{
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

void ConeZombies::SetShieldPrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 1) /* 护盾一级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone2");
		iter->second.AnimationID = 2;
	}
}

void ConeZombies::SetShieldSecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 2) /* 护盾二级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone3");
		iter->second.AnimationID = 3;
	}
}

void ConeZombies::SetShieldThirdInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 3) /* 护盾三级伤害 */
	{
		iter->second.ZombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		iter->second.AnimationID = 5;
		iter->second.IsHaveShield = false;

		/* 僵尸掉护盾 */
		ZombiesLoseShieldAnimation(iter, node, "Zombie_cone");
	}
}

void ConeZombies::ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	iter->second.ZombiesAnimation->setVisible(false);

	/* 僵尸掉护盾 */
	if (iter->second.IsHaveShield)
	{
		ZombiesLoseShieldAnimation(iter, node, "Zombie_cone");
	}

	/* 僵尸掉头 */
	ZombiesLoseHeadAnimation(iter, node, "CommonZombieHead");

	/* 僵尸掉胳膊 */
	ZombiesLoseArmAnimation(iter, node, "ZombieArm");

	/* 僵尸掉身体 */
	//ZombiesBodyAnimation(iter, node, "ZombiesBody");
}