#include "LpmZombies.h"

LmpZombies* LmpZombies::_instance = nullptr;

LmpZombies* LmpZombies::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)LmpZombies;
	}
	return _instance;
}

void LmpZombies::CreateLmpZombies(Node* node, const Vec2& Position, const int& tag, const int& AnimationID, const bool& IsReadyScene)
{
	static string AnimationName[] = { {"Zombies_Stand"},{"Zombies_Walk"}};

	auto animation = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("LmpZombies")->second);
	animation->setPosition(Position);
	animation->setScale(0.8f);
	animation->setAnchorPoint(Vec2(0, 0));
	animation->setTag(tag);
	animation->setTimeScale(0.8f);
	animation->addEmptyAnimation(0, rand() % 2, rand() % 2);
	animation->addAnimation(0, AnimationName[IsReadyScene == true ? 0 : 1], true);
	animation->setGlobalZOrder(SetZOrder(Position.y));
	animation->setOpacity(0);
	node->addChild(animation);

	/* ´´½¨½©Ê¬Ó°×Ó */
	auto shadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
	shadow->setScale(2.0f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, 10));
	shadow->setOpacity(0);
	animation->addChild(shadow, -1);

	Global::_Zombies zombies;
	zombies.BloodVolume = 200;
	zombies._BloodVolume = 200;
	zombies.Speed = 40;
	zombies.CaveSpeed = 40;
	zombies.ZombiesAnimation = animation;
	zombies.ShieldVolume = 0;
	zombies.ShieldVolume1 = 0;
	zombies.ZombiesType = Global::ZombiesType::LmpZombies;
	zombies.MusicType = 0;
	zombies.AnimationID = 5;
	zombies.IsHaveShield = false;
	zombies.IsEat = false;
	zombies.IsUseForGameType = false;
	zombies.Tag = tag;

	global->Zombies.insert(pair<int, Global::_Zombies>(tag, zombies));
}

void LmpZombies::CreateLmpZombiesAniamton(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.BloodVolume <= iter->second._BloodVolume / 2.0f)  /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÒ»¶¨Öµ£¬½©Ê¬µô¸ì²² */
	{
		LmpZombies::SetZombiesPrimaryInjure(iter, node);
	}
	if (iter->second.BloodVolume <= 0)   /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÁã£¬½©Ê¬ËÀÍö */
	{
		LmpZombies::SetZombiesSecondaryInjure(iter, node);
	}
}

void LmpZombies::ZombiesKilledByCar(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	iter->second.ZombiesAnimation->setVisible(false);

	/* ½©Ê¬µôÍ· */
	ZombiesLoseHeadAnimation(iter, node, "ZombieImpHead");

	/* ½©Ê¬µô¸ì²² */
	ZombiesLoseArmAnimation(iter, node, "LmpZombiesArm");
}

void LmpZombies::SetZombiesPrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID <= 5 && iter->second.AnimationID >= 1) /* ½©Ê¬¿ªÊ¼µô¸ì²² */
	{
		iter->second.ZombiesAnimation->setAttachment("13", "0");
		iter->second.ZombiesAnimation->setAttachment("4", "0");
		iter->second.ZombiesAnimation->setAttachment("18", "9");
		iter->second.AnimationID = 6;

		ZombiesLoseArmAnimation(iter, node, "LmpZombiesArm");
	}
}

void LmpZombies::SetZombiesSecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 6)
	{
		iter->second.ZombiesAnimation->setAttachment("3", "0");
		iter->second.ZombiesAnimation->setAttachment("14", "0");
		iter->second.ZombiesAnimation->setAnimation(1, "Zombies_Die", false);
		iter->second.AnimationID = 10;

		ZombiesLoseHeadAnimation(iter, node, "ZombieImpHead");

		auto animation = iter->second.ZombiesAnimation;
		auto const speed = iter;
		iter->second.ZombiesAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
			{
				if (!strcmp(event->data->name, "filldown"))
				{
					speed->second.Speed = 0; /* Í£Ö¹ÔË¶¯ */
					rand() % 2 == 0 ?
						AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("zombie_falling_1")->second), global->GlobalMusic.SoundEffect) :
						AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("zombie_falling_2")->second), global->GlobalMusic.SoundEffect);
				}
				if (!strcmp(event->data->name, "die"))
				{
					animation->runAction(Sequence::create(FadeOut::create(1.0f), 
						CallFunc::create([=]() 
							{
								animation->setVisible(false);
								animation->pause();
								animation->stopAllActions();
							}), nullptr));
				}
			});
	}
}
