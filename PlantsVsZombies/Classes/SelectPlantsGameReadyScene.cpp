#include "Quit_Game.h"
#include "Pause_Quit_Game.h"
#include "AdventureGameScene.h"
#include "SelectPlantsGameReadyScene.h"

constexpr auto BUTTONNUMBER = 10;

Scene* SelectPlantsGameReadyScene::createScene()
{
	return SelectPlantsGameReadyScene::create();
}

bool SelectPlantsGameReadyScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	/* 播放音乐 */
	global->changeBgMusic("mainmusic.mo3.1", true);

	/* 创建背景 */
	this->CreateBackGround();
	
	this->schedule(schedule_selector(SelectPlantsGameReadyScene::Update), 0.1f);;
	return true;
}


void SelectPlantsGameReadyScene::CreateBackGround()
{
	/* 视图大小 */
	Size VisableSize = director->getWinSize();
	/* 容器大小 */
	Size ScrollSize(2930, 1080);

	/* 创建图层（容器）*/
	auto scrollLayer = LayerColor::create(Color4B(255, 255, 255, 0));
	scrollLayer->setContentSize(ScrollSize);//设置容器大小

	/* 创建容器中的东西（精灵）*/
	scrollLayer->addChild(BackGroundLayer::create()->CreateBackGroundEffect(nullptr, 1) /* 创建背景 */);

	//创建滚动视图
	scrollView = extension::ScrollView::create(VisableSize, scrollLayer);
	scrollView->setPosition(Vec2(0, 0));
	scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);//设置只能纵向滚动
    scrollView->setTouchEnabled(false);//关闭触碰事件
	scrollView->runAction(Sequence::create(DelayTime::create(1.5f),
		CallFuncN::create([&](Node* node)
			{
				scrollView->setContentOffsetInDuration(Vec2(-1010, 0), 2.5f);
			}), nullptr));
	this->addChild(scrollView);

	/* 创建预览僵尸 */
	this->CreatePreviewZombies(global->NowPlayLevels.background);

	/* 创建菜单按钮 */
	this->CreateMenubutton(0);

	this->CreateUserName(global->GameText.find("的时空冒险之旅")->second);
}

void SelectPlantsGameReadyScene::CreateFire(Sprite* background)
{
	auto fire = ParticleFire::create();
	fire->setPosition(Vec2(1900, 30));
	background->addChild(fire);

	auto fire1 = ParticleFire::create();
	fire1->setPosition(Vec2(1820, 1000));
	background->addChild(fire1);
}

void SelectPlantsGameReadyScene::CreatePreviewZombies(Sprite* name)
{
	global->Zombies.clear();
	srand(time(nullptr));
	int n = 0;
	switch (global->BackGroundImageName)
	{
	case Global::_BackGroundImageName::Mordern:
		for (int i = 0; i < (int)LevelData->LevelData.ZombiesType.size(); i++)
		{
			n++;
			zombies->CreateZombies(Vec2(2050 + rand() % 450, rand() % 600 + 80), name, 10 + n, rand() % 2, LevelData->LevelData.ZombiesType.at(i), true);
		}
		zombies->SetZombiesForGameType();
		break;
	case Global::_BackGroundImageName::DarkAges:
		break;
	case Global::_BackGroundImageName::NeonMixtapeTour:
		break;
	case Global::_BackGroundImageName::FrostibiteCaves:
		break;
	default:
		break;
	}
	
	SortZombies();
}

bool cmp(SelectPlantsGameReadyScene::_PreViewZombies& a, SelectPlantsGameReadyScene::_PreViewZombies& b)
{
	return a.PositionY > b.PositionY;
}

void SelectPlantsGameReadyScene::SortZombies()
{
	PreviewZombies.clear();
	for (auto Iter : global->Zombies)
	{
		_PreViewZombies _zombies;
		_zombies.PreviewZombie = Iter.second.ZombiesAnimation;
		_zombies.PositionY = Iter.second.ZombiesAnimation->getPositionY();
		PreviewZombies.push_back(_zombies);
	}
	global->Zombies.clear();

	std::sort(PreviewZombies.begin(), PreviewZombies.end(), cmp);

	int i = 0;
	std::vector< _PreViewZombies>::iterator Preview;
	for (Preview = PreviewZombies.begin(); Preview != PreviewZombies.end(); ++Preview)
	{
		i++;
		Preview->PreviewZombie->setLocalZOrder(110 + i);
		Preview->PreviewZombie->setOpacity(255);
		Preview->PreviewZombie->getChildByName("shadow")->setOpacity(255);
	}
}

void SelectPlantsGameReadyScene::CreateMenubutton(const int& ID)
{
	/* 创建退出按钮 */
	auto button = ui::Button::create(global->ImagesPath.find("StopButton")->second, global->ImagesPath.find("StopButtonDown")->second);
	button->setPosition(Vec2(1870, 1030));
	button->setScale(0.7f);
	this->addChild(button);
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (ID)
				{
				case 0:
					director->pushScene(Quit_Game::CreateScene());
					break;
				case 1:
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("pause")->second), global->GlobalMusic.SoundEffect);
					global->CaveScene.Temporary = director->getRunningScene();
					global->CaveScene.Level = global->NowPlayLevels.PlayLevels;

					director->pushScene(Pause_Quit_Game::CreateScene());
					break;
				}
				
				break;
			}
		});
}

void SelectPlantsGameReadyScene::CreateUserName(const std::string& name)
{
	char buff[128];
	sprintf(buff, "第 %d 天", global->NowPlayLevels.PlayLevels);

	auto username = ui::Text::create();
	username->setString(global->UserData.UserName + name + global->NowPlayLevels.PlayWorldName + buff);
	username->setFontName("resources/fonts/fzse_gbk.ttf");
	username->setFontSize(60);
	username->setColor(Color3B::YELLOW);
	username->setName("username");
	username->setPosition(Vec2(director->getWinSize().width / 2.0f, 150));
	this->addChild(username);
}

void SelectPlantsGameReadyScene::Update(float Time)
{
	if (scrollView->getContentOffset().x <= -1010)
	{
		/* 取消定时函数 */
		this->unschedule(schedule_selector(SelectPlantsGameReadyScene::Update));

		/* 创建对话框 */
		this->CreateDialogs();
	}
}

void SelectPlantsGameReadyScene::CreateDialogs()
{
	/* 删除名字 */
	this->removeChildByName("username");

	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("swing")->second), global->GlobalMusic.SoundEffect);

	/* 卡牌栏 */
	auto SeedBank = Scale9Sprite::create(global->ImagesPath.find("SeedBank")->second);
	SeedBank->setPosition(Vec2(0, 1080));
	SeedBank->setAnchorPoint(Vec2(0, 1));
	SeedBank->setCapInsets(Rect(0, 5, 87, 373));
	SeedBank->setContentSize(Size(217.5f, 980));
	this->addChild(SeedBank);

	for (int i = 0; i < 9; i++)
	{
		auto Seed = Sprite::create(global->ImagesPath.find("SeedPacketSilhouette")->second);
		Seed->setContentSize(Size(180, 100));
		Seed->setPosition(Vec2(110, 905 - 103 * i));
		SeedBank->addChild(Seed);
	}

	/* 卡牌选择 */
	SeedChooser = Sprite::create(global->ImagesPath.find("SeedChooser_Background")->second);
	SeedChooser->setPosition(Vec2(600, -600));
	SeedChooser->setContentSize(Size(780, 1080));
	SeedChooser->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(600, 540)), CallFuncN::create(CC_CALLBACK_1(SelectPlantsGameReadyScene::SelectPlantsCallBack, this, 1)), nullptr));
	this->addChild(SeedChooser);

	/* 选择植物背景 */
	auto SelectBackground = Sprite::create(global->ImagesPath.find("SelectBackground")->second);
	SelectBackground->setPosition(Vec2(190, 730));
	SelectBackground->setScaleY(0.96f);
	SelectBackground->setScaleX(1.14f);
	SelectBackground->setColor(Color3B(0, 255, 255));
	SeedChooser->addChild(SelectBackground);
	SelectBackground->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2f, 150), FadeTo::create(0.2f, 50), nullptr)));

	auto State = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("Stage")->second);
	State->setPosition(Vec2(195, 720));
	State->setScale(3.5f);
	State->setAnimation(0, "Rotate", true);
	SeedChooser->addChild(State);

	auto SeedChooserText = ui::Text::create(global->GameText.find("请选择你出战的植物！")->second, "resources/fonts/fzse_gbk.ttf", 50);
	SeedChooserText->setColor(Color3B::YELLOW);
	SeedChooserText->setPosition(Vec2(400, 1000));
	SeedChooser->addChild(SeedChooserText);

	/* 创建植物卡牌滚动视图 */
	this->CreatePlantsCardScrollView();

	for (int i = 0; i < 23; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CreateButtons(Vec2(110 + 185 * j, 490 - 105 * i), 4 * i + j);
		}
	}
	SeedChooser->addChild(PlantCardScrollView);
	SeedChooser->addChild(PlantCardTextScrollView);

	/* 预加载动画介绍 */
	this->PreloadText();

	/* 开始进入创建随机动画 */
	srand(time(nullptr));
	this->CreatePlantsAnimation(rand() % 3);
}

void SelectPlantsGameReadyScene::CreatePlantsCardScrollView()
{
	/* 植物卡牌滑动视图 */
	PlantCardScrollView = ui::ScrollView::create();
	PlantCardScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	PlantCardScrollView->setContentSize(Size(800.0f, 420));
	PlantCardScrollView->setPosition(Vec2(0, 120));
	PlantCardScrollView->setInnerContainerSize(Size(800, 2500));
	PlantCardScrollView->setBounceEnabled(true);
	PlantCardScrollView->setScrollBarPositionFromCorner(Vec2(40, 0));
	PlantCardScrollView->setScrollBarWidth(7);
	PlantCardScrollView->setScrollBarAutoHideTime(2.0f);
	PlantCardScrollView->setScrollBarColor(Color3B::WHITE);
	this->schedule(schedule_selector(SelectPlantsGameReadyScene::ScrollViewUpdate), 0.1f);

	/* 植物卡牌介绍滑动视图 */
	PlantCardTextScrollView = ui::ScrollView::create();
	PlantCardTextScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	PlantCardTextScrollView->setContentSize(Size(400.0f, 360.0f));
	PlantCardTextScrollView->setInnerContainerSize(Size(400, 620));
	PlantCardTextScrollView->setPosition(Vec2(360, 580));
	PlantCardTextScrollView->setBounceEnabled(true);
	PlantCardTextScrollView->setScrollBarPositionFromCorner(Vec2(10, 0));
	PlantCardTextScrollView->setScrollBarAutoHideTime(2.0f);
	PlantCardTextScrollView->setScrollBarWidth(7);
	PlantCardTextScrollView->setScrollBarColor(Color3B::WHITE);

	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [&](Event* event)
	{
		auto e = (EventMouse*)event;
		auto cur = e->getLocationInView();
		auto a = e->getScrollY();
		cur.x -= 200;

		if (PlantCardScrollView->getBoundingBox().containsPoint(cur))
		{
			if (PlantCardScroll + a >= 0 && PlantCardScroll + a <= 50)
			{
				PlantCardScroll += a;
			}
			if (PlantCardScroll < 0)PlantCardScroll = 0;
			if (PlantCardScroll > 50)PlantCardScroll = 50;
			PlantCardScrollView->jumpToPercentVertical(PlantCardScroll * 2);
		}
		/*else if (PlantCardTextScrollView->getBoundingBox().containsPoint(cur))
		{
			if (PlantCardTextScroll + a >= 0 && PlantCardTextScroll + a <= 18)
			{
				PlantCardTextScroll += a;
			}
			if (PlantCardTextScroll < 0)PlantCardTextScroll = 0;
			if (PlantCardTextScroll > 20)PlantCardTextScroll = 20;
			PlantCardTextScrollView->jumpToPercentVertical(PlantCardTextScroll * 8);
		}*/
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, this);
}

void SelectPlantsGameReadyScene::PreloadText()
{
	for (int i = 0; i < 11; i++)
	{
		AnimationText[i] = ui::Text::create("", "resources/fonts/fzse_gbk.ttf", 30);
		AnimationText[i]->setName("AnimationText");
		AnimationText[i]->enableOutline(Color4B(0, 255, 255, 255));
		AnimationText[i]->enableShadow(Color4B::GRAY, Size(4, -4), 1);
		switch (i)
		{
		case 0:
			SeedChooser->addChild(AnimationText[i]);
			break;
		default:
			PlantCardTextScrollView->addChild(AnimationText[i]);
			break;
		}
	}
}

void SelectPlantsGameReadyScene::SelectPlantsCallBack(Node* node, const int& ID)
{
	switch (ID)
	{
	case 1:
	{
		auto button = ui::Button::create(global->ImagesPath.find("SeedChooser_Button_Disabled")->second, global->ImagesPath.find("SeedChooser_Button")->second);
		button->setTitleText(global->GameText.find("开始战斗吧！")->second);
		button->setTitleColor(Color3B::WHITE);
		button->setTitleFontName("resources/fonts/fzse_gbk.ttf");
		button->setTitleFontSize(20);
		button->setPosition(Vec2(390, 70));
		button->setScale(2.0f);
		button->setEnabled(false);
		button->setName("Begin");
		SeedChooser->addChild(button);
		button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
					break;
				case ui::Widget::TouchEventType::ENDED:
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("swing")->second), global->GlobalMusic.SoundEffect);
					/* 设置按钮不可用 */
					for (int i = 0; i < seedbankbutton[0].size; i++)
					{
						seedbankbutton[i].button->setEnabled(false);
						seedbankbutton[i].button->setColor(Color3B::GRAY);
						seedbankbutton[i].button->setCascadeColorEnabled(true);
					}
					if (seedbankbutton[0].size > 4)
					{
						SeedChooser->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(605, -600)), CallFuncN::create(CC_CALLBACK_1(SelectPlantsGameReadyScene::SelectPlantsCallBack, this, 2)), nullptr));
					}
					break;
				}
			});
	}
	break;
	case 2:
		scrollView->setContentOffsetInDuration(Vec2(-220, 0), 2.5f);//设置滚动方向与时间
		this->schedule(schedule_selector(SelectPlantsGameReadyScene::Update1));
		break;
	}

}

void SelectPlantsGameReadyScene::Update1(float Time)
{

	if (scrollView->getContentOffset().x >= -220)
	{
		/* 取消定时函数 */
		this->unschedule(schedule_selector(SelectPlantsGameReadyScene::Update1));

		/* 播放音乐 */
		AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("readysetplant")->second), global->GlobalMusic.SoundEffect);

		this->CreateReadyText(global->ImagesPath.find("StartReady")->second, 1);
	}
}

void SelectPlantsGameReadyScene::CreateReadyText(const std::string& name,const int &ID)
{
	static float time[] = { 0,0.2f,0.2f,1.5f };
	auto text = Sprite::create(name);
	text->setScale(15.0f);
	text->setName(name);
	text->setPosition(director->getWinSize() / 2.0f);

	text->runAction(Sequence::create(ScaleTo::create(0.3f, 2.0f), DelayTime::create(time[ID]),
			CallFuncN::create(CC_CALLBACK_1(SelectPlantsGameReadyScene::ReadyGameCallBack, this, name, ID)), nullptr));
	this->addChild(text);
}

void SelectPlantsGameReadyScene::ReadyGameCallBack(Node* node, const std::string &name,const int &ID)
{
	this->removeChildByName(name);
	switch (ID)
	{
	case 1:
		this->CreateReadyText(global->ImagesPath.find("StartSet")->second, 2);
		break;
	case 2:
		this->CreateReadyText(global->ImagesPath.find("StartPlant")->second, 3);
		break;
	default:

		/* 设置按钮可以用并记录用户选择的卡牌 */
		global->UserSelectCard.CardNumbers = seedbankbutton[0].size;
		for (int i = 0; i < seedbankbutton[0].size; i++)
		{
			global->UserSelectCard.CardID[i] = seedbankbutton[i].ID;
			//seedbankbutton[i].button->setEnabled(true);
		}

		// 初始化
		/* 设定初始阳光数 */
		global->SunNumbers = 100;

		/* 如果有要求则显示通过要求 */
		if (OpenLevelData::getInstance()->LevelData.GameType.size())
		{
			UserWinRequirement::getInstance()->CreateDialogBox(this, -1);
		}
		else
		{
			Director::getInstance()->replaceScene(AdventrueGameScene::createScene());
		}
		break;
	}
}

void SelectPlantsGameReadyScene::ScrollViewUpdate(float Time)
{
	PlantCardScrollViewPercent = (PlantCardScrollView->getScrolledPercentVertical()) * 25;
}

ui::Button* SelectPlantsGameReadyScene::CreateButtons(const Vec2& vec2, const int& ID)
{
	auto button = ui::Button::create(global->ImagesPath.find("SeedPacket_Larger")->second);
	button->setPosition(Vec2(vec2.x, PlantCardScrollView->getInnerContainerSize().height + vec2.y - 540));
	button->setTitleColor(Color3B::RED);
	button->setTitleFontSize(25);
	PlantCardScrollView->addChild(button);

	if (ID > BUTTONNUMBER)
	{
		button->setEnabled(false);
	}

	/* 显示阳光数与植物图片 */
	this->ShowPlantsAndNeedSunNumbers(button, ID);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:

				if (seedbankbutton[0].size > 3)/* 583 */
				{
					((Button*)SeedChooser->getChildByName("Begin"))->setEnabled(true);
					((Button*)SeedChooser->getChildByName("Begin"))->setTitleColor(Color3B::YELLOW);
				}
				if (seedbankbutton[0].size < 9)
				{
					this->CreateMoveButton(button, vec2, ID);
				}

				/* 创建植物动画 */
				this->CreatePlantsAnimation(ID);
				break;
			}
		});
	return button;
}

void SelectPlantsGameReadyScene::CreateMoveButton(ui::Button* button,const Vec2& vec2,const int &ID)
{
	/* 设置卡牌不再可以选择 */
	button->setEnabled(false);
	button->setColor(Color3B(70, 70, 70));
	button->setCascadeColorEnabled(true);/* 设置父节点影响子节点 */

	/* 创建移动卡牌 */
	auto MoveCard = ui::Button::create(global->ImagesPath.find("SeedPacket_Larger")->second);
	MoveCard->setPosition(Vec2(vec2.x + 212, vec2.y + PlantCardScrollViewPercent));
	MoveCard->setTitleColor(Color3B::RED);
	MoveCard->setTitleFontSize(25);
	MoveCard->setTag(ID);
	this->addChild(MoveCard);
	MoveCard->runAction(MoveTo::create(0.2f, Vec2(110, 1008 - 103 * seedbankbutton[0].size)));

	this->ShowPlantsAndNeedSunNumbers(MoveCard, ID);

	float PlantCardScrollViewPercentLast = PlantCardScrollViewPercent;
	/* 加载到卡牌栏中 */
	seedbankbutton[seedbankbutton[0].size].button = MoveCard;
	seedbankbutton[seedbankbutton[0].size++].ID = ID;
	
	MoveCard->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				/* 创建植物动画 */
				this->CreatePlantsAnimation(ID);

				/* 对植物卡牌重新排序 */
				this->SortPlantsCard(ID);

				/* 滚动到初始位置 */
				PlantCardScrollView->scrollToPercentVertical(PlantCardScrollViewPercentLast / 25.0f, 0.5f, true);
				PlantCardScroll = PlantCardScrollViewPercentLast / 25.0f;
				
				/* 植物卡牌取消选择 */
				MoveCard->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(vec2.x + 212, vec2.y + PlantCardScrollViewPercentLast)),
					CallFuncN::create(CC_CALLBACK_1(SelectPlantsGameReadyScene::RemovePlantsCardCallBack, this, ID, button)), nullptr));
				break;
			}
		});
}

void SelectPlantsGameReadyScene::RemovePlantsCardCallBack(Node* node,const int& ID, ui::Button* button)
{
	/* 删除移动卡牌精灵 */
	this->removeChildByTag(ID);

	/* 设置卡牌精灵可以再次选择 */
	button->setEnabled(true);
	button->setColor(Color3B::WHITE);
}

void SelectPlantsGameReadyScene::SortPlantsCard(const int &ID)
{
	/* 删除取消选择的卡牌 */
	for (int i = 0; i < seedbankbutton[0].size; i++)
	{
		if (seedbankbutton[i].ID == ID)
		{
			for (int j = i + 1; j < seedbankbutton[0].size; j++)
			{
				seedbankbutton[j - 1].button = seedbankbutton[j].button;
				seedbankbutton[j - 1].ID = seedbankbutton[j].ID;
			}
			seedbankbutton[0].size--;
			break;
		}
	}

	/* 为植物卡牌重新设置位置 */
	for (int i = 0; i < seedbankbutton[0].size; i++)
	{
		seedbankbutton[i].button->runAction(MoveTo::create(0.2f, Vec2(110, 1008 - 103 * i)));
	}

	if (seedbankbutton[0].size <= 4)
	{
		((Button*)SeedChooser->getChildByName("Begin"))->setEnabled(false);
		((Button*)SeedChooser->getChildByName("Begin"))->setTitleColor(Color3B::WHITE);
	}
}

ui::Text* SelectPlantsGameReadyScene::ShowPlantsAndNeedSunNumbers(ui::Button* button,const int& ID)
{
	/* 创建阳光数文本 */
	auto PlantsNeedSuns = ui::Text::create();
	PlantsNeedSuns->setFontName("resources/fonts/arial.ttf");
	PlantsNeedSuns->setColor(Color3B::BLACK);
	PlantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	PlantsNeedSuns->setPosition(Vec2(175, 20));
	PlantsNeedSuns->setFontSize(30);
	button->addChild(PlantsNeedSuns);

	const string PlantsImageName[][5] =
	{
		{{"SunFlower_2"},{"50"},{"PlantsIcon4"}},{{"PeaShooter"},{"100"},{"PlantsIcon5"}},{{"Wallnut_body"},{"50"},{"PlantsIcon6"}},{{"CherryBomb"},{"150"},{"PlantsIcon7"}},
		{{"PotatoMine"},{"25"},{"PlantsIcon7"}},{{"Cabbage"},{"100"},{"PlantsIcon3"}},{{"Torchwood"},{"175"},{"PlantsIcon2"}},{{"Spikeweed"},{"100"},{"PlantsIcon12"}},
	    {{"Garlic"},{"50"},{"PlantsIcon10"}},{{"FirePeaShooter"},{"200"},{"PlantsIcon2"}},{{"Jalapeno"},{"150"},{"PlantsIcon2"}}
	};
	Color3B color[] = 
	{
		Color3B::YELLOW,Color3B::GREEN,Color3B(140,80,10),Color3B::RED,Color3B(250,75,10),Color3B(64,0,0),Color3B::RED,Color3B::BLACK,Color3B(128,0,128),Color3B::RED,
		Color3B::RED
	};

	if (ID < 11)
	{
		this->CreateSprite(button, global->ImagesPath.find(PlantsImageName[ID][0])->second);
		PlantsNeedSuns->setString(PlantsImageName[ID][1]);

		auto PlantsLevelIamge = Sprite::create(global->ImagesPath.find("PlantsLevel_Copper")->second);
		PlantsLevelIamge->setPosition(Vec2(155, 74));
		PlantsLevelIamge->setScale(0.65f);
		button->addChild(PlantsLevelIamge);

		auto PlantsLevelText = ui::Text::create();
		PlantsLevelText->setFontName("resources/fonts/GameFont.ttf");
		PlantsLevelText->setColor(Color3B::BLACK);
		PlantsLevelText->setAnchorPoint(Vec2(1, 0.5f));
		PlantsLevelText->setPosition(Vec2(177, 77));
		PlantsLevelText->setFontSize(20);
		PlantsLevelText->setString("LVL1");
		button->addChild(PlantsLevelText);

		auto _PlantsIconBackground = Sprite::create(global->ImagesPath.find("PlantsIconBackground")->second);
		_PlantsIconBackground->setOpacity(0);
		button->addChild(_PlantsIconBackground);

		auto PlantsIconBackground = Sprite::create(global->ImagesPath.find("PlantsIconBackground")->second);
		PlantsIconBackground->setPosition(Vec2(93, 150));
		PlantsIconBackground->setScale(0.28f);
		PlantsIconBackground->setColor(color[ID]);
		_PlantsIconBackground->addChild(PlantsIconBackground);

		auto PlantsIcon = Sprite::create(global->ImagesPath.find(PlantsImageName[ID][2])->second);
		PlantsIcon->setPosition(Vec2(73, 75));
		PlantsIcon->setScale(1.2f);
		PlantsIconBackground->addChild(PlantsIcon);
	}
	
	return PlantsNeedSuns;
}

void SelectPlantsGameReadyScene::CreateSprite(ui::Button* button,const std::string resource, const float& scale)
{
	auto Card_Plants = Sprite::create(resource);
	Card_Plants->setScale(scale);
	Card_Plants->setPosition(Vec2(55, 50));
	button->addChild(Card_Plants);
}

void SelectPlantsGameReadyScene::CreatePlantsAnimation(const int& ID)
{
	SeedChooser->removeChildByName("PlantsAnimation");
	for (int i = 0; i < 10; i++)AnimationText[i]->setString("");

	switch (ID)
	{
	case 0:
		PlantCardTextScrollView->setInnerContainerSize(Size(400,430));
		this->CreateAnimation("SunFlower_2", "SunFlower_Normal", "SunFlower_Normal", Vec2(200, 610), 1.5f);
		this->CreateAnimationText(0, global->GameText.find("SUNFLOWER_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(1, global->GameText.find("SUNFLOWER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(2, global->GameText.find("SUNFLOWER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(3, global->GameText.find("SUNFLOWER_4")->second, Vec2(360, 830), 30, Color3B::YELLOW, false);
		break;
	case 1:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 610));
		this->CreateAnimation("PeaShooter", "PeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->CreateAnimationText(0, global->GameText.find("PEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("PEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("PEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("PEASHOOTER_4")->second, Vec2(360, 830), 30, Color3B::YELLOW, false);
		
		break;
	case 2:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 460));
		this->CreateAnimation("WallNut", "WallNut_Nornal", "WallNut_Normal", Vec2(200, 610), 1.5f);
		this->CreateAnimationText(0, global->GameText.find("WALLNUT_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("WALLNUT_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("WALLNUT_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("WALLNUT_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
	    break;
	case 3:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 650));
		this->CreateAnimation("CherryBomb", "CherryBomb_Normal", "", Vec2(200, 610), 1.8f);
		this->CreateAnimationText(0, global->GameText.find("CHERRYBOMB_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("CHERRYBOMB_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("CHERRYBOMB_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("CHERRYBOMB_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 4:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 650));
		this->CreateAnimation("PotatoMine", "PotatoMine_Normal", "", Vec2(200, 610), 1.8f);
		this->CreateAnimationText(0, global->GameText.find("POTATOMINE_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("POTATOMINE_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("POTATOMINE_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("POTATOMINE_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 5:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 430));
		this->CreateAnimation("Cabbage", "Cabbage_Normal", "", Vec2(200, 610), 1.3f);
		this->CreateAnimationText(0, global->GameText.find("CABBAGE_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("CABBAGE_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("CABBAGE_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("CABBAGE_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 6:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 540));
		this->CreateAnimation("Torchwood", "animation", "", Vec2(200, 610), 2.3f);
		this->CreateAnimationText(0, global->GameText.find("TORCHWOOD_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("TORCHWOOD_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("TORCHWOOD_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("TORCHWOOD_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 7:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 500));
		this->CreateAnimation("Spikeweed", "Spikeweed_Normal", "", Vec2(200, 610), 1.5f);
		this->CreateAnimationText(0, global->GameText.find("SPIKEWEED_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("SPIKEWEED_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("SPIKEWEED_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("SPIKEWEED_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 8:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 535));
		this->CreateAnimation("Garlic", "Garlic_Normal", "", Vec2(200, 610), 1.5f);
		this->CreateAnimationText(0, global->GameText.find("GARLIC_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("GARLIC_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("GARLIC_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("GARLIC_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 9:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 470));
		this->CreateAnimation("FirePeaShooter", "FirePeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->CreateAnimationText(0, global->GameText.find("FIREPEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("FIREPEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("FIREPEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("FIREPEASHOOTER_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 10:
		PlantCardTextScrollView->setInnerContainerSize(Size(400, 470));
		this->CreateAnimation("Jalapeno", "Jalapeno_Normal", "", Vec2(200, 610), 1.5f);
		this->CreateAnimationText(0, global->GameText.find("FIREPEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->CreateAnimationText(2, global->GameText.find("FIREPEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->CreateAnimationText(3, global->GameText.find("FIREPEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->CreateAnimationText(1, global->GameText.find("FIREPEASHOOTER_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	default:
	    break;
	}
	SeedChooser->addChild(PlantsAnimation);
}

void SelectPlantsGameReadyScene::CreateAnimation(const std::string& filepath, const std::string& AnimationName,const std::string& skin, Vec2& vec2, const float& scale)
{ 
	/* 创建动画 */
	auto iter = global->GlobalAnimationData.find(filepath);
	if (iter != global->GlobalAnimationData.end())/* 如果可以找到 */
	{
		PlantsAnimation = SkeletonAnimation::createWithData(iter->second);
		PlantsAnimation->setPosition(vec2);
		PlantsAnimation->setScale(scale);
		PlantsAnimation->setSkin(skin);
		PlantsAnimation->setAnimation(0, AnimationName, true);
		PlantsAnimation->setName("PlantsAnimation");
	}
}

void SelectPlantsGameReadyScene::CreateAnimationText(const int &ID,const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color, bool AnchorPoint)
{
	AnimationText[ID]->setString(name);
	AnimationText[ID]->setFontSize(fontsize);;
	AnimationText[ID]->setColor(color);
	PlantCardTextScrollView->scrollToTop(0.1f, true);
	AnchorPoint ? AnimationText[ID]->setAnchorPoint(Vec2(0.5f, 0.5f)) : AnimationText[ID]->setAnchorPoint(Vec2(0, 1));
	switch (ID)
	{
	case 0:
		AnimationText[ID]->setPosition(vec2);
		break;
	default:
		AnimationText[ID]->setPosition(Vec2(vec2.x - PlantCardTextScrollView->getInnerContainerSize().width + 40, vec2.y + PlantCardTextScrollView->getInnerContainerSize().height - 1000));
		break;
	}
}
