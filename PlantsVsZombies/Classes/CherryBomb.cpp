#include "CherryBomb.h"

CherryBomb* CherryBomb::_instance = nullptr;

CherryBomb* CherryBomb::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)CherryBomb;
	}
	return _instance;
}

void CherryBomb::Initialization()
{
	CherryBomb::getInstance()->CherryBombAnimation.clear();
}

void CherryBomb::CreateCherryBombImage(const Vec2& position, Node* node)
{
	CherryBombImage = CreateImage("CherryBomb", position, node);
	CherryBombImage->setScale(1.0f);
}

void CherryBomb::CreateCherryBombAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto CherryBombAnimations = CreateAnimation("CherryBomb", "CherryBomb_Explode", position, 0.8f, 1.8f, false);
	CherryBombAnimations->setAnchorPoint(Vec2(0, 0));
	//CherryBombAnimations->setScale(0.9f);
	CherryBombAnimations->setTimeScale(1.5f);
	node->addChild(CherryBombAnimations, order, tag);
	
	_CherryBombAnimation cherrybombanimation;
	cherrybombanimation.Animation = CherryBombAnimations;
	cherrybombanimation.BloodVolume = 300;
	cherrybombanimation.position = position;
	cherrybombanimation.RowAndColumn = RowAndColumn;
	cherrybombanimation.tag = tag;
	cherrybombanimation.ExplosiveDamage = 1800;
	cherrybombanimation.IsReadyExplode = false;
	cherrybombanimation.IsExplode = false;

	CherryBombAnimation.insert(pair<int, _CherryBombAnimation>(tag, cherrybombanimation));

	CherryBombAnimations->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				CherryBombAnimations->stopAllActions();
				CherryBombReady.push_back(tag);
			}
		});
	PlaySoundEffect("wakeup");
}

void CherryBomb::CreateCherryBombListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(CherryBomb::Update, this, node), "UpdateCherryBomb");
}

void CherryBomb::Update(float Time, Node* node)
{
	UpdateCherryBombReady();

	JudgeCherryBombAndZombiesPosition(node);
}

void CherryBomb::UpdateCherryBombReady()
{
	list<int>::iterator ReadyIter;
	for (ReadyIter = CherryBombReady.begin(); ReadyIter != CherryBombReady.end();)
	{
		CherryBombAnimation.find(*ReadyIter)->second.IsReadyExplode = true;
		CherryBombAnimation.find(*ReadyIter)->second.BloodVolume = 0;
		CherryBombReady.erase(ReadyIter++);
	}
}

void CherryBomb::JudgeCherryBombAndZombiesPosition(Node* node)
{
	for (CherryBombIter = CherryBombAnimation.begin(); CherryBombIter != CherryBombAnimation.end(); /*++CherryBombIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (CherryBombIter->second.BloodVolume > 0 &&                                                                                      /* 植物没有死 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - CherryBombIter->second.Animation->getPositionY()) <= 10 &&
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - CherryBombIter->second.Animation->getPositionX()) <= 80)           /* 僵尸遇到植物 */
			{
				ZombiesEatPlants(global->Iter, &CherryBombIter->second.BloodVolume, CherryBombIter->second.Animation);
			}
			if (CherryBombIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
			{
				global->Iter->second.IsEat = false;
				if (global->Iter->second.AnimationID != 10)
				{
					global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.5f);
					global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
					global->Iter->second.Speed = global->Iter->second.CaveSpeed;
				}
			}

			if (CherryBombIter->second.BloodVolume <= 0 || CherryBombIter->second.IsReadyExplode)
			{
				CherryBombExplodeKillZombies(node, CherryBombIter, global->Iter);
			}
		}

		/* 删除樱桃炸弹 */
		if (CherryBombIter->second.BloodVolume <= 0)
		{
			PlayCherryBombExplode(node, CherryBombIter);
			global->MapSize.PlantsMap[(int)CherryBombIter->second.RowAndColumn.y][(int)CherryBombIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(CherryBombIter->second.tag);
			CherryBombAnimation.erase(CherryBombIter++);
		}                                                           
		else
		{
			CherryBombIter++;
		}
	}
}

void CherryBomb::PlayCherryBombExplode(Node* node,map<int, _CherryBombAnimation>::iterator CherryBombIter)
{
	PlaySoundEffect("cherrybomb"); 
	
	auto CherryBomb_Explode = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("CherryBomb_Explode")->second);
	CherryBomb_Explode->setPosition(CherryBombIter->second.Animation->getPosition());
	CherryBomb_Explode->setAnimation(0, "CherryBomb_Explode", false);
	CherryBomb_Explode->setScale(1.5f);
	CherryBomb_Explode->setGlobalZOrder(SetZorder(CherryBombIter->second.Animation->getPositionY()));
	CherryBomb_Explode->setName("CherryBomb_Explode");
	node->addChild(CherryBomb_Explode);

	CherryBomb_Explode->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
			    CherryBomb_Explode->stopAllActions();
				CherryBomb_Explode->setVisible(false);
			}
		});

	CherryBomb_Explode->runAction(Sequence::create(DelayTime::create(2),
		CallFunc::create([=]()
			{
				node->removeChildByName("CherryBomb_Explode");
			}), nullptr));
}

void CherryBomb::CherryBombExplodeKillZombies(Node* node, map<int, _CherryBombAnimation>::iterator CherryBombIter, map<int, Global::_Zombies>::iterator Iter)
{
	if (Iter->second.BloodVolume > 0 && Iter->second.ZombiesAnimation->isVisible()&&                                            /* 僵尸存活 */
		Iter->second.ZombiesAnimation->getPositionX() <= 1730 &&                                                                /* 僵尸进入地图 */
		sqrt(pow(Iter->second.ZombiesAnimation->getPositionX() - CherryBombIter->second.Animation->getPositionX(), 2) +
			pow(Iter->second.ZombiesAnimation->getPositionY() - CherryBombIter->second.Animation->getPositionY(), 2)) <= 195)   /* 僵尸在爆炸范围内 */
	{
		if (Iter->second.ShieldVolume1 < CherryBombIter->second.ExplosiveDamage) /* 如果护盾1血量小于爆炸伤害 */
		{
			if (Iter->second.ShieldVolume1 + Iter->second.ShieldVolume < CherryBombIter->second.ExplosiveDamage) /* 如果护盾1加护盾血量小于爆炸伤害 */
			{
				if (Iter->second.ShieldVolume1 + Iter->second.ShieldVolume + Iter->second.BloodVolume <= CherryBombIter->second.ExplosiveDamage) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
				{
					Iter->second.ZombiesAnimation->setVisible(false);
					Iter->second.ZombiesAnimation->pause();
					if (Iter->second.ZombiesType == Global::ZombiesType::LmpZombies)
					{
						zombie->PlayZombiesDieAnimation(node, Iter, "LmpZombies_Charre");
					}
					else
					{
						zombie->PlayZombiesDieAnimation(node, Iter, "Zombies_Ash");
					}
				}
				else
				{
					/* 计算僵尸本体血量 */
					Iter->second.BloodVolume = Iter->second.ShieldVolume1 + Iter->second.ShieldVolume + Iter->second.BloodVolume - CherryBombIter->second.ExplosiveDamage;
					Iter->second.ShieldVolume1 = 0;
					Iter->second.ShieldVolume = 0;
				}
			}
			else
			{
				/* 计算僵尸护盾剩于血量 */
				Iter->second.ShieldVolume = Iter->second.ShieldVolume1 + Iter->second.ShieldVolume - CherryBombIter->second.ExplosiveDamage;
				Iter->second.ShieldVolume1 = 0;
			}
		}
		else
		{
			Iter->second.ShieldVolume1 -= CherryBombIter->second.ExplosiveDamage;
		}
	}
}

int CherryBomb::SetZorder(const float& position)
{
	int const pos[5] = { 675,537,399,261,123 };
	for (int i = 0; i < 5; i++)
		if (pos[i] == position)
			return (i + 1) * 2;
	return 0;
}
