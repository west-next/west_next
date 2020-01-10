#include "PotatoMine.h"

PotatoMine* PotatoMine::_instance = nullptr;

PotatoMine* PotatoMine::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)PotatoMine;
	}
	return _instance;
}

void PotatoMine::Initialization()
{
	PotatoMine::getInstance()->PotatoMineAnimations.clear();
}

void PotatoMine::CreatePotatoMineImage(const Vec2& position, Node* node)
{
	PotatoMineImage = CreateImage("PotatoMine", position, node);
	PotatoMineImage->setScale(1.0f);
}

void PotatoMine::CreatePotatoMineAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto PotatoMineAnimation = CreateAnimation("PotatoMine", "PotatoMine_Ready", position, 0.8f, 1.8f, false);
	PotatoMineAnimation->setAnchorPoint(Vec2(0, 0));
	PotatoMineAnimation->setScale(1.0f);
	node->addChild(PotatoMineAnimation, order, tag);
	
	_PotatoMineAnimation animation;
	animation.Animation = PotatoMineAnimation;
	animation.BloodVolume = 300;
	animation.tag = tag;
	animation.IsReady = false;
	animation.IsExplode = false;
	animation.ExplosiveDamage = 1800;
	animation.RecordSecond = 0;
	animation.BreakGround = 15;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	PotatoMineAnimations.insert(pair<int, _PotatoMineAnimation>(tag, animation));
}

void PotatoMine::CreatePotatoMineListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(PotatoMine::Update, this, node), "UpdatePotatoMine");
	node->schedule(CC_CALLBACK_1(PotatoMine::UpdatePotateMine, this), 1.0f, "PotatoMine");
}

void PotatoMine::Update(float Time, Node* node)
{
	JudgePeaShooterAndZombiesPosition(node);
}

void PotatoMine::UpdatePotateMine(float time)
{
	for (PotatoMineIter = PotatoMineAnimations.begin(); PotatoMineIter != PotatoMineAnimations.end(); ++PotatoMineIter)
	{
		PotatoMineIter->second.RecordSecond++;
		if (PotatoMineIter->second.RecordSecond >= PotatoMineIter->second.BreakGround && !PotatoMineIter->second.IsReady)
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("dirt_rise")->second), global->GlobalMusic.SoundEffect);
			PotatoMineIter->second.IsReady = true;
			PotatoMineIter->second.BloodVolume = 300000;
			PotatoMineIter->second.Animation->setAnimation(0, "PotatoMine_Out", false);
			PotatoMineIter->second.Animation->addAnimation(0, "PotatoMine_Normal", true);
		}
	}
}

void PotatoMine::JudgePeaShooterAndZombiesPosition(Node* node)
{
	for (PotatoMineIter = PotatoMineAnimations.begin(); PotatoMineIter != PotatoMineAnimations.end(); /*++PotatoMineIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                     /* 僵尸没有死亡 */                                                                                          /* 僵尸没有死亡 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - PotatoMineIter->second.Animation->getPositionY()) <= 10 &&    /* 僵尸与植物在同一行 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - PotatoMineIter->second.Animation->getPositionX()) < 40)              /* 僵尸遇到植物 */
			{
				if (PotatoMineIter->second.IsReady)                                                                     /* 土豆雷准备好 */
				{
					PotatoMineIter->second.IsExplode = true;
				}
				if (PotatoMineIter->second.BloodVolume > 0)                                                             /* 植物没有死 */
				{
					ZombiesEatPlants(global->Iter, &PotatoMineIter->second.BloodVolume, PotatoMineIter->second.Animation);      /* 僵尸吃植物 */
				}
				if (PotatoMineIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
				{
					PotatoMineIter->second.Animation->setVisible(false);
					global->Iter->second.IsEat = false;
					if (global->Iter->second.AnimationID != 10)
					{
						global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.5f);
						global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
						global->Iter->second.Speed = global->Iter->second.CaveSpeed;
					}
				}
			}
		}
		if (PotatoMineIter->second.IsExplode)/* 土豆雷爆炸 */
		{
			if (!PotatoMineIter->second.IsFirst)
			{
				PotatoMineIter->second.IsFirst = true;
				PotatoMineIter->second.Animation->setScale(1.2f);
				PotatoMineIter->second.Animation->setAnimation(0, "PotatoMine_Blast", false);
				PotatoMineIter->second.Animation->setGlobalZOrder(10);
			}

			auto potatomineiter = PotatoMineIter;
			PotatoMineIter->second.Animation->setEventListener([=](spTrackEntry* entry, spEvent* event)
				{
					if (strcmp(event->data->name, "BlastBegin") == 0)
					{
						AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("potato_mine")->second), global->GlobalMusic.SoundEffect);
						PotatoMineExplodeKillZombies(node, potatomineiter);             /* 对僵尸产生伤害 */
						global->MapSize.PlantsMap[(int)potatomineiter->second.RowAndColumn.y][(int)potatomineiter->second.RowAndColumn.x] = -1;
					}
				});
			PotatoMineIter->second.Animation->runAction(Sequence::create(DelayTime::create(1.5f),
				CallFunc::create([=]()
					{
						potatomineiter->second.Animation->setVisible(false);
						potatomineiter->second.BloodVolume = 0;

					}), nullptr));
		}
		
		/* 删除土豆雷 */
		if (!PotatoMineIter->second.Animation->isVisible())
		{
			global->MapSize.PlantsMap[(int)PotatoMineIter->second.RowAndColumn.y][(int)PotatoMineIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(PotatoMineIter->second.tag);
			PotatoMineAnimations.erase(PotatoMineIter++);
		}
		else
		{
			PotatoMineIter++;
		}
	}
}

void PotatoMine::PotatoMineExplodeKillZombies(Node* node, map<int, _PotatoMineAnimation>::iterator PotatoMineIter)
{
	for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
	{
		if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                      /* 僵尸没有死亡 */
			fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - PotatoMineIter->second.Animation->getPositionY()) <= 10 &&     /* 僵尸与植物在同一行 */
			fabs(global->Iter->second.ZombiesAnimation->getPositionX() - PotatoMineIter->second.Animation->getPositionX()) < 150)              /* 僵尸在土豆雷爆炸范围内 */
		{
			if (global->Iter->second.ShieldVolume1 < PotatoMineIter->second.ExplosiveDamage) /* 如果护盾1血量小于爆炸伤害 */
			{
				if (global->Iter->second.ShieldVolume1 + global->Iter->second.ShieldVolume < PotatoMineIter->second.ExplosiveDamage) /* 如果护盾1加护盾血量小于爆炸伤害 */
				{
					if (global->Iter->second.ShieldVolume1 + global->Iter->second.ShieldVolume + global->Iter->second.BloodVolume <= PotatoMineIter->second.ExplosiveDamage) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
					{
						global->Iter->second.ZombiesAnimation->setVisible(false);
						global->Iter->second.ZombiesAnimation->pause();

						StrikeFlyZombies(node, "CommonZombieHead");
						StrikeFlyZombies(node, "ZombieArm");
					}
					else
					{
						/* 计算僵尸本体血量 */
						global->Iter->second.BloodVolume = global->Iter->second.ShieldVolume1 + global->Iter->second.ShieldVolume + global->Iter->second.BloodVolume - PotatoMineIter->second.ExplosiveDamage;
						global->Iter->second.ShieldVolume1 = 0;
						global->Iter->second.ShieldVolume = 0;
					}
				}
				else
				{
					/* 计算僵尸护盾剩于血量 */
					global->Iter->second.ShieldVolume = global->Iter->second.ShieldVolume1 + global->Iter->second.ShieldVolume - PotatoMineIter->second.ExplosiveDamage;
					global->Iter->second.ShieldVolume1 = 0;
				}
			}
			else
			{
				global->Iter->second.ShieldVolume1 -= PotatoMineIter->second.ExplosiveDamage;
			}
		}
	}
}

void PotatoMine::StrikeFlyZombies(Node* node, const string& NameHead)
{
	auto head = Sprite::create(global->ImagesPath.find(NameHead)->second);
	head->setPosition(global->Iter->second.ZombiesAnimation->getPosition() + Vec2(-40, 150));
	head->setGlobalZOrder(global->Iter->second.ZombiesAnimation->getGlobalZOrder());
	head->setScale(1.5f);
	node->addChild(head, global->Iter->second.ZombiesAnimation->getTag() + 1, "head");

	head->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(1.0f, Vec2(-400 + rand() % 800, -120), rand() % 300 + 200, 1),JumpBy::create(0.5f, Vec2(-50 + rand() % 100, 0), rand() % 50 + 50, 1), nullptr),
		RotateBy::create(0.5f, rand() % 2 == 0 ? 90 : -90), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f),
		CallFunc::create([=]()
			{
				node->removeChildByName("head");
			}), nullptr));

	/* 判断是否是小僵尸 */
	if (OpenLevelData::getInstance()->LevelData.ZombiesIsSmall)
	{
		head->setScale(0.7f);
	}
	/* 判断是否是巨人僵尸 */
	else if (OpenLevelData::getInstance()->LevelData.ZombiesIsBig)
	{
		head->setScale(1.8f);
	}
}

