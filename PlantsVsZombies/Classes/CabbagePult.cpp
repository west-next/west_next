#include "CabbagePult.h"

CabbagePult* CabbagePult::_instance = nullptr;

CabbagePult* CabbagePult::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)CabbagePult;
	}
	return _instance;
}


void CabbagePult::Initialization()
{
	CabbagePult::getInstance()->CabbagePultAnimations.clear();
	CabbagePult::getInstance()->CabbageBullet.clear();
	CabbagePult::getInstance()->CabbageBulletTag = 0;
}

void CabbagePult::CreateCabbagePultImage(const Vec2& position, Node* node)
{
	CabbagePultImage = CreateImage("Cabbage", position, node);
	CabbagePultImage->setScale(1.5f);
}

void CabbagePult::CreateCabbagePultAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto CabbagePultAnimation = CreateAnimation("Cabbage", "Cabbage_Normal", position, 1.0f, 2.6f);
	CabbagePultAnimation->setAnchorPoint(Vec2(0, 0));
	CabbagePultAnimation->setScale(0.8f);
	node->addChild(CabbagePultAnimation, order, tag);

	_CabbagePultAnimation animation;
	animation.Animation = CabbagePultAnimation;
	animation.BloodVolume = 300;
	animation.tag = tag;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	CabbagePultAnimations.insert(pair<int, _CabbagePultAnimation>(tag, animation));
}

void CabbagePult::CreateCabbagePultListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(CabbagePult::Update, this, node), "UpdateCabbage");
}

void CabbagePult::Update(float Time, Node* node)
{
	JudgeCabbagePultAndZombiesPosition(node); /* 僵尸与卷心菜投手位置判断 */

	JudgeCabbageBulletAndZombiesPosition(node);
}

void CabbagePult::JudgeCabbagePultAndZombiesPosition(Node* node)
{
	for (CabbagePultIter = CabbagePultAnimations.begin(); CabbagePultIter != CabbagePultAnimations.end(); /*++CabbagePultIter*/)
	{
		map<int, Global::_Zombies>::iterator ZombiesIter;
		float distance = 10000000;
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                     /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() <= 1730 &&                                                                  /* 僵尸进入地图 */
				global->Iter->second.ZombiesAnimation->getPositionX() >= CabbagePultIter->second.Animation->getPositionX() &&                     /* 僵尸在植物的前方 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - CabbagePultIter->second.Animation->getPositionY()) <= 10)     /* 僵尸与植物在同一行 */
			{
				if (CabbagePultIter->second.BloodVolume > 0 &&                                                                                    /* 植物没有死 */
					fabs(global->Iter->second.ZombiesAnimation->getPositionX() - CabbagePultIter->second.Animation->getPositionX()) <= 80)        /* 僵尸遇到植物 */
				{
					ZombiesEatPlants(global->Iter, &CabbagePultIter->second.BloodVolume, CabbagePultIter->second.Animation);
				}
				if (CabbagePultIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
				{
					global->Iter->second.IsEat = false;
					if (global->Iter->second.AnimationID != 10)
					{
						global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.5f);
						global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
						global->Iter->second.Speed = global->Iter->second.CaveSpeed;
					}
				}

				CabbagePultIter->second.IsCreateCabbage = true; /* 表示有僵尸与植物在同一行 */
				if (!CabbagePultIter->second.IsChanged)         /* 判断动画是否已经改变 */
				{
					CabbagePultIter->second.Animation->addAnimation(0, "Cabbage_Shoot", true);
					CabbagePultIter->second.IsChanged = true;
				}

				if (global->Iter->second.ZombiesAnimation->getPositionX() > CabbagePultIter->second.Animation->getPositionX() &&          /* 僵尸在植物前方 */
					distance > global->Iter->second.ZombiesAnimation->getPositionX() - CabbagePultIter->second.Animation->getPositionX()) /* 僵尸于植物的距离小于上一个僵尸与植物的距离 */
				{
					distance = global->Iter->second.ZombiesAnimation->getPositionX() - CabbagePultIter->second.Animation->getPositionX();
					ZombiesIter = global->Iter;
				}
			}
		}

		if (CabbagePultIter->second.IsCreateCabbage)
		{
			Vec2 CabbagePosition = CabbagePultIter->second.Animation->getPosition();
			Vec2 ZombiesPosition = ZombiesIter->second.ZombiesAnimation->getPosition();
			CabbagePultIter->second.Animation->setEventListener([=](spTrackEntry* entry, spEvent* event)
				{
					if (strcmp(event->data->name, "Shoot") == 0)
					{
						rand() % 2 == 0 ? PlaySoundEffect("throw") : PlaySoundEffect("throw2");
						CreateCabbage(CabbagePosition, ZombiesPosition, node, ZombiesIter);
					}
				});
		}

		if (!CabbagePultIter->second.IsCreateCabbage) /* 判断是否有僵尸与卷心菜投手在同一行 */
		{
			if (CabbagePultIter->second.IsChanged)
			{
				CabbagePultIter->second.Animation->addAnimation(0, "Cabbage_Normal", true);
				CabbagePultIter->second.IsChanged = false;
			}
		}

		CabbagePultIter->second.IsCreateCabbage = false; /* 每循环一次就初始化 */

		/* 删除卷心菜投手 */
		if (CabbagePultIter->second.BloodVolume <= 0)
		{
			global->MapSize.PlantsMap[(int)CabbagePultIter->second.RowAndColumn.y][(int)CabbagePultIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(CabbagePultIter->second.tag);
			CabbagePultAnimations.erase(CabbagePultIter++);
		}
		else
		{
			CabbagePultIter++;
		}
	}
}

void CabbagePult::JudgeCabbageBulletAndZombiesPosition(Node* node)
{
	for (CabbageBulletIter = CabbageBullet.begin(); CabbageBulletIter != CabbageBullet.end(); /*++CabbageBulletIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (!CabbageBulletIter->second.IsUse &&
				global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                           /* 僵尸没有死亡 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionY() - CabbageBulletIter->second.PositionY) <= 10 &&
				fabs(global->Iter->second.ZombiesAnimation->getPositionY() - CabbageBulletIter->second.CabbageBullet->getPositionY()) <= 170 &&
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - CabbageBulletIter->second.CabbageBullet->getPositionX()) <= 60)            /* 豌豆与僵尸碰撞 */
			{
				CabbageBulletIter->second.CabbageBullet->setOpacity(0);
				CreateCabbageExplode(global->Iter->second.ZombiesAnimation->getPosition() + Vec2(-40, 170), node);
				
				CabbageBulletIter->second.IsUse = true;
				PlayPeaExplodeMusic(global->Iter->second.MusicType); /* 播放指定音乐 */

				global->Iter->second.ZombiesAnimation->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

				if (global->Iter->second.ShieldVolume > 0)
				{
					global->Iter->second.ShieldVolume -= CabbageBulletIter->second.Attack;
				}
				else
				{
					global->Iter->second.BloodVolume -= CabbageBulletIter->second.Attack;
				}
				break; /* 一个只能击中一个僵尸 */
			}
		}

		/* 删除 */
		if (CabbageBulletIter->second.CabbageBullet != nullptr && CabbageBulletIter->second.CabbageBullet->isVisible() == false)
		{
			CabbageBullet.erase(CabbageBulletIter++);
		}
		else
		{
			CabbageBulletIter++;
		}
	}
}

void CabbagePult::CreateCabbage(const Vec2& CabbagePosition, const Vec2& ZombiesPosition, Node* node, map<int, Global::_Zombies>::iterator Iter)
{
	CabbageBulletTag++;

	auto Cabbage = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("CabbageBullet")->second);
	Cabbage->setAnimation(0, "Cabbage_Rotate", true);
	Cabbage->setScale(0.8f);
	Cabbage->setGlobalZOrder(SetZOrder(CabbagePosition.y));
	Cabbage->setPosition(CabbagePosition + Vec2(70, 150));
	Cabbage->setTag(CabbageBulletTag);
	Cabbage->setAnchorPoint(Vec2(0, 0));
	node->addChild(Cabbage);

	Cabbage->runAction(Sequence::create(JumpTo::create(1.0f, ZombiesPosition + Vec2(5, 20), 300, 1),
		CallFunc::create([=]()
			{
				if (Cabbage->getOpacity())
				{
					PlayPeaExplodeMusic(0);
				}
				Cabbage->setAnimation(0, "Cabbage_Crush", false);
			}),
		DelayTime::create(2.0f),
				CallFunc::create([=]()
					{
						Cabbage->setVisible(false);
					}), nullptr));

	/* 创建影子 */
	auto shadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
	shadow->setPosition(CabbagePosition + Vec2(70, 15));
	shadow->setGlobalZOrder(SetZOrder(CabbagePosition.y));
	shadow->setName("CabbageShadow");
	shadow->setGlobalZOrder(Cabbage->getGlobalZOrder());
	node->addChild(shadow);
	shadow->runAction(Sequence::create(MoveTo::create(0.9f, Vec2(ZombiesPosition.x - 100, CabbagePosition.y + 15)),
		CallFunc::create([=]()
			{
				node->removeChildByName("CabbageShadow");
			}), nullptr));

	_CabbageBullet CabbageBullets;
	CabbageBullets.CabbageBullet = Cabbage;
	CabbageBullets.IsUse = false;
	CabbageBullets.tag = CabbageBulletTag;
	CabbageBullets.PositionY = CabbagePosition.y;
	CabbageBullet.insert(pair<int, _CabbageBullet>(CabbageBulletTag, CabbageBullets));

}

float CabbagePult::SetZOrder(const float& pos_y)
{
	int const pos[5] = { 675,537,399,261,123 };
	for (int i = 0; i < 5; i++)
		if (fabs(pos[i] - pos_y) <= 10)
			return i * 2 + 2;
	return 0;
}

void CabbagePult::PlayPeaExplodeMusic(const int& MusicType)
{
	switch (MusicType)
	{
	case 0:
		rand() % 2 == 0 ? PlaySoundEffect("kernelpult") : PlaySoundEffect("kernelpult2");
		break;
	case 1:
		rand() % 2 == 0 ? PlaySoundEffect("shieldhit") : PlaySoundEffect("shieldhit2");
		break;
	}
}

void CabbagePult::CreateCabbageExplode(const Vec2& position, Node* node)
{
	auto Cabbage = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("CabbageBullet")->second);
	Cabbage->setAnimation(0, "Cabbage_Crush", false);
	Cabbage->setScale(0.6f);
	Cabbage->setGlobalZOrder(12);
	Cabbage->setPosition(position);
	Cabbage->setName("CabbageBulletTag");
	Cabbage->setAnchorPoint(Vec2(0, 0));
	node->addChild(Cabbage);

	Cabbage->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() 
		{
			node->removeChildByName("CabbageBulletTag");
		}), nullptr));
}
