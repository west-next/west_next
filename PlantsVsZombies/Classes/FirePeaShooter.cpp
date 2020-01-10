#include "FirePeaShooter.h"

FirePeaShooter* FirePeaShooter::_instance = nullptr;

FirePeaShooter* FirePeaShooter::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)FirePeaShooter;
	}
	return _instance;
}


void FirePeaShooter::Initialization()
{
	FirePeaShooter::getInstance()->FirePeaShooterAnimations.clear();
	FirePeaShooter::getInstance()->FirePeas.clear();
	FirePeaShooter::getInstance()->FirePeaTag = 0;
}

void FirePeaShooter::CreateFirePeaShooterImage(const Vec2& position, Node* node)
{
	FirePeaShooterImage = CreateImage("FirePeaShooter", position, node);
	FirePeaShooterImage->setScale(1.5f);
}

void FirePeaShooter::CreateFirePeaShooterAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto FirePeaShooterAnimation = CreateAnimation("FirePeaShooter", "FirePeaShooter_Normal", position, 0.8f, 1.8f);
	FirePeaShooterAnimation->setAnchorPoint(Vec2(0, 0));
	FirePeaShooterAnimation->setScale(1.2f);
	node->addChild(FirePeaShooterAnimation, order, tag);

	_FirePeaShooterAnimation animation;
	animation.Animation = FirePeaShooterAnimation;
	animation.BloodVolume = 300;
	animation.tag = tag;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	FirePeaShooterAnimations.insert(pair<int, _FirePeaShooterAnimation>(tag, animation));
}

void FirePeaShooter::CreateFirePeaShooterListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(FirePeaShooter::Update, this, node), "UpdateFirePeaShooter");
}

void FirePeaShooter::Update(float Time, Node* node)
{
	JudgeFirePeaShooterAndZombiesPosition(node); /* 僵尸与豌豆射手位置判断 */

	JudgeFirePeaAndZombiesPosition(node);        /* 僵尸与豌豆位置判断 */
}

void FirePeaShooter::JudgeFirePeaShooterAndZombiesPosition(Node* node)
{
	for (FirePeaShooterIter = FirePeaShooterAnimations.begin(); FirePeaShooterIter != FirePeaShooterAnimations.end(); /*++FirePeaShooterIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                       /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() <= 1730 &&                                                                     /* 僵尸进入地图 */
				global->Iter->second.ZombiesAnimation->getPositionX() >= FirePeaShooterIter->second.Animation->getPositionX() &&                     /* 僵尸在植物的前方 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - FirePeaShooterIter->second.Animation->getPositionY()) <= 10)     /* 僵尸与植物在同一行 */
			{
				if (FirePeaShooterIter->second.BloodVolume > 0 &&                                                                            /* 植物没有死 */
					fabs(global->Iter->second.ZombiesAnimation->getPositionX() - FirePeaShooterIter->second.Animation->getPositionX()) <= 80)        /* 僵尸遇到植物 */
				{
					ZombiesEatPlants(global->Iter, &FirePeaShooterIter->second.BloodVolume, FirePeaShooterIter->second.Animation);
				}
				if (FirePeaShooterIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
				{
					global->Iter->second.IsEat = false;
					if (global->Iter->second.AnimationID != 10)
					{
						global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.3f);
						global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
						global->Iter->second.Speed = global->Iter->second.CaveSpeed;
					}
				}

				FirePeaShooterIter->second.IsCreatePea = true; /* 表示有僵尸与植物在同一行 */
				if (!FirePeaShooterIter->second.IsChanged)     /* 判断动画是否已经改变 */
				{
					FirePeaShooterIter->second.Animation->addAnimation(0, "FirePeaShooter_Shoot", true);
					FirePeaShooterIter->second.IsChanged = true;
				}
				Vec2 pos = FirePeaShooterIter->second.Animation->getPosition();
				FirePeaShooterIter->second.Animation->setEventListener([=](spTrackEntry* entry, spEvent* event)
					{
						if (strcmp(event->data->name, "shoot") == 0)
						{
							rand() % 2 == 0 ? PlaySoundEffect("throw") : PlaySoundEffect("throw2");
							CreatePea(pos, node);
						}
					});
			}
		}
		if (!FirePeaShooterIter->second.IsCreatePea) /* 判断是否有僵尸与豌豆射手在同一行 */
		{
			if (FirePeaShooterIter->second.IsChanged)
			{
				FirePeaShooterIter->second.Animation->addAnimation(0, "FirePeaShooter_Normal", true);
				FirePeaShooterIter->second.IsChanged = false;
			}
		}

		FirePeaShooterIter->second.IsCreatePea = false; /* 每循环一次就初始化 */

		/* 删除豌豆射手 */
		if (FirePeaShooterIter->second.BloodVolume <= 0)
		{
			global->MapSize.PlantsMap[(int)FirePeaShooterIter->second.RowAndColumn.y][(int)FirePeaShooterIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(FirePeaShooterIter->second.tag);
			FirePeaShooterAnimations.erase(FirePeaShooterIter++);
		}
		else
		{
			FirePeaShooterIter++;
		}
	}
}

void FirePeaShooter::JudgeFirePeaAndZombiesPosition(Node* node)
{
	for (FirePeaIter = FirePeas.begin(); FirePeaIter != FirePeas.end(); /*++FirePeaIter*/)
	{
		/* 删除出界豌豆 */
		if (FirePeaIter->second.Pea->getPositionX() > 2000)
		{
			FirePeaIter->second.Pea->setOpacity(0);
		}

		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (!FirePeaIter->second.IsUsed &&                                                                                          /* 豌豆没有被使用 */
				global->Iter->second.ZombiesAnimation->isVisible() && global->Iter->second.BloodVolume > 0 &&                           /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() < 1730 &&                                                         /* 僵尸进入地图内 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() + 83) - FirePeaIter->second.Pea->getPositionY()) <= 5 &&    /* 豌豆与僵尸在同一行 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - FirePeaIter->second.Pea->getPositionX()) <= 25)            /* 豌豆与僵尸碰撞 */
			{
				PlaySoundEffect("firepea");

				FirePeaIter->second.Pea->setOpacity(0);
				FirePeaIter->second.Pea->getChildByName("shadow")->setOpacity(0);

				global->Iter->second.ZombiesAnimation->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

				if (global->Iter->second.ShieldVolume1 > 0)
				{
					global->Iter->second.ShieldVolume1 -= FirePeaIter->second.Attack;
				}
				else if (global->Iter->second.ShieldVolume > 0)
				{
					global->Iter->second.ShieldVolume -= FirePeaIter->second.Attack;
				}
				else
				{
					global->Iter->second.BloodVolume -= FirePeaIter->second.Attack;
				}

				CreatePeaExplode(FirePeaIter->second.Pea->getPosition(), node);

				FirePeaIter->second.Attack = 0;
				FirePeaIter->second.IsUsed = true;

				break; /* 一个豌豆只能击中一个僵尸 */
			}
		}

		/* 删除 */
		if (FirePeaIter->second.Pea->isVisible() == false)
		{
			//node->removeChildByTag(FirePeaIter->second.tag);
			FirePeas.erase(FirePeaIter++);
		}
		else
		{
			FirePeaIter++;
		}
	}
}

void FirePeaShooter::CreatePea(const Vec2& position, Node* node)
{
	FirePeaTag++;

	auto Pea = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("FirePea")->second);
	Pea->setAnimation(0, "fire", true);
	Pea->setPosition(position + Vec2(70, 90));
	Pea->setScale(1.5f);
	Pea->setTag(FirePeaTag);
	Pea->setGlobalZOrder(SetZOrder(position.y + 90));
	node->addChild(Pea);
	Pea->runAction(Sequence::create(MoveBy::create(2.5f, Vec2(1500, 0)),
		CallFunc::create([=]()
			{
				Pea->setVisible(false);
			}), nullptr));

	/* 创建影子 */
	auto shadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
	shadow->setScaleY(0.7f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -52));
	shadow->setGlobalZOrder(Pea->getGlobalZOrder());
	Pea->addChild(shadow, -1);

	_FirePea pea;
	pea.Pea = Pea;
	pea.tag = FirePeaTag;
	pea.IsUsed = false;
	FirePeas.insert(pair<int, _FirePea>(FirePeaTag, pea));
}

float FirePeaShooter::SetZOrder(const float& pos_y)
{
	int const pos[5] = { 765,627,489,351,213 };
	for (int i = 0; i < 5; i++)
		if (pos[i] == pos_y)
			return (i + 1) * 2;
	return 0;
}

void FirePeaShooter::CreatePeaExplode(const Vec2& position, Node* node, const int& ID)
{
	auto PeaExplode = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("PeaExplode")->second);
	PeaExplode->setPosition(position);
	PeaExplode->setAnimation(0, "Explode3", false);
	PeaExplode->setScale(1.4f);
	PeaExplode->setGlobalZOrder(SetZOrder(position.y));
	PeaExplode->setName("PeaExplode");
	node->addChild(PeaExplode);

	PeaExplode->runAction(Sequence::create(DelayTime::create(0.9f), CallFunc::create([=]()
		{
			node->removeChildByName("PeaExplode");
		}), nullptr));
}