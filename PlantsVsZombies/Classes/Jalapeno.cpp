#include "Jalapeno.h"

Jalapeno* Jalapeno::_instance = nullptr;

Jalapeno* Jalapeno::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Jalapeno;
	}
	return _instance;
}

void Jalapeno::Initialization()
{
	Jalapeno::getInstance()->JalapenoAnimation.clear();
}

void Jalapeno::CreateJalapenoImage(const Vec2& position, Node* node)
{
	JalapenoImage = CreateImage("Jalapeno", position, node);
	JalapenoImage->setScale(1.2f);
}

void Jalapeno::CreateJalapenoAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto JalapenoAnimations = CreateAnimation("Jalapeno", "Jalapeno_Explode", position, 0.8f, 1.8f, false);
	JalapenoAnimations->setAnchorPoint(Vec2(0, 0));
	//JalapenoAnimations->setScale(0.9f);
	JalapenoAnimations->setTimeScale(1.5f);
	node->addChild(JalapenoAnimations, order, tag);

	_JalapenoAnimation jalapenoanimation;
	jalapenoanimation.Animation = JalapenoAnimations;
	jalapenoanimation.BloodVolume = 300;
	jalapenoanimation.position = position;
	jalapenoanimation.RowAndColumn = RowAndColumn;
	jalapenoanimation.tag = tag;
	jalapenoanimation.ExplosiveDamage = 1800;
	jalapenoanimation.IsReadyExplode = false;
	jalapenoanimation.IsExplode = false;

	JalapenoAnimation.insert(pair<int, _JalapenoAnimation>(tag, jalapenoanimation));

	JalapenoAnimations->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				JalapenoAnimations->stopAllActions();
				JalapenoReady.push_back(tag);
			}
		});
	PlaySoundEffect("wakeup");
}

void Jalapeno::CreateJalapenoListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(Jalapeno::Update, this, node), "UpdateJalapeno");
}

int Jalapeno::SetZorder(const float& position)
{
	int const pos[5] = { 675,537,399,261,123 };
	for (int i = 0; i < 5; i++)
		if (pos[i] == position)
			return (i + 1) * 2;
	return 0;
}

void Jalapeno::Update(float Time, Node* node)
{
	UpdateJalapenoReady();

	JudgeJalapenoAndZombiesPosition(node);
}

void Jalapeno::UpdateJalapenoReady()
{
	list<int>::iterator ReadyIter;
	for (ReadyIter = JalapenoReady.begin(); ReadyIter != JalapenoReady.end();)
	{
		JalapenoAnimation.find(*ReadyIter)->second.IsReadyExplode = true;
		JalapenoAnimation.find(*ReadyIter)->second.BloodVolume = 0;
		JalapenoReady.erase(ReadyIter++);
	}
}

void Jalapeno::JudgeJalapenoAndZombiesPosition(Node* node)
{
	for (JalapenoIter = JalapenoAnimation.begin(); JalapenoIter != JalapenoAnimation.end(); /*++JalapenoIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (JalapenoIter->second.BloodVolume > 0 &&                                                                                         /* 植物没有死 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - JalapenoIter->second.Animation->getPositionY()) <= 10 &&
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - JalapenoIter->second.Animation->getPositionX()) <= 80)             /* 僵尸遇到植物 */
			{
				ZombiesEatPlants(global->Iter, &JalapenoIter->second.BloodVolume, JalapenoIter->second.Animation);
			}
			if (JalapenoIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
			{
				global->Iter->second.IsEat = false;
				if (global->Iter->second.AnimationID != 10)
				{
					global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.5f);
					global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
					global->Iter->second.Speed = global->Iter->second.CaveSpeed;
				}
			}

			if (JalapenoIter->second.BloodVolume <= 0 || JalapenoIter->second.IsReadyExplode)
			{
				JalapenoExplodeKillZombies(node, JalapenoIter, global->Iter);
			}
		}

		/* 删除樱桃炸弹 */
		if (JalapenoIter->second.BloodVolume <= 0)
		{
			PlayJalapenoExplode(node, JalapenoIter);
			global->MapSize.PlantsMap[(int)JalapenoIter->second.RowAndColumn.y][(int)JalapenoIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(JalapenoIter->second.tag);
			JalapenoAnimation.erase(JalapenoIter++);
		}
		else
		{
			JalapenoIter++;
		}
	}
}

void Jalapeno::PlayJalapenoExplode(Node* node, map<int, _JalapenoAnimation>::iterator JalapenoIter)
{
	PlaySoundEffect("cherrybomb");

	for (int i = 0; i < 9; i++)
	{
		auto Jalapeno_Fire = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("Jalapeno_Fire")->second);
		Jalapeno_Fire->setPosition(Vec2(570 + 122 * i + 60, JalapenoIter->second.Animation->getPositionY() - 10));
		Jalapeno_Fire->setAnimation(0, "animation", false);
		Jalapeno_Fire->setScaleY(1.3f);
		Jalapeno_Fire->setGlobalZOrder(SetZorder(JalapenoIter->second.Animation->getPositionY()));
		Jalapeno_Fire->setName("Jalapeno_Fire");
        node->addChild(Jalapeno_Fire); 
		
		Jalapeno_Fire->runAction(Sequence::create(DelayTime::create(2),
			CallFunc::create([=]()
				{
					node->removeChildByName("Jalapeno_Fire");
				}), nullptr));
	}
}

void Jalapeno::JalapenoExplodeKillZombies(Node* node, map<int, _JalapenoAnimation>::iterator JalapenoIter, map<int, Global::_Zombies>::iterator Iter)
{
	if (Iter->second.BloodVolume > 0 && Iter->second.ZombiesAnimation->isVisible() &&                                                /* 僵尸存活 */
		Iter->second.ZombiesAnimation->getPositionX() <= 1730 &&                                                                     /* 僵尸进入地图 */
		fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - JalapenoIter->second.Animation->getPositionY()) <= 10)   /* 僵尸与火爆辣椒在同一行 */
	{
		if (Iter->second.ShieldVolume1 < JalapenoIter->second.ExplosiveDamage) /* 如果护盾1血量小于爆炸伤害 */
		{
			if (Iter->second.ShieldVolume1 + Iter->second.ShieldVolume < JalapenoIter->second.ExplosiveDamage) /* 如果护盾1加护盾血量小于爆炸伤害 */
			{
				if (Iter->second.ShieldVolume1 + Iter->second.ShieldVolume + Iter->second.BloodVolume <= JalapenoIter->second.ExplosiveDamage) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
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
					Iter->second.BloodVolume = Iter->second.ShieldVolume1 + Iter->second.ShieldVolume + Iter->second.BloodVolume - JalapenoIter->second.ExplosiveDamage;
					Iter->second.ShieldVolume1 = 0;
					Iter->second.ShieldVolume = 0;
				}
			}
			else
			{
				/* 计算僵尸护盾剩于血量 */
				Iter->second.ShieldVolume = Iter->second.ShieldVolume1 + Iter->second.ShieldVolume - JalapenoIter->second.ExplosiveDamage;
				Iter->second.ShieldVolume1 = 0;
			}
		}
		else
		{
			Iter->second.ShieldVolume1 -= JalapenoIter->second.ExplosiveDamage;
		}
	}
}
