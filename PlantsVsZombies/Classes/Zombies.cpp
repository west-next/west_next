#include "Zombies.h"
#include "SunFlower.h"
#include "LpmZombies.h"
#include "ConeZombies.h"
#include "BucketZombies.h"
#include "CommonZombies.h"
#include "ConeDoorZombies.h"
#include "BucketDoorZombies.h"
#include "CommonDoorZombies.h"

Zombie* Zombie::_instance = nullptr;

Zombie* Zombie::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Zombie;
	}
	return _instance;
}

void Zombie::CreateZombies(const Vec2& Position, Node* node, const int& tag, const int& AnimationID, const int& ZombiesTyped, const bool& IsReadyScene)
{
	switch (ZombiesTyped)
	{
	case 1:
		CommonZombies::getInstance()->CreateCommonZombies(node, Position, AnimationID, tag);
		break;
	case 2:
		ConeZombies::getInstance()->CreateConeZombies(node, Position, AnimationID, tag);
		break;
	case 3:
		BucketZombies::getInstance()->CreateBucketZombies(node, Position, AnimationID, tag);
		break;
	case 4:
		CommonDoorZombies::getInstance()->CreateCommonDoorZombies(node, Position, tag, AnimationID, IsReadyScene);
		break;
	case 5:
		ConeDoorZombies::getInstance()->CreateConeDoorZombies(node, Position, tag, AnimationID, IsReadyScene);
		break;
	case 6:
		BucketDoorZombies::getInstance()->CreateBucketDoorZombies(node, Position, tag, AnimationID, IsReadyScene);
		break;
	case 7:
		LmpZombies::getInstance()->CreateLmpZombies(node, Position, tag, AnimationID, IsReadyScene);
		break;
	default:
		break;
	}
}

float Zombie::SetZOrder(const float& pos_y)
{
	static int pos[] = { 682,544,406,268,130 };
	for (int i = 0; i < 5; i++)
	{
		if (pos[i] == pos_y)
			return i * 2 + 1;
	}
	return 0;
}

void Zombie::ZombiesUpdate(Node* node)
{
	node->schedule(CC_CALLBACK_1(Zombie::Update, this, node), "Update");
	node->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([=]()
			{
				if (rand() % 10000 <= 500)
				{
					static string music[6] = { "groan","groan2","groan3","groan4","groan5","groan6" };
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find(music[rand() % 6])->second), global->GlobalMusic.SoundEffect);
				}
			}), nullptr)));
}

void Zombie::Update(float Time, Node* node)
{
	for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); /*++Iter*/)
	{
		/* 僵尸移动 */
		global->Iter->second.ZombiesAnimation->getPositionX() < 1730 ?
			global->Iter->second.ZombiesAnimation->setPositionX(global->Iter->second.ZombiesAnimation->getPositionX() - Time * global->Iter->second.Speed) :
			global->Iter->second.ZombiesAnimation->setPositionX(global->Iter->second.ZombiesAnimation->getPositionX() - Time * global->Iter->second.Speed * 1.5f);

		if (global->Iter->second.ZombiesAnimation->getPositionX() < 1730 && !global->Iter->second.IsShow)
		{
			global->Iter->second.IsShow = true;
			global->Iter->second.ZombiesAnimation->runAction(FadeIn::create(1.0f));
			global->Iter->second.ZombiesAnimation->getChildByName("shadow")->runAction(FadeIn::create(1.0f));
		}
		
		switch (global->Iter->second.ZombiesType)
		{
		case Global::ZombiesType::CommonZombies:
			CommonZombies::getInstance()->CreateCommonZombiesAnimation(global->Iter, node);
			break;
		case Global::ZombiesType::ConeZombies:
			ConeZombies::getInstance()->CreateConeZombiesAniamton(global->Iter, node);
			break;
		case Global::ZombiesType::BucketZombies:
			BucketZombies::getInstance()->CreateBucketZombiesAniamton(global->Iter, node);
			break;
		case Global::ZombiesType::CommonDoorZombies:
			CommonDoorZombies::getInstance()->CreateCommonDoorZombiesAniamton(global->Iter, node);
			break;
		case Global::ZombiesType::ConeDoorZombies:
			ConeDoorZombies::getInstance()->CreateConeDoorZombiesAniamton(global->Iter, node);
			break;
		case Global::ZombiesType::BucketDoorZombies:
			BucketDoorZombies::getInstance()->CreateBucketDoorZombiesAniamton(global->Iter, node);
			break;
		case Global::ZombiesType::LmpZombies:
			LmpZombies::getInstance()->CreateLmpZombiesAniamton(global->Iter, node);
			break;
		default:
			break;
		}
		
		/* 判断僵尸胜利 */
		request->JudgeZombiesWin(global->Iter);

		/* 删除死亡僵尸 */
		if (!global->Iter->second.ZombiesAnimation->isVisible())
		{
			if (!global->Iter->second.IsCanDelete[0])
			{
				global->Iter->second.IsCanDelete[0] = true;
				UserDefault::getInstance()->setIntegerForKey("KILLALLZOMBIES", ++global->UserGameData.KillAllZombies);/* 杀死僵尸数加一 */
				UiImageText::getInstance()->UpdateZombiesDieNumbers();

				SunFlower::getInstance()->RandomCreateCoins(node, global->Iter->second.ZombiesAnimation->getPosition());
				auto zombies = global->Iter;
				node->runAction(Sequence::create(DelayTime::create(5.0f),
					CallFunc::create([=]()
						{
							zombies->second.IsCanDelete[1] = true;
						}), nullptr));
			}

			if (global->Iter->second.IsCanDelete[1])
			{
				global->Iter->second.ZombiesAnimation->pause();
				global->Iter->second.ZombiesAnimation->stopAllActions();

				node->removeChildByTag(global->Iter->first);
				global->Zombies.erase(global->Iter++);
			}
			else
			{
				global->Iter++;
			}
		}
		else
		{
			global->Iter++;
		}
	}
}

void Zombie::JudgeGameTypeForZombies(Sprite* sprite)
{
	/* 判断是否是小僵尸 */
	if (OpenLevelData::getInstance()->LevelData.ZombiesIsSmall)
	{
		sprite->setScale(0.7f);
	}
	/* 判断是否是巨人僵尸 */
	else if (OpenLevelData::getInstance()->LevelData.ZombiesIsBig)
	{
		sprite->setScale(1.8f);
	}
}

void Zombie::SetZombiesPrimaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID <= 5 && iter->second.AnimationID >= 1) /* 僵尸开始掉胳膊 */
	{
		iter->second.ZombiesAnimation->setAttachment("tt_outerarm_upper", "tt_outerarm_upper2");
		iter->second.ZombiesAnimation->setAttachment("tt_outerarm_lower", "tt_innerleg_foot3");
		iter->second.ZombiesAnimation->setAttachment("tt_outerarmhand", "tt_innerleg_foot3");
		iter->second.AnimationID = 6;

		ZombiesLoseArmAnimation(iter, node, "ZombieArm");
	}
}

void Zombie::SetZombiesSecondaryInjure(map<int, Global::_Zombies>::iterator iter, Node* node)
{
	if (iter->second.AnimationID == 6)
	{
		iter->second.ZombiesAnimation->setAttachment("tt_head", "tt_innerleg_foot3");
		iter->second.ZombiesAnimation->setAttachment("tt_jaw", "tt_innerleg_foot3");
		iter->second.ZombiesAnimation->setAnimation(1, "Zombies_Die", false);
		iter->second.AnimationID = 10;

		ZombiesLoseHeadAnimation(iter, node, "CommonZombieHead");

		auto animation = iter->second.ZombiesAnimation;
		auto const speed = iter;
		iter->second.ZombiesAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
			{
				if (!strcmp(event->data->name, "filldown"))
				{
					speed->second.Speed = 0; /* 停止运动 */
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

void Zombie::ZombiesLoseArmAnimation(map<int, Global::_Zombies>::iterator iter,Node* node, const string& name)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("limbs_pop")->second), global->GlobalMusic.SoundEffect);
	/* 掉胳膊 */
	auto arm = Sprite::create(global->ImagesPath.find(name)->second);
	arm->setPosition(iter->second.ZombiesAnimation->getPosition() + Vec2(-10, 100));
	arm->setGlobalZOrder(iter->second.ZombiesAnimation->getGlobalZOrder());
	node->addChild(arm, iter->second.ZombiesAnimation->getTag() + 1, "arm");
	arm->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(0.5f, Vec2(-80 + rand() % 160, -100), rand() % 80 + 66, 1), JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
		RotateBy::create(0.5f, rand() % 2 == 0 ? 90 : -90), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f),nullptr));

	JudgeGameTypeForZombies(arm);
}

void Zombie::ZombiesLoseHeadAnimation(map<int, Global::_Zombies>::iterator iter,Node* node, const string& name)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("limbs_pop")->second), global->GlobalMusic.SoundEffect);
	/* 僵尸掉头 */
	auto head = Sprite::create(global->ImagesPath.find(name)->second);
	head->setPosition(iter->second.ZombiesAnimation->getPosition() + Vec2(-40, 150));
	head->setGlobalZOrder(iter->second.ZombiesAnimation->getGlobalZOrder());
	head->setScale(1.5f);
	node->addChild(head, iter->second.ZombiesAnimation->getTag() + 1, "head");

	head->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(0.5f, Vec2(-150 + rand() % 300, -120), rand() % 100 + 66, 1),JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1),nullptr),
		RotateBy::create(0.5f, -180 + rand() % 360), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f),nullptr));

	JudgeGameTypeForZombies(head);
}

void Zombie::ZombiesLoseShieldAnimation(map<int, Global::_Zombies>::iterator iter,Node* node, const string& name)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("limbs_pop")->second), global->GlobalMusic.SoundEffect);
	/* 僵尸掉护盾 */
	auto cone = Sprite::create(global->ImagesPath.find(name)->second);
	cone->setPosition(iter->second.ZombiesAnimation->getPosition() + Vec2(-40, 200));
	cone->setGlobalZOrder(iter->second.ZombiesAnimation->getGlobalZOrder());
	cone->setScale(1.5f);
	node->addChild(cone, iter->second.ZombiesAnimation->getTag() + 1, "cone");

	cone->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(0.5f, Vec2(-150 + rand() % 300, -170), rand() % 100 + 66, 1), JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
		RotateBy::create(0.5f, -180 + rand() % 360), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f),nullptr));

	JudgeGameTypeForZombies(cone);
}

void Zombie::ZombiesBodyAnimation(map<int, Global::_Zombies>::iterator iter, Node* node, const string& name)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("limbs_pop")->second), global->GlobalMusic.SoundEffect);
	/* 僵尸掉身体 */
	auto cone = Sprite::create(global->ImagesPath.find(name)->second);
	cone->setPosition(iter->second.ZombiesAnimation->getPosition() + Vec2(-40, 200));
	cone->setGlobalZOrder(iter->second.ZombiesAnimation->getGlobalZOrder());
	cone->setScale(1.5f);
	node->addChild(cone, iter->second.ZombiesAnimation->getTag() + 1, "body");

	const int bodyrotate[4] = { -180,-360,360,180 };
	cone->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(0.5f, Vec2(-80 + rand() % 160, -180), rand() % 80 + 66, 1), JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
		RotateBy::create(0.5f, bodyrotate[rand() % 4]), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f),nullptr));

	JudgeGameTypeForZombies(cone);
}

void Zombie::ErgodicZombies()
{
	for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
	{
		SetZombiesSpeed(global->Iter);
	}
}

void Zombie::SetZombiesSpeed(map<int, Global::_Zombies>::iterator Iter)
{
	if (!Iter->second.Speed && !Iter->second.IsEat)
	{
		if (Iter->second.AnimationID != 10)
		{
			Iter->second.ZombiesAnimation->setMix("Zombies_Eat", ZombiesTypeAniamtion(Iter->second.ZombiesType), 1.3f);
			Iter->second.ZombiesAnimation->setAnimation(1, ZombiesTypeAniamtion(Iter->second.ZombiesType), true);
			Iter->second.Speed = ZombiesSpeed[(int)Iter->second.ZombiesType];
		}
	}
}

string Zombie::ZombiesTypeAniamtion(Global::ZombiesType& zombiestype)
{
	string name;
	switch (zombiestype)
	{
	case Global::ZombiesType::CommonDoorZombies:
	case Global::ZombiesType::ConeDoorZombies:
	case Global::ZombiesType::BucketDoorZombies:
		name = "Zombies_Door_Walk";
		break;
	default:
		name = "Zombies_Walk";
		break;
	}
	return name;
}

void Zombie::ZombiesKilledByCar(Node* node, map<int, Global::_Zombies>::iterator Iter)
{
	switch (Iter->second.ZombiesType)
	{
	case Global::ZombiesType::CommonZombies:
		CommonZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::ConeZombies:
		ConeZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::BucketZombies:
		BucketZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::CommonDoorZombies:
		CommonDoorZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::ConeDoorZombies:
		ConeDoorZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::BucketDoorZombies:
		BucketDoorZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	case Global::ZombiesType::LmpZombies:
		LmpZombies::getInstance()->ZombiesKilledByCar(Iter, node);
		break;
	default:
		break;
	}
}

void Zombie::SetZombiesForGameType()
{
	for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); global->Iter++)
	{
		/* 判断是否是小僵尸 */
		if (OpenLevelData::getInstance()->LevelData.ZombiesIsSmall)
		{
			if (!global->Iter->second.IsUseForGameType)
			{
				/* 血量减半 */
				global->Iter->second.BloodVolume = global->Iter->second.BloodVolume / 2.0f;
				global->Iter->second._BloodVolume = global->Iter->second._BloodVolume / 2.0f;
				global->Iter->second.ShieldVolume = global->Iter->second.ShieldVolume * 2 / 3.0f;
				global->Iter->second._ShieldVolume = global->Iter->second._ShieldVolume * 2 / 3.0f;
				global->Iter->second.ShieldVolume1 = global->Iter->second.ShieldVolume1 * 2 / 3.0f;
				global->Iter->second._ShieldVolume1 = global->Iter->second._ShieldVolume1 * 2 / 3.0f;
				/* 速度增加 */
				global->Iter->second.Speed = 40;
				global->Iter->second.CaveSpeed = 40;
				/* 身体变小，动作变快 */
				global->Iter->second.ZombiesAnimation->setScale(0.5f);
				global->Iter->second.ZombiesAnimation->setTimeScale(1.2f);

				global->Iter->second.IsUseForGameType = true;
			}
		}
		/* 判断是否是巨人僵尸 */
		else if (OpenLevelData::getInstance()->LevelData.ZombiesIsBig)
		{
			if (!global->Iter->second.IsUseForGameType)
			{
				/* 血量减半 */
				global->Iter->second.BloodVolume += global->Iter->second.BloodVolume;
				global->Iter->second._BloodVolume += global->Iter->second._BloodVolume;
				global->Iter->second.ShieldVolume += global->Iter->second.ShieldVolume * 1 / 3.0f;
				global->Iter->second._ShieldVolume += global->Iter->second._ShieldVolume * 1 / 3.0f;
				global->Iter->second.ShieldVolume1 += global->Iter->second.ShieldVolume1 * 1 / 3.0f;
				global->Iter->second._ShieldVolume1 += global->Iter->second._ShieldVolume1 * 1 / 3.0f;
				/* 速度减少 */
				global->Iter->second.Speed = 20;
				global->Iter->second.CaveSpeed = 20;
				/* 身体变大，动作变慢 */
				global->Iter->second.ZombiesAnimation->setScale(1.3f);
				global->Iter->second.ZombiesAnimation->setTimeScale(0.4f);

				global->Iter->second.IsUseForGameType = true;
			}
		}
		/* 判断僵尸是否隐身 */
		else if (OpenLevelData::getInstance()->LevelData.ZombiesIsVisible)
		{
			global->Iter->second.ZombiesAnimation->setOpacity(0);
			global->Iter->second.IsShow = true;
			global->Iter->second.ZombiesAnimation->getChildByName("shadow")->setOpacity(0);
		}
	}
}

void Zombie::PlayZombiesDieAnimation(Node* node, map<int, Global::_Zombies>::iterator Iter, const string& name)
{
	auto Explode = SkeletonAnimation::createWithData(global->GlobalAnimationData.find(name)->second);
	Explode->setPosition(Iter->second.ZombiesAnimation->getPosition() + Vec2(0, -10));
	Explode->setGlobalZOrder(Iter->second.ZombiesAnimation->getGlobalZOrder());
	Explode->setName("Explode");
	Explode->setScale(1.3f);
	Explode->setAnimation(0, "animation", false);
	node->addChild(Explode);

	Explode->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				Explode->runAction(Sequence::create(FadeOut::create(1.0f),
					CallFunc::create([=]()
						{
							Explode->stopAllActions();
							Explode->setVisible(false);
						}), nullptr));
			}
		});

	Explode->runAction(Sequence::create(DelayTime::create(3),
		CallFunc::create([=]()
			{
				node->removeChildByName("Explode");
			}), nullptr));

	/* 判断是否是小僵尸 */
	if (OpenLevelData::getInstance()->LevelData.ZombiesIsSmall)
	{
		Explode->setScale(0.7f);
	}
	/* 判断是否是巨人僵尸 */
	else if (OpenLevelData::getInstance()->LevelData.ZombiesIsBig)
	{
		Explode->setScale(1.8f);
	}
}

int Zombie::CreateZombiesForNumbers(const int& ZombiesAppearFrequency)
{
	const int number = LevelData->LevelData.ZombiesNumbers.at(ZombiesAppearFrequency);
	return number + rand() % number;
}

int Zombie::CreateZombiesForProbility(const int& ZombiesAppearFrequency)
{
	int number = rand() % 100;
	int Sum = 0;

	for (unsigned int j = 0; j < LevelData->LevelData.ZombiesTypeProbabilityFrequency.at(ZombiesAppearFrequency).size(); j++)
	{
		Sum += LevelData->LevelData.ZombiesTypeProbabilityFrequency.at(ZombiesAppearFrequency).at(j);
		if (number < Sum)
		{
			return LevelData->LevelData.ZombiesType.at(j);
		}
	}
	return -1;
}