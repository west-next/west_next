#include "Plants.h"
#include "Pause_Quit_Game.h"
#include "AdventureGameScene.h"

Scene* AdventrueGameScene::createScene()
{
	return AdventrueGameScene::create();
}

bool AdventrueGameScene::init()
{
	if (!Scene::init()) return false;
	srand(time(nullptr));

	AudioEngine::stopAll();
	global->GlobalMusic.BackGroundMusic.clear();

	switch (global->NowPlayLevels.PlayLevels)
	{
	case 35:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.2")->second, true));
		break;
	case 36:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.3")->second, true));
		break;
	case 37:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.7")->second, true));
		break;
	case 50:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.10")->second, true));
		break;
	case 51:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.11")->second, true));
		break;
	case 52:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic.mo3.12")->second, true));
		break;
	default:
		global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("KitanaiSekai")->second, true));
		break;
	}
	AudioEngine::setVolume(*global->GlobalMusic.BackGroundMusic.begin(), global->GlobalMusic.BackGroundMusicVolume);
	
	BackGroundLayer::create()->addScene(this);           /* 创建背景 */
	this->addChild(UiLayer, 500, "UiLayer");
	this->addChild(SunLayer, 66666, "SunLayer");
	this->addChild(ZombiesLayer, 100, "ZombiesLayer");
	this->addChild(CarLayer, 10000, "CarLayer");         
	this->CreateMenubutton(1);                           /* 创建菜单按钮 */
	this->CreateBank();                                  /* 创建铲子 */
	this->CreateCardButton();                            /* 创建卡牌按钮 */
	this->CreateMouse();                    	         /* 创建鼠标监听 */

	if (global->NowPlayLevels.PlayLevels != 36 && 
		global->NowPlayLevels.PlayLevels != 50 && 
		global->NowPlayLevels.PlayLevels != 52)
	{
		SunFlowers->RandomCreateSuns(this);       /* 出现阳光 */
	}
	zombies->ZombiesUpdate(ZombiesLayer);         /* 僵尸出现 */

	return true;
}

void AdventrueGameScene::Update(float Time)
{
	/* 计算植物种植位置 */
	this->CalculatePlantPosition();

	/* 控制卡牌是否可用 */
	this->ControlCardEnabled();

	/* 创建僵尸 */
	this->CreateZombies();                       
}

void AdventrueGameScene::CalculatePlantPosition()
{
	for (int i = 0; i < global->MapSize.RowNumbers; i++)
	{
		for (int j = 0; j < global->MapSize.ColumnNumbers; j++)
		{
			if (Cur.x > GrassPosition + 122 * j && Cur.x < GrassPosition + 122 * (j + 1) && Cur.y>110 + 138 * i && Cur.y < 110 + 138 * (i + 1))
			{
				PlantPlantsPosition.Plants_x = j;
				PlantPlantsPosition.Plants_y = i;
			}
		}
	}

	/* 如果不在范围内，移除到画面外 */
	if (Cur.x < GrassPosition || Cur.x>1668 || Cur.y < 110 || Cur.y>810)
	{
		PlantPlantsPosition.Plants_x = -100;
		PlantPlantsPosition.Plants_y = -100;
	}
}

void AdventrueGameScene::ControlCardEnabled()
{
	for (int i = 0; i < global->UserSelectCard.CardNumbers; i++)
	{
		/* 如果植物所需阳光数大于拥有阳光数 */
		if (PlantsButton.PlantsNeedSuns[global->UserSelectCard.CardID[i]] > global->SunNumbers)
		{
			PlantsButton.CardText[global->UserSelectCard.CardID[i]]->setColor(Color3B::RED);
		}
		else
		{
			PlantsButton.CardText[global->UserSelectCard.CardID[i]]->setColor(Color3B::BLACK);
		}
		/* 如果卡牌填充倒计时完成 */
		if (PlantsButton.TimeBarFinished[global->UserSelectCard.CardID[i]])
		{
			PlantsButton.Card[global->UserSelectCard.CardID[i]]->setEnabled(true);
			/* 如果植物所需阳光数小于阳光数 */
			if (PlantsButton.PlantsNeedSuns[global->UserSelectCard.CardID[i]] <= global->SunNumbers)
			{
				PlantsButton.Card[global->UserSelectCard.CardID[i]]->setColor(Color3B::WHITE);
			}
			else
			{
				PlantsButton.Card[global->UserSelectCard.CardID[i]]->setColor(Color3B::GRAY);
			}
		}
	}
}

void AdventrueGameScene::CreateBank()
{
	/* 游戏类型显示 */
	GameType::getInstance()->CreateGameType(UiLayer);
	UiImageText::getInstance()->CreateCar(CarLayer);                                          /* 小车 */
	progressbar = UiImageText::getInstance()->CreateProgressBar(UiLayer);                     /* 进度条 */
	UiImageText::getInstance()->CreateSunBank(UiLayer);                                       /* 太阳栏 */
	UiImageText::getInstance()->CreateSeedBank(UiLayer);                                      /* 卡牌栏 */
	UiImageText::getInstance()->CreateCoinbank(UiLayer);                                      /* 金币栏 */
	UiImageText::getInstance()->CreateShovelBank(UiLayer, &MouseSelectImage.IsSelectShovel);  /* 铲子栏 */
	UiImageText::getInstance()->CreateText(UiLayer);                                          /* 文字 */
	UiImageText::getInstance()->CreateControlSpeed(UiLayer);                                  /* 控制速度按钮 */
	UiImageText::getInstance()->CreateZombiesDieBank(UiLayer);                                /* 僵尸死亡数 */
	SunFlowers->ShowSunNumbers(SunLayer);                                                     /* 显示阳光数 */
	SunFlowers->ShowCoinNumbers(SunLayer);                                                    /* 显示金币数 */
	SunFlowers->CreateSunListener(SunLayer);                                                  /* 创建太阳光监听 */
}

void AdventrueGameScene::sliderAction(Ref* sender, Control::EventType /*controlEvent*/)
{
	/*ControlSlider* pSliderCtl = static_cast<ControlSlider*>(sender);
	float scale;
	scale = pSliderCtl->getValue();*/

	//director->getScheduler()->setTimeScale(scale);
}

void AdventrueGameScene::CreateCardButton()
{
	for (int i = 0; i < global->UserSelectCard.CardNumbers; i++)
	{
		auto Card = ui::Button::create(global->ImagesPath.find("SeedPacket_Larger")->second, global->ImagesPath.find("SeedPacket_Larger")->second);
		Card->setPosition(Vec2(110, 1008 - 103 * i));
		Card->setEnabled(false);
		Card->setColor(Color3B::GRAY);
		Card->setCascadeColorEnabled(true);
		UiLayer->addChild(Card);

		CreateLayer(global->UserSelectCard.CardID[i]);

		CreateSelectPlantsSkill(global->UserSelectCard.CardID[i]);

		/* 存储卡牌 */
		PlantsButton.Card[global->UserSelectCard.CardID[i]] = Card;

		/* 如果向日葵卡牌存在 设置可以用 */
		if (global->UserSelectCard.CardID[i] == 0)
		{
			PlantsButton.Card[global->UserSelectCard.CardID[i]]->setColor(Color3B::WHITE);
			PlantsButton.Card[global->UserSelectCard.CardID[i]]->setEnabled(true);
		}

		/* 记录所需阳光数 */
		PlantsButton.PlantsNeedSuns[global->UserSelectCard.CardID[i]] = PlantsNeedSuns[global->UserSelectCard.CardID[i]];

		/* 创建按钮上的图片数字并记录 */
		PlantsButton.CardText[global->UserSelectCard.CardID[i]] = ShowPlantsAndNeedSunNumbers(Card, global->UserSelectCard.CardID[i]);

		/* 创建倒计时进度条 */
		global->UserSelectCard.CardID[i] == 0 ? this->SetTimerBar(Card, 0, global->UserSelectCard.CardID[i]) : this->SetTimerBar(Card, PlantsCoolTime[global->UserSelectCard.CardID[i]], global->UserSelectCard.CardID[i]);

		Card->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					this->CreateMovePlantsImage(global->UserSelectCard.CardID[i]);
					break;
				}
			});
	}

	/* 定时函数 */
	this->schedule(schedule_selector(AdventrueGameScene::Update));
	this->schedule(CC_CALLBACK_1(AdventrueGameScene::ZombiesUpdateTimeing, this), 1.0f, "ZombiesUpdateTimeing");
}

void AdventrueGameScene::SetTimerBar(ui::Button* Card, const float& Time,const int& ID)
{
	auto TimerBar = ProgressTimer::create(Sprite::create(global->ImagesPath.find("SeedPacketSilhouette1")->second));
	TimerBar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	TimerBar->setBarChangeRate(Vec2(1, 0));       //设置横向
	TimerBar->setMidpoint(Vec2(0, 1));            //设置从左往右
	TimerBar->setPosition(Vec2(90, 48));
	TimerBar->setOpacity(150);
	TimerBar->setPercentage(100);
	TimerBar->runAction(Sequence::create(ProgressFromTo::create(Time, 100, 0),
		CallFuncN::create(CC_CALLBACK_1(AdventrueGameScene::TimeBarFinishedCallBack, this, Card, ID)), nullptr));
	Card->addChild(TimerBar);
	
	PlantsButton.TimerBar[ID] = TimerBar;
}

void AdventrueGameScene::TimeBarFinishedCallBack(Node* node, ui::Button* Card,const int& ID)
{
	/* 倒计时结束 */
	PlantsButton.TimeBarFinished[ID] = true;
}

void AdventrueGameScene::CreateMouse()
{
	/* 创建鼠标监听 */
	auto Mouse = EventListenerMouse::create();

	/* 鼠标移动 */
	Mouse->onMouseMove = [&](Event* event)
	{
		/* 获取鼠标位置 */
		Cur = ((EventMouse*)event)->getLocationInView();

		this->MouseMoveControl();
	};

	/* 鼠标按下 */
	Mouse->onMouseDown = [&](Event* event)
	{
		this->MouseDownControl((EventMouse*)event);
	};

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(Mouse, this);

	/* 创建键盘监听 */
	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* 空格键暂停 */
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			director->pushScene(Pause_Quit_Game::CreateScene());
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:                 /* 大小写w或者向上箭头加速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			if (director->getScheduler()->getTimeScale() == 1.5f)
			{
				director->getScheduler()->setTimeScale(1.0f);
				((ui::Button*)UiLayer->getChildByName("AddSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
			}
			else
			{
				director->getScheduler()->setTimeScale(1.5f);
				((ui::Button*)UiLayer->getChildByName("SubSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
				((ui::Button*)UiLayer->getChildByName("AddSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButtonDown")->second);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:                /* 大小写s或者向下箭头减速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			if (director->getScheduler()->getTimeScale() == 0.5f)
			{
				director->getScheduler()->setTimeScale(1.0f);
				((ui::Button*)UiLayer->getChildByName("SubSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
			}
			else
			{
				director->getScheduler()->setTimeScale(0.5f);
				((ui::Button*)UiLayer->getChildByName("AddSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
				((ui::Button*)UiLayer->getChildByName("SubSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButtonDown")->second);
			}
			break;
		default:
			break;
		}
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}

void AdventrueGameScene::MouseMoveControl()
{
	/* 如果鼠标选择了植物 */
	if (MouseSelectImage.IsSelectPlants)
	{
		if (PlantPlantsPosition.Plants_x == -100 || PlantPlantsPosition.Plants_y == -100 ||                    /* 如果不在地图内 */
			global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] == NOPLANTS) /* 如果没有种植物 */
		{
			Plant->SetImagePosition(this, Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 60));
		}
		if (global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] != NOPLANTS)
		{
			Plant->SetImagePosition(this, Vec2(-1000, -1000));
		}
	}

	/* 鼠标上有铲子 */
	if (MouseSelectImage.IsSelectShovel)
	{
		/* 循环把植物恢复到原来的颜色 */
		for (int i = 0; i < global->MapSize.RowNumbers; i++)
		{
			for (int j = 0; j < global->MapSize.ColumnNumbers; j++)
			{
				if (global->MapSize.PlantsMap[i][j] != 10000 &&
					global->MapSize.PlantsMap[i][j] != NOPLANTS)
				{
					auto Plants = static_cast<SkeletonAnimation*>(this->getChildByTag(j * 5 + i));
					Plants->setColor(Color3B::WHITE);
				}
			}
		}
		if (PlantPlantsPosition.Plants_x >= 0 && PlantPlantsPosition.Plants_x <= 8 &&                             /* 如果在地图范围内 */
			PlantPlantsPosition.Plants_y >= 0 && PlantPlantsPosition.Plants_y <= 4 &&
			global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] != 10000 &&
			global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] != NOPLANTS)    /* 种有植物 */
		{
			auto Plants = static_cast<SkeletonAnimation*>(this->getChildByTag(PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y));
			Plants->setColor(Color3B::GRAY);
		}
	}
}

void AdventrueGameScene::MouseDownControl(EventMouse* eventmouse)
{
	/* 如果鼠标坐标不在卡牌栏内并且鼠标上有植物 */
	if (MouseSelectImage.IsSelectPlants) 
	{
		if (Cur.x > 200)
		{
			if (PlantPlantsPosition.Plants_x >= 0 && PlantPlantsPosition.Plants_x <= 8 &&                             /* 如果在地图范围内 */
				PlantPlantsPosition.Plants_y >= 0 && PlantPlantsPosition.Plants_y <= 4 &&
				global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] == NOPLANTS)    /* 没有种植物 */
			{
				if (static_cast<int>(eventmouse->getMouseButton()) == 0) /* 左键 */
				{
					/* 记录使用植物数量 */
					UserDefault::getInstance()->setIntegerForKey("USEPLANTSNUMBERS", ++global->UserGameData.UsePlantsNumbers);

					/* 种植植物 */
					this->PlantPlants(MouseSelectImage.SelectPlantsID);

					/* 地图记录种植的植物 */
					global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] = MouseSelectImage.SelectPlantsID;

					/* 设置倒计时并且按钮不可用 */
					PlantsButton.TimeBarFinished[MouseSelectImage.SelectPlantsID] = false;
					PlantsButton.Card[MouseSelectImage.SelectPlantsID]->setEnabled(false);
					PlantsButton.Card[MouseSelectImage.SelectPlantsID]->setColor(Color3B::GRAY);
					PlantsButton.TimerBar[MouseSelectImage.SelectPlantsID]->runAction(
						Sequence::create(
							ProgressFromTo::create(PlantsCoolTime[MouseSelectImage.SelectPlantsID], 100, 0),
							CallFuncN::create(CC_CALLBACK_1(AdventrueGameScene::TimeBarFinishedCallBack, this, PlantsButton.Card[MouseSelectImage.SelectPlantsID], MouseSelectImage.SelectPlantsID)), nullptr));
				}
			}
			else
			{
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
				/* 卡牌颜色恢复 */
				PlantsButton.TimerBar[MouseSelectImage.SelectPlantsID]->setPercentage(0);
				PlantsButton.Card[MouseSelectImage.SelectPlantsID]->setColor(Color3B::WHITE);

				if (static_cast<int>(eventmouse->getMouseButton()) == 0) /* 左键 */
				{
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("buzzer")->second), global->GlobalMusic.SoundEffect);
					UiImageText::getInstance()->CreatePromptText(UiLayer, global->GameText.find("提示信息")->second);
				}
			}	

			/* 移除预览植物 */
			this->removeChildByName("Preview");
			director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);
			MouseSelectImage.IsSelectPlants = false;
		}

		if (static_cast<int>(eventmouse->getMouseButton()) == 1) /* 右键 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
			PlantsButton.TimerBar[MouseSelectImage.SelectPlantsID]->setPercentage(0);
			PlantsButton.Card[MouseSelectImage.SelectPlantsID]->setColor(Color3B::WHITE);
			/* 加上所需的阳光数并更新 */
			global->SunNumbers += PlantsButton.PlantsNeedSuns[MouseSelectImage.SelectPlantsID];
			SunFlowers->UpdateSunNumbers();
			/* 植物要求更新 */
			GameType::getInstance()->UpdateRequirementNumbers(UiLayer, "植物数量增加");

			/* 移除预览植物 */
			this->removeChildByName("Preview");
			director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);
			MouseSelectImage.IsSelectPlants = false;
		}
	}

	if (MouseSelectImage.IsSelectShovel &&                                                                /* 鼠标上有铲子 */
		!(ui::Button*)(UiLayer->getChildByName("ShovelBank"))->getBoundingBox().containsPoint(Cur))       /* 鼠标不在铲子栏内 */
	{
		AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("plant2")->second), global->GlobalMusic.SoundEffect);
		if (PlantPlantsPosition.Plants_x >= 0 && PlantPlantsPosition.Plants_x <= 8 &&                             /* 如果在地图范围内 */
			PlantPlantsPosition.Plants_y >= 0 && PlantPlantsPosition.Plants_y <= 4 &&
			global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] != 10000 &&
			global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x] != NOPLANTS)    /* 种有植物 */
		{
			/* 铲除植物 */
			this->DeletePlants(global->MapSize.PlantsMap[PlantPlantsPosition.Plants_y][PlantPlantsPosition.Plants_x], PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
			
			Zombie::getInstance()->ErgodicZombies();
		}
		/* 铲子消失 */
		MouseSelectImage.IsSelectShovel = false;
		director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);
	}
}

void AdventrueGameScene::CreateMovePlantsImage(const int& ID)
{
	switch (MouseSelectImage.IsSelectPlants)
	{
	case true:
		/* 播放音乐 */
		AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
		/* 鼠标改变 */
		director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);
		/* 卡片还原 */
		PlantsButton.TimerBar[MouseSelectImage.SelectPlantsID]->setPercentage(0);
		PlantsButton.Card[ID]->setColor(Color3B::WHITE);

		/* 加上所需的阳光数并更新 */
		global->SunNumbers += PlantsButton.PlantsNeedSuns[MouseSelectImage.SelectPlantsID];
		SunFlowers->UpdateSunNumbers();
		
		/* 移除预览植物 */
		this->removeChildByName("Preview");
		MouseSelectImage.IsSelectPlants = false;

		/* 植物要求更新 */
		GameType::getInstance()->UpdateRequirementNumbers(UiLayer, "植物数量增加");
		break;
	case false:
		if (PlantsButton.PlantsNeedSuns[ID] > global->SunNumbers)                               /* 如果植物所需阳光大于拥有阳光，则发出警告 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("buzzer")->second), global->GlobalMusic.SoundEffect);
			SunFlowers->SunNumberTextRunAction(SunLayer);
		}
		else if (GameType::getInstance()->PlantsRequriement.IsHavePlantsRequriement &&
			GameType::getInstance()->PlantsRequriement.SurPlusPlantsNumbers <= 0)       /* 剩余植物数量小于等于0 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("buzzer")->second), global->GlobalMusic.SoundEffect);
			GameType::getInstance()->WaringPlantsNull();
		}
		else
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("seedlift")->second), global->GlobalMusic.SoundEffect);

			/* 选择植物图片 */
			this->SelectPalntsImage(ID);

			/* 减去所需的阳光数并更新 */
			global->SunNumbers -= PlantsButton.PlantsNeedSuns[ID];
			SunFlowers->UpdateSunNumbers();

			/* 卡片变黑 */
			PlantsButton.Card[ID]->setColor(Color3B::GRAY);
			PlantsButton.TimerBar[ID]->setPercentage(100);

			/* 鼠标选择标记 */
			MouseSelectImage.IsSelectPlants = true;
			MouseSelectImage.SelectPlantsID = ID;

			/* 植物要求更新 */
			GameType::getInstance()->UpdateRequirementNumbers(UiLayer, "植物数量减少");
		}
		break;
	}
}

void AdventrueGameScene::CreateZombies()
{
	/* 刷新僵尸 */
	if (ZombiesAppearControl.LastFrequencyZombiesWasDeath)
	{
		ZombiesAppearControl.LastFrequencyZombiesWasDeath = false;
		if (ZombiesAppearControl.ZombiesAppearFrequency < LevelData->LevelData.ZombiesFrequency)
		{
			int zombiesnumbers = zombies->CreateZombiesForNumbers(ZombiesAppearControl.ZombiesAppearFrequency);
			for (int i = 0; i < zombiesnumbers; i++)
			{
				zombies->CreateZombies(Vec2(1780 + rand() % 800, ZombiesAppearControl.ZombiesPosition[rand() % 5]),
					ZombiesLayer, ZombiesTag++, 2 + rand() % 2, zombies->CreateZombiesForProbility(ZombiesAppearControl.ZombiesAppearFrequency), false);
			}
			/* 设置不同的僵尸在不同的游戏类型 */
			zombies->SetZombiesForGameType();

			ZombiesAppearControl.ZombiesAppearFrequency++;

			float percent = ZombiesAppearControl.ZombiesAppearFrequency / (float)LevelData->LevelData.ZombiesFrequency;
			progressbar->runAction(ProgressFromTo::create(10.0f, LevelLastPrecent * 100, percent * 100));
			LevelLastPrecent = percent;
		}
	}
	UiLayer->getChildByName("ProgressBar")->getChildByName("progressbarhead")->setPosition(Vec2(470 - 460 * (progressbar->getPercentage() / 100.0f), 22));

	/* 控制僵尸的刷新 */
	if (IsRefurbishZombies())
	{
		if (ZombiesAppearControl.ZombiesAppearFrequency)
		{
			ZombiesAppearControl.Time = 0; /* 距离上一波刷新时间清零 */
		}
		ZombiesAppearControl.LastFrequencyZombiesWasDeath = true;
		ZombiesAppearControl.IsBegin = false;
		UserDefault::getInstance()->flush();
	}

	/* 控制大波僵尸来袭的文字与音乐 */
	if (ZombiesAppearControl.Time >= LevelData->LevelData.FirstFrequencyTime && ZombiesAppearControl.ZombiesAppearFrequency == 0)
	{
		AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("awooga")->second), global->GlobalMusic.SoundEffect);
	}
    if (ZombiesAppearControl.ZombiesAppearFrequency == LevelData->LevelData.ZombiesFrequency && !ZombiesAppearControl.IsShowWords)
	{
		if (progressbar->getPercentage() >= LevelLastPrecent * 100 - 8)
		{
			auto flag = dynamic_cast<Sprite*>(UiLayer->getChildByName("ProgressBar")->getChildByName("_progressbarflag"));
			flag->setTexture(global->ImagesPath.find("ProgressBarFlag1")->second);
			CreateGameWords("Final");
			ZombiesAppearControl.IsShowWords = true;
		}
	}
	bool istrue = false;
	for (unsigned int i = 0; i < LevelData->LevelData.MunchZombiesFrequency.size(); i++)
	{
		if (ZombiesAppearControl.ZombiesAppearFrequency == LevelData->LevelData.MunchZombiesFrequency.at(i) && !ZombiesAppearControl.IsShowWords)
		{
			if (progressbar->getPercentage() >= LevelLastPrecent * 100 - 8)
			{
				CreateGameWords("ManyZombiesComing");
				ZombiesAppearControl.IsShowWords = true;
			}
		}
		
		if (ZombiesAppearControl.ZombiesAppearFrequency == LevelData->LevelData.MunchZombiesFrequency.at(i))
		{
			if (progressbar->getPercentage() >= LevelLastPrecent * 100 - 8)
			{
				char name[20] = { "" };
				sprintf(name, "progressbarflag%d", i);
				auto flag = dynamic_cast<Sprite*>(UiLayer->getChildByName("ProgressBar")->getChildByName(name));
				flag->setTexture(global->ImagesPath.find("ProgressBarFlag1")->second);
				istrue = true;
			}
		}
	}
	if (ZombiesAppearControl.IsShowWords &&
		!istrue &&
		ZombiesAppearControl.ZombiesAppearFrequency != LevelData->LevelData.ZombiesFrequency)
	{
		ZombiesAppearControl.IsShowWords = false;
	}

	/* 关卡结束 */
	if (global->Zombies.size() <= 0 && ZombiesAppearControl.ZombiesAppearFrequency >= LevelData->LevelData.ZombiesFrequency)
	{
		director->getScheduler()->setTimeScale(1.0f);/* 恢复播放倍数 */
		director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT); /* 恢复鼠标样式 */

		SetStop();

		if (request->JudgeUserWin() == 100)
		{
			if (global->NowPlayLevels.PlayLevels >= 52)
			{
				director->getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::createScene()));
			}
			UserWin();
		}
		else
		{
			GameOver(); /* 闯关失败 */
		}
	}
}

void AdventrueGameScene::GameOver()
{
	/* 记录闯关失败个数 */
	UserDefault::getInstance()->setIntegerForKey("BREAKTHROUGH", ++global->UserGameData.BreakThrough);
	UserDefault::getInstance()->flush();

	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("losemusic")->second), global->GlobalMusic.SoundEffect);
	UserWinRequirement::getInstance()->CreateDialogBox(UiLayer, request->JudgeUserWin(), 1);

	this->runAction(Sequence::create(DelayTime::create(5),
		CallFunc::create([]()
			{
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, World_1::createScene()));
			}), nullptr));

	LevelData->LevelData.FlowerPosition = 0;
}

void AdventrueGameScene::ZombiesUpdateTimeing(float Time)
{	
	if (!ZombiesAppearControl.IsBegin)
	{
		ZombiesAppearControl.Time = 0;
		ZombiesAppearControl.IsBegin = true;
	}

	/* 计时 */
	ZombiesAppearControl.Time++;
}

void AdventrueGameScene::CreateGameWords(const string& name)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("finalwave")->second), global->GlobalMusic.SoundEffect);
	auto zombiescoming = Sprite::create(global->ImagesPath.find(name)->second);
	zombiescoming->setPosition(director->getWinSize() / 2);
	zombiescoming->setGlobalZOrder(11);
	zombiescoming->setScale(5.0f);
	zombiescoming->setName("zombiescoming");
	this->addChild(zombiescoming);
	zombiescoming->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), DelayTime::create(1.5f),
		CallFunc::create([=]()
			{
				this->removeChildByName("zombiescoming");
			}), nullptr));
}

bool AdventrueGameScene::IsRefurbishZombies()
{
	if ((ZombiesAppearControl.Time >= LevelData->LevelData.FirstFrequencyTime &&
		 ZombiesAppearControl.ZombiesAppearFrequency == 0)                                                                       /* 第一波刷新控制 */

		|| (ZombiesAppearControl.Time >= 32 + rand() % 10 &&
		   (ZombiesAppearControl.ZombiesAppearFrequency == 1 || ZombiesAppearControl.ZombiesAppearFrequency == 2))               /* 第二三波刷新控制 */

		|| ZombiesAppearControl.Time >= 45                                                                                       /* 如果大于45秒刷新下一波 */

		|| (global->Zombies.size() <= 3 && ZombiesAppearControl.ZombiesAppearFrequency > 2)                                      /* 如果活着的僵尸数小于规定，刷新下一波 */

		|| (global->Zombies.size() <= 0 && ZombiesAppearControl.ZombiesAppearFrequency != 0)
		)
	{
		return true;
	}
	return false;
}

void AdventrueGameScene::UserWin()
{
	/* 记录关数 */
	if (global->NowLevels[global->NowPlayLevels.PlayWorlds].Levels == global->NowPlayLevels.PlayLevels)
	{
		
		UserDefault::getInstance()->setIntegerForKey(global->UserGameData.UserDataName[global->UserGameData.UserDataNumber].c_str(), ++(global->NowLevels[1].Levels));
		UserDefault::getInstance()->flush();
	}

	auto moneybag_hi_res = ui::Button::create(global->ImagesPath.find("moneybag_hi_res")->second);
	moneybag_hi_res->setPosition(Vec2(700 + rand() % 1000, 100 + rand() % 700));
	moneybag_hi_res->runAction(JumpBy::create(0.5f, Vec2(100 - rand() % 200, 0), 100 + rand() % 100, 1));
	moneybag_hi_res->setGlobalZOrder(20);
	moneybag_hi_res->setName("moneybag_hi_res");
	this->addChild(moneybag_hi_res);

	moneybag_hi_res->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.3f, 150, 150, 150), TintTo::create(0.3f, 255, 255, 255), nullptr)));

	moneybag_hi_res->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("moneyfalls")->second), global->GlobalMusic.SoundEffect);
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				moneybag_hi_res->setEnabled(false);
				moneybag_hi_res->setBright(true);
				auto cariter = Car::getInstance();
				for (cariter->cariter = cariter->Cars.begin(); cariter->cariter != cariter->Cars.end(); ++cariter->cariter)
				{
					if (!cariter->cariter->second.IsLive)
					{
						cariter->cariter->second.IsLive = true;
						cariter->cariter->second.AnimationID = 2;
						AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("plastichit2")->second), global->GlobalMusic.SoundEffect);

						cariter->cariter->second.Image->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.1f, 0), FadeOut::create(0.1f), nullptr), nullptr));
						cariter->cariter->second.Image->setVisible(false);
						CoinMove(cariter->cariter->second.Image->getPosition(), 0);
					}
				}

				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("moneyfalls")->second), global->GlobalMusic.SoundEffect);
				CreateCoin(moneybag_hi_res);
				break;
			}
		});


}

void AdventrueGameScene::SetStop()
{
	((ui::Button*)UiLayer->getChildByName("SubSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
	((ui::Button*)UiLayer->getChildByName("SubSpeed"))->setEnabled(false);
	((ui::Button*)UiLayer->getChildByName("AddSpeed"))->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
	((ui::Button*)UiLayer->getChildByName("AddSpeed"))->setEnabled(false);

	/* 定时器取消 */
	this->unschedule(schedule_selector(AdventrueGameScene::Update));
	this->unscheduleAllCallbacks();

	/* 卡牌不可用 */
	for (int i = 0; i < global->UserSelectCard.CardNumbers; i++)
	{
		PlantsButton.Card[global->UserSelectCard.CardID[i]]->setEnabled(false);
		PlantsButton.Card[global->UserSelectCard.CardID[i]]->setColor(Color3B::GRAY);
	}

	/* 铲子不可用 */
	ui::Button* ShovelBank = (ui::Button*)(UiLayer->getChildByName("ShovelBank"));
	ShovelBank->setEnabled(false);

	/* 暂停阳光产生 */
	for (SunFlowers->SunflowerIter = SunFlowers->SunFlowerAnimations.begin(); SunFlowers->SunflowerIter != SunFlowers->SunFlowerAnimations.end(); ++SunFlowers->SunflowerIter)
	{
		SunFlowers->SunflowerIter->second.Animation->stopAllActions();
	}
	SunLayer->stopAllActions();

	/* 鼠标切换 */
	director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);

}

void AdventrueGameScene::CreateCoin(ui::Button* button)
{
	const int number = LevelData->getInstance()->LevelData.CoinNumbers / 2 + rand() % (LevelData->getInstance()->LevelData.CoinNumbers / 2);

	button->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.05f, Vec2(5, 5)), MoveBy::create(0.05f, Vec2(-5, -5)), nullptr), number / 2), DelayTime::create(0.5f), FadeOut::create(0.5f),
		CallFunc::create([=]()
			{
				this->removeChildByName("moneybag_hi_res");
			}),
		nullptr));

	for (int i = 0; i < number; i++)
	{
		CoinMove(button->getPosition() - Vec2(-10, 55), i);

		if (i >= number - 1)
		{
			this->runAction(Sequence::create(DelayTime::create(2.0f),
				CallFunc::create([=]()
					{
						auto audio = AudioEngine::play2d(global->MusicsPath.find("winmusic")->second);
						AudioEngine::setVolume(audio, global->GlobalMusic.SoundEffect);
						AudioEngine::setFinishCallback(audio, [=](int i, string name)
							{
								AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("lightfill")->second), global->GlobalMusic.SoundEffect);
							});

						auto AwardRays = Sprite::create(global->ImagesPath.find("AwardRays")->second);
						AwardRays->setPosition(director->getWinSize() / 2.0f);
						AwardRays->runAction(RepeatForever::create(Spawn::create(RotateBy::create(0.5f, 30), ScaleBy::create(0.5f, 1.5f), nullptr)));
						AwardRays->setGlobalZOrder(20);
						this->addChild(AwardRays);

						auto White = Sprite::create(global->ImagesPath.find("White")->second);
						White->setAnchorPoint(Vec2(0, 0));
						White->setContentSize(director->getWinSize());
						White->setOpacity(0);
						White->setGlobalZOrder(20);
						this->addChild(White);
						White->runAction(Sequence::create(DelayTime::create(1.0f), FadeIn::create(7.0f),
							CallFunc::create([=]()
								{
									/* 保存金币数 */
									UserDefault::getInstance()->setIntegerForKey("COINNUMBERS", global->CoinNumbers);
									UserDefault::getInstance()->flush();

									director->replaceScene(World_1::createScene());
								}), nullptr));
					}), nullptr));
		}
	}
}

void AdventrueGameScene::CoinMove(const Vec2& position, const int& ID)
{
	auto coin = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("coin")->second);
	coin->setScale(0.05f);
	coin->setPosition(position);
	coin->setGlobalZOrder(20);
	coin->setOpacity(0);
	coin->setName("coin");
	this->addChild(coin);

	coin->runAction(Sequence::create(
		DelayTime::create(ID * 0.05f),
		Spawn::create(
			FadeIn::create(0.5f),
			ScaleTo::create(0.5f, 0.3f),
			JumpBy::create(0.5f, Vec2(150 - rand() % 300, 100 - rand() % 100), rand() % 100 + 200, 1), nullptr),
		DelayTime::create(0.05f),
		MoveTo::create(0.5f, Vec2(1650, -40)),
		Spawn::create(ScaleTo::create(0.2f, 0.5f), FadeOut::create(0.2f),
			CallFunc::create([&]()
				{
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("moneyfalls")->second), global->GlobalMusic.SoundEffect);
					this->removeChildByName("coin");
					global->CoinNumbers += 2;
					SunFlowers->getInstance()->UpdateCoinNumbers(SunLayer);
				}), nullptr),
		nullptr));
}

void AdventrueGameScene::CreateLayer(const int& ID)
{
	Layer* LayerName[10] =
	{   
		PeaShooterLayer ,WallNutLayer ,CherryBombLayer,PotatpMineLayer ,CabbageLayer ,TorchwoodLayer ,
		SpikeweedLayer,GarlicLayer,FirePeaShooterLayer,JalapenoLayer
	};

	const string LayerName1[] =
	{
		{"PeaLayer"},{"WallNutLayer"},{"CherryBombLayer"},{"PotatpMineLayer"},{"CabbageLayer"},{"TorchwoodLayer"},
	    {"SpikeweedLayer"},{"GarlicLayer"},{"FirePeaShooterLayer"},{"JalapenoLayer"}
	};

	if (ID < 11 && ID > 0)
	{
		this->addChild(LayerName[ID - 1], 1000 + ID, LayerName1[ID - 1]);
	}
}

void AdventrueGameScene::CreateSelectPlantsSkill(const int& ID)
{
	switch (ID)
	{
	case 1:
		PeaShooters->CreatePeaShooterListener(PeaShooterLayer);
		break;
	case 2:
		WallNuts->CreateWallNutListener(WallNutLayer);
		break;
	case 3:
		CherryBombs->CreateCherryBombListener(CherryBombLayer);
		break;
	case 4:
		PotatoMines->CreatePotatoMineListener(PotatpMineLayer);
		break;
	case 5:
		CabbagePults->CreateCabbagePultListener(CabbageLayer);
		break;
	case 6:
		Torchwoods->CreateTorchwoodListener(TorchwoodLayer);
		break;
	case 7:
		Spikeweeds->CreateSpikeweedListener(SpikeweedLayer);
		break;
	case 8:
		Garlics->CreateGarlicListener(GarlicLayer);
		break;
	case 9:
		FirePeaShooters->CreateFirePeaShooterListener(FirePeaShooterLayer);
		break;
	case 10:
		Jalapenos->CreateJalapenoListener(JalapenoLayer);
		break;
	default:
		break;
	}
}

void AdventrueGameScene::DeletePlants(const int& ID, const int& tag)
{
	switch (ID)
	{
	case 0:
		SunFlowers->SunFlowerAnimations.find(tag)->second.BloodVolume = 0;
		break;
	case 1:
		PeaShooters->PeaShooterAnimations.find(tag)->second.BloodVolume = 0;
		break;
	case 2:
		WallNuts->WallNutAnimations.find(tag)->second.BloodVolume = 0;
		break;
	case 3:
		CherryBombs->CherryBombAnimation.find(tag)->second.BloodVolume = 0;
		break;
	case 4:
		PotatoMines->PotatoMineAnimations.find(tag)->second.Animation->setVisible(false);
		break;
	case 5:
		CabbagePults->CabbagePultAnimations.find(tag)->second.BloodVolume = 0;
		break;
	case 6:
		Torchwoods->TorchwoodAnimation.find(tag)->second.BloodVolume = 0;
		break;
	case 7:
		Spikeweeds->SpikeweedAnimation.find(tag)->second.BloodVolume = 0;
		break;
	case 8:
		Garlics->GarlicAnimations.find(tag)->second.EatNumbers = 0;
		break;
	case 9:
		FirePeaShooters->FirePeaShooterAnimations.find(tag)->second.BloodVolume = 0;
		break;
	case 10:
		Jalapenos->JalapenoAnimation.find(tag)->second.BloodVolume = 0;
		break;
	default:
		break;
	}
}

void AdventrueGameScene::SelectPalntsImage(const int& ID)
{
	string PlantsIamgeName[] = 
	{
		{"SunFlower_2"},{"PeaShooter"},{"Wallnut_body"},{"CherryBomb"},{"PotatoMine"},{"Cabbage"},{"Torchwood"},
	    {"Spikeweed"},{"Garlic"},{"FirePeaShooter"},{"Jalapeno"}
	};
	string PlantsIamgeName1[] = 
	{ 
		{"SunFlower_2_1"},{"PeaShooter_1"},{"Wallnut_body_1"},{"CherryBomb_1"},{"PotatoMine_1"},{"Cabbage_1"},
	    {"Torchwood_1"},{"Spikeweed_1"},{"Garlic_1"},{"FirePeaShooter_1"},{"Jalapenos_1"}
	};
	if (global->GlobalParameter.Global_CursorNotHide == cocos2d::ui::CheckBox::EventType::UNSELECTED)
	{
		director->getOpenGLView()->setCursor(global->ImagesPath.find(PlantsIamgeName[ID])->second, Point::ANCHOR_MIDDLE);
	}
	else
	{
		director->getOpenGLView()->setCursor(global->ImagesPath.find(PlantsIamgeName1[ID])->second, Point::ANCHOR_MIDDLE);
	}

	switch (ID)
	{
	case 0:
		SunFlowers->CreateSunFlowerImage(Vec2(-1000, -1000), this);
		break;
	case 1:
		PeaShooters->CreatePeaShooterImage(Vec2(-1000, -1000), this);
		break;
	case 2:
		WallNuts->CreateWallNutImage(Vec2(-1000, -1000), this);
		break;
	case 3:
		CherryBombs->CreateCherryBombImage(Vec2(-1000, -1000), this);
		break;
	case 4:
		PotatoMines->CreatePotatoMineImage(Vec2(-1000, -1000), this);
		break;
	case 5:
		CabbagePults->CreateCabbagePultImage(Vec2(-1000, -1000), this);
		break;
	case 6:
		Torchwoods->CreateTorchwoodImage(Vec2(-1000, -1000), this);
		break;
	case 7:
		Spikeweeds->CreateSpikeweedImage(Vec2(-1000, -1000), this);
		break;
	case 8:
		Garlics->CreateGarlicImage(Vec2(-1000, -1000), this);
		break;
	case 9:
		FirePeaShooters->CreateFirePeaShooterImage(Vec2(-1000, -1000), this);
		break;
	case 10:
		Jalapenos->CreateJalapenoImage(Vec2(-1000, -1000), this);
		break;
	default:
		break;
	}
}

void AdventrueGameScene::PlantPlants(const int& ID)
{
	srand(time(nullptr));
	rand() % 2 == 0 ? AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("plant2")->second), global->GlobalMusic.SoundEffect) : AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("plant")->second), global->GlobalMusic.SoundEffect);
	switch (ID)
	{
	case 0:
		SunFlowers->CreateSunFlowerAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 1:
		PeaShooters->CreatePeaShooterAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 2:
		WallNuts->CreateWallNutAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 3:
		CherryBombs->CreateCherryBombAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 4:
		PotatoMines->CreatePotatoMineAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 5:
		CabbagePults->CreateCabbagePultAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 6:
		Torchwoods->CreateTorchwoodAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 7:
		Spikeweeds->CreateSpikeweedAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 8:
		Garlics->CreateGarlicAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 9:
		FirePeaShooters->CreateFirePeaShooterAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	case 10:
		Jalapenos->CreateJalapenoAnimation(Vec2(PlantPlantsPosition.Plants_x, PlantPlantsPosition.Plants_y),
			Vec2(GrassPosition + 122 * PlantPlantsPosition.Plants_x + 60, 110 + 138 * (PlantPlantsPosition.Plants_y + 1) - 125),
			this, 19 - PlantPlantsPosition.Plants_x + 5 - PlantPlantsPosition.Plants_y, PlantPlantsPosition.Plants_x * 5 + PlantPlantsPosition.Plants_y);
		break;
	default:
		break;
	}
}