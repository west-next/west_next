#include "GameType.h"
#include "SunFlower.h"

SunFlower* SunFlower::_instance = nullptr;

SunFlower* SunFlower::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)SunFlower;
	}
	return _instance;
}

void SunFlower::Initialization()
{
	SunFlower::getInstance()->Suns.clear();
	SunFlower::getInstance()->Coins.clear();
	SunFlower::getInstance()->SunFlowerAnimations.clear();
	SunFlower::getInstance()->suntag = 0;
	SunFlower::getInstance()->cointag = 10000;
}

void SunFlower::CreateSunFlowerImage(const Vec2& position, Node* node)
{
	SunFlowerImage = CreateImage("SunFlower_2", position, node);
	SunFlowerImage->setScale(1.5f);
}

void SunFlower::CreateSunFlowerAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建向日葵动画 */
	auto SunFlowerAnimation = CreateAnimation("SunFlower_2", "SunFlower_Normal", position, 1.0f, 2.3f);
	SunFlowerAnimation->setAnchorPoint(Vec2(0.5f, 0.5f));
	SunFlowerAnimation->setScale(0.8f);
	SunFlowerAnimation->setSkin("SunFlower_Normal");
	node->addChild(SunFlowerAnimation, order, tag);

	_SunFlowerAnimation animation;
	animation.Animation = SunFlowerAnimation;
	animation.BloodVolume = 300;
	animation.tag = tag;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	SunFlowerAnimations.insert(pair<int, _SunFlowerAnimation>(tag, animation));

	/* 获取太阳层 */
	auto sunlayer = static_cast<Layer*>(node->getChildByName("SunLayer"));

	auto delaytime = DelayTime::create(5);
	auto delaytime2 = DelayTime::create(0.5f);
	auto delaytime1 = DelayTime::create(18);
	auto callfunc = CallFunc::create([=]()
		{
			SunFlowerAnimation->setSkin("SunFlower_ProduceSun"); /* 设置皮肤 */

			srand(time(nullptr)); rand() % 2 == 0 ? PlaySoundEffect("throw") : PlaySoundEffect("throw2");

			CreateSun(sunlayer, SunFlowerAnimation, Vec2(0, 0), 1);/* 创建太阳 */
		});
	auto callfunc1 = CallFunc::create([=]() 
		{
			SunFlowerAnimation->setSkin("SunFlower_Normal"); /* 设置皮肤 */
		});

	SunFlowerAnimation->runAction(RepeatForever::create(Sequence::create(delaytime, callfunc, delaytime2, callfunc1, delaytime1, nullptr)));
}

void SunFlower::CreateSun(Node* node, SkeletonAnimation* animation, const Vec2& position, const int& ID)
{
	suntag++;

	auto Sun= SkeletonAnimation::createWithData(global->GlobalAnimationData.find("Sun")->second);
	Sun->addAnimation(0, "Sun_rotate", true);
	Sun->setAnchorPoint(Vec2(0.5f, 0.5f));
	Sun->setGlobalZOrder(15);
	Sun->setTag(suntag);
	node->addChild(Sun);

	_Suns suns;
	suns.Animation = Sun;
	suns.IsUsed = false;

	Suns.insert(pair<int&, _Suns>(suntag, suns));

	srand(time(nullptr));
	switch (ID)
	{
	case 1:
		Sun->setScale(0.5f);
		Sun->setPosition(animation->getPosition());
		Sun->runAction(Sequence::create(
			Spawn::create(
				ScaleTo::create(0.5f, 1.2f), JumpBy::create(0.6f, Vec2(rand() % 120 - 60, 0), rand() % 100 + 100, 1), nullptr),
			DelayTime::create(14), CallFuncN::create(CC_CALLBACK_1(SunFlower::RemoveSunCallBack, this, node, suntag)), nullptr));
		break;
	case 2 :
		Sun->setScale(1.2f);
		Sun->setPosition(position);
		auto action = Sun->runAction(Sequence::create(MoveBy::create(5.0f, Vec2(0, -rand() % 500 - 500)), DelayTime::create(14),
			CallFuncN::create(CC_CALLBACK_1(SunFlower::RemoveSunCallBack, this, node, suntag)), nullptr));
		action->setTag(0);
		break;
	}
}

void SunFlower::RandomCreateSuns(Node* node)
{
	/* 获取太阳层 */
	auto sunlayer = static_cast<Layer*>(node->getChildByName("SunLayer"));
	sunlayer->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5), 
		CallFunc::create([=]()
		{
				CreateSun(sunlayer, nullptr, Vec2(rand() % 1100 + 800, 1150), 2);/* 创建太阳 */
		}), DelayTime::create(20), nullptr)));
}

void SunFlower::RandomCreateCoins(Node* node,const Vec2& position)
{
	if (rand() % 100 < 5)
	{
		PlaySoundEffect("moneyfalls");
		auto coin = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("coin")->second);
		coin->setScale(0.25f);
		coin->setAnimation(0, "rotate", true);
		coin->setPosition(position);
		coin->setGlobalZOrder(20);
		node->addChild(coin);

		_Coins coins;
		coins.Animation = coin;
		coins.IsUsed = false;

		Coins.push_back(coins);

		coin->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(rand() % 120 - 60, 0), rand() % 100 + 100, 1),DelayTime::create(15),
			CallFunc::create([=]() 
				{
					coin->setVisible(false);
					Coins.pop_front();
				}) ,nullptr));
	}
}

void SunFlower::RemoveSunCallBack(Node* node1,Node* node, const int& tag)
{
	Suns.erase(tag);
	node->getChildByTag(tag)->setVisible(false);
	//node->removeChildByTag(tag);
}

void SunFlower::CreateSunListener(Node* node)
{
	auto linster = EventListenerTouchOneByOne::create();
	linster->onTouchBegan = [=](Touch* t, Event* e)
	{
		Point p = t->getLocation();

		for (SunsIter=Suns.begin(); SunsIter !=Suns.end();++SunsIter)
		{	
			if (SunsIter->second.Animation->getBoundingBox().containsPoint(p) && !SunsIter->second.IsUsed)
			{
				PlaySoundEffect("points");
				SunsIter->second.IsUsed = true;
				SunsIter->second.Animation->stopAllActions();
				SunsIter->second.Animation->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(290, 1010))),
					Spawn::create(ScaleTo::create(0.2f, 0.3f), FadeOut::create(0.2f), nullptr),
					CallFunc::create([=]()
						{
							global->SunNumbers += 50;
							UpdateSunNumbers(node);

							GameType::getInstance()->UpdateRequirementNumbers(node->getParent()->getChildByName("UiLayer"), "阳光数量增加");
						}), nullptr));
			}
		}

		for (CoinIter = Coins.begin(); CoinIter != Coins.end(); ++CoinIter)
		{
			if (CoinIter->Animation->getBoundingBox().containsPoint(p) && !CoinIter->IsUsed)
			{
				PlaySoundEffect("coin");
				CoinIter->IsUsed = true;
				CoinIter->Animation->stopAllActions();
				CoinIter->Animation->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(1650, -30))),
					Spawn::create(FadeOut::create(0.2f),
						CallFunc::create([=]()
							{
								global->CoinNumbers += 2;
								UpdateCoinNumbers(node);
							}), nullptr), nullptr));
			}
		}

		return true;
	};

	/* 创建监听 */
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(linster, node);
}

void SunFlower::ShowSunNumbers(Node* node)
{
	SunNumbersText = ui::Text::create();
	SunNumbersText->setFontSize(50);
	SunNumbersText->setFontName("resources/fonts/arial.ttf");
	SunNumbersText->setPosition(Vec2(410, 1010));
	SunNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	SunNumbersText->setColor(Color3B(255, 127, 39));
	SunNumbersText->setName("SunNumbersText");
	UpdateSunNumbers();
	node->addChild(SunNumbersText);

	node->schedule(CC_CALLBACK_1(SunFlower::Update, this, node), 0.1f, "UpdateSunNumbers");
}

void SunFlower::ShowCoinNumbers(Node* node)
{
	CoinNumbersText = ui::Text::create();
	CoinNumbersText->setFontSize(40);
	CoinNumbersText->setFontName("resources/fonts/arial.ttf");
	CoinNumbersText->setPosition(Vec2(1770, 50));
	CoinNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	CoinNumbersText->setColor(Color3B::YELLOW);
	UpdateCoinNumbers(node);
	node->addChild(CoinNumbersText);
}

void SunFlower::Update(float Time, Node* node) 
{
	JudgeSunFlowerAndZombies(node);   /* 判断植物与僵尸 */
}

void SunFlower::UpdateSunNumbers(Node* node)
{
	if (node)
	{
		auto sun = Sprite::create(global->ImagesPath.find("Sun")->second);
		sun->setPosition(Vec2(290, 1010));
		sun->setScale(0.9f);
		sun->setName("sun");
		node->addChild(sun);
		sun->runAction(Sequence::create(DelayTime::create(0.15f), CallFunc::create([=]() {node->removeChildByName("sun"); }), nullptr));
	}

	char sunnumbers[20];
	sprintf(sunnumbers, "%d", global->SunNumbers);
	SunNumbersText->setString(sunnumbers);
}

void SunFlower::UpdateCoinNumbers(Node* node)
{
	char coinnumbers[20];
	global->CoinNumbers == 0 ? sprintf(coinnumbers, "%d", 0) : sprintf(coinnumbers, "%d", global->CoinNumbers);
	CoinNumbersText->setString(coinnumbers);
}

void SunFlower::JudgeSunFlowerAndZombies(Node* node)
{
	for (SunflowerIter = SunFlowerAnimations.begin(); SunflowerIter != SunFlowerAnimations.end(); /*++SunflowerIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                    /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() >= SunflowerIter->second.position.x &&                     /* 僵尸在植物的前方 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - SunflowerIter->second.position.y) <= 10 &&   /* 僵尸与植物在同一行 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() <= 1730))                                             /* 僵尸进入地图 */
			{
				if (SunflowerIter->second.BloodVolume > 0 &&                                                             /* 植物没有死 */
					fabs(global->Iter->second.ZombiesAnimation->getPositionX() - SunflowerIter->second.position.x) <= 80)        /* 僵尸遇到植物 */
				{
					ZombiesEatPlants(global->Iter, &SunflowerIter->second.BloodVolume, SunflowerIter->second.Animation);
				}
				if (SunflowerIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
				{
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
		/* 删除 */
		if (SunflowerIter->second.BloodVolume <= 0)
		{
			global->MapSize.PlantsMap[(int)SunflowerIter->second.RowAndColumn.y][(int)SunflowerIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(SunflowerIter->second.tag);
			SunFlowerAnimations.erase(SunflowerIter++);
		}
		else
		{
			SunflowerIter++;
		}
	}
}

void SunFlower::SunNumberTextRunAction(Node* node)
{
	auto text = (ui::Text*)node->getChildByName("SunNumbersText");
	auto action = TintBy::create(0.1f, 255, 0, 0);
	text->runAction(Repeat::create(Sequence::create(action, action->reverse(),CallFunc::create([=]() 
		{
			text->setColor(Color3B(255, 127, 39));
		}), nullptr), 5));
}
