#include "Garlic.h"

Garlic* Garlic::_instance = nullptr;

Garlic* Garlic::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Garlic;
	}
	return _instance;
}

void Garlic::Initialization()
{
	Garlic::getInstance()->GarlicAnimations.clear();
}

void Garlic::CreateGarlicImage(const Vec2& position, Node* node)
{
	GarlicImage = CreateImage("Garlic", position, node);
	GarlicImage->setScale(1.5f);
}

void Garlic::CreateGarlicAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto GarlicAnimation = CreateAnimation("Garlic", "Garlic_Normal", position, 1.0f, 3.0f);
	GarlicAnimation->setAnchorPoint(Vec2(0, 0));
	GarlicAnimation->setScale(0.8f);
	node->addChild(GarlicAnimation, order, tag);

	_GarlicAnimation animation;
	animation.Animation = GarlicAnimation;
	animation.tag = tag;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	GarlicAnimations.insert(pair<int, _GarlicAnimation>(tag, animation));
}

void Garlic::CreateGarlicListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(Garlic::Update, this, node), "UpdateGarlic");
}

void Garlic::Update(float Time, Node* node)
{
	JudgeGarlicAndZombiesPosition(node);
}

void Garlic::JudgeGarlicAndZombiesPosition(Node* node)
{
	for (GarlicIter = GarlicAnimations.begin(); GarlicIter != GarlicAnimations.end(); /*++GarlicIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{ 
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() >= GarlicIter->second.Animation->getPositionX() &&                     /* 僵尸在植物的前方 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - GarlicIter->second.Animation->getPositionY()) <= 10)     /* 僵尸与植物在同一行 */
			{
				if (GarlicIter->second.BloodVolume > 0 &&                                                                            /* 植物没有死 */
					fabs(global->Iter->second.ZombiesAnimation->getPositionX() - GarlicIter->second.Animation->getPositionX()) <= 80)        /* 僵尸遇到植物 */
				{
					ZombiesEatPlants(global->Iter, &GarlicIter->second.BloodVolume, GarlicIter->second.Animation);
					
					if (!global->Iter->second.IsEatGarlic)
					{
						global->Iter->second.IsEatGarlic = true;
						GarlicIter->second.EatNumbers--;
						auto zombie = global->Iter;
						node->runAction(Sequence::create(DelayTime::create(1.0f),
							CallFunc::create([=]()
								{
									rand() % 2 == 0 ? PlaySoundEffect("squash_hmm") : PlaySoundEffect("squash_hmm2");
									if (zombie->second.ZombiesAnimation->getPositionY() == 130)
									{
										zombie->second.ZombiesAnimation->runAction(MoveBy::create(0.5f, Vec2(-20, 138)));
										zombie->second.ZombiesAnimation->setGlobalZOrder(zombie->second.ZombiesAnimation->getGlobalZOrder() - 2);
									}
									else if (zombie->second.ZombiesAnimation->getPositionY() == 682)
									{
										zombie->second.ZombiesAnimation->runAction(MoveBy::create(0.5f, Vec2(-20, -138)));
										zombie->second.ZombiesAnimation->setGlobalZOrder(zombie->second.ZombiesAnimation->getGlobalZOrder() + 2);
									}
									else
									{
										auto number = rand() % 2;
										zombie->second.ZombiesAnimation->runAction(MoveBy::create(0.5f, Vec2(-20, number == 0 ? 138 : -138)));
										zombie->second.ZombiesAnimation->setGlobalZOrder(zombie->second.ZombiesAnimation->getGlobalZOrder() + (number == 0 ? -2 : 2));
									}
									node->runAction(Sequence::create(DelayTime::create(0.5f),
										CallFunc::create([=]() 
										{
												zombie->second.IsEatGarlic = false; 
												if (!zombie->second.Speed && zombie->second.IsEat)
												{
													zombie->second.IsEat = false;
													if (zombie->second.AnimationID != 10)
													{
														zombie->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(zombie->second.ZombiesType), 1.3f);
														zombie->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(zombie->second.ZombiesType), true);
														zombie->second.Speed = zombie->second.CaveSpeed;
													}
												}
										}), nullptr));
								}), nullptr));
					}
				}
			}
		}
		if (GarlicIter->second.EatNumbers <= (int)GarlicIter->second.SumEatNumbers * 2.0f / 3 && GarlicIter->second.AnimationID == 0)
		{
			GarlicIter->second.AnimationID = 1;
			GarlicIter->second.Animation->setAnimation(0, "Garlic_Damage", true);
		}
		if (GarlicIter->second.EatNumbers <= (int)GarlicIter->second.SumEatNumbers * 1.0f / 3 && GarlicIter->second.AnimationID == 1)
		{
			GarlicIter->second.AnimationID = 2;
			GarlicIter->second.Animation->setAnimation(0, "Garlic_Damage2", true);
		}
		/* 删除大蒜 */
		if (GarlicIter->second.EatNumbers <= 0)
		{
			global->MapSize.PlantsMap[(int)GarlicIter->second.RowAndColumn.y][(int)GarlicIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(GarlicIter->second.tag);
			GarlicAnimations.erase(GarlicIter++);
		}
		else
		{
			GarlicIter++;
		}
	}
}
