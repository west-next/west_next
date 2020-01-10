#include "MainMenu.h"
#include "GlobalVariable.h"

using namespace cocos2d;

Scene* MainMenu::createScene()
{
	return MainMenu::create();
}

bool MainMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}

	/* 播放音乐 */
	global->changeBgMusic("mainmusic", true);

	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("roll_in")->second), global->GlobalMusic.SoundEffect);
	
	/* 创建主要精灵 */
	this->CreateMainSprite();

	/* 创建花朵 */
	this->CreateFlowers();

	/* 创建动画 */
	this->CreateAnimation();

	/* 创建鼠标监听 */
	this->CreateMouseListener();

	/* 创建按钮 */
	this->CreateMainButton();

	/* 定时器 */
	this->schedule(schedule_selector(MainMenu::CurUpdate), 0.05f);

	return true;
}

void MainMenu::CurUpdate(float time)
{
	/* 鼠标移动到按钮上播放音乐 */
	this->PlayMusicBleepInMainBuutons(0, Vec2(cur.x - 606, cur.y));
	this->PlayMusicBleepInMainBuutons(1, Vec2(cur.x - 606, cur.y));
	this->PlayMusicBleepInMainBuutons(2, Vec2(cur.x - 606, cur.y));
	this->PlayMusicBleepInMainBuutons(3, cur);
	
	this->PlayMusicBleepInGameButtons(1);
	this->PlayMusicBleepInGameButtons(2);
	this->PlayMusicBleepInGameButtons(3);
	this->PlayMusicBleepInGameButtons(4);

	/* 鼠标移动到按钮式更换图片 */
	MenuItem[0]->getBoundingBox().containsPoint(Vec2(cur.x - 606, cur.y)) ? 
		MenuItem[0]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Options2")->second))) :
		MenuItem[0]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Options1")->second)));
	MenuItem[1]->getBoundingBox().containsPoint(Vec2(cur.x - 606, cur.y)) ? 
		MenuItem[1]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Help2")->second))) :
		MenuItem[1]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Help1")->second)));
	MenuItem[2]->getBoundingBox().containsPoint(Vec2(cur.x - 606, cur.y)) ? 
		MenuItem[2]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Quit2")->second))) :
		MenuItem[2]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Quit1")->second)));
	MenuItem[3]->getBoundingBox().containsPoint(cur) ? 
		MenuItem[3]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_WoodSign2_press")->second))) :
		MenuItem[3]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_WoodSign2")->second)));
	
	if (global->UserData.IsUpdate)/* 如果名字更新 */
	{
		UserText->setString(global->UserData.UserName);
		this->scheduleOnce(schedule_selector(MainMenu::UpdateUserNameOnce), 0);
		global->UserData.IsUpdate = false;
	}
}

void MainMenu::UpdateUserNameOnce(float Time)
{
	/* 如果有孩子 */
	if (UserNameActionParent)
	{
		for (int i = 0; i < 10; i++)
		{
			UserNameActionParent->removeChildByTag(1000);
		}
	}

	srand(time(nullptr));
	/* 获取名字长度 */
	auto const len = UserText->getStringLength();
	for (int i = 0; i < len; i++)
	{
		auto UserNameAction = UserText->getLetter(i);
		if (UserNameAction && len > 3)
		{
			UserNameAction->setTag(1000);
			UserNameActionParent = UserNameAction->getParent();
			UserNameAction->setColor(Color3B(rand() % 256, rand() % 256, rand() % 256));
			UserNameAction->runAction(
				RepeatForever::create(
					Sequence::create(
						DelayTime::create(1 * ((i + 1) % (len - 3))), Spawn::create(RotateBy::create(1.0f, 360), JumpBy::create(1.0f, Vec2(0, 0), 30, 1), NULL), DelayTime::create(1.5f), NULL)));
		}
	}
}

void MainMenu::PlayMusicBleepInGameButtons(int ID)
{
	/* 如果鼠标在按钮上 */
	if (this->CheckCurInButtons() == ID)
	{
		MainButton[ID]->setColor(Color3B::WHITE);
		/* 如果没有播放音乐 */
		if (!PlayMusic[ID - 1])
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("bleep")->second), global->GlobalMusic.SoundEffect);
			PlayMusic[ID - 1] = true;
		}
	}
	else
	{
		if (PlayMusic[ID - 1])  PlayMusic[ID - 1] = false;
		MainButton[ID]->setColor(Color3B(150, 150, 150));
	}
}

void MainMenu::PlayMusicBleepInMainBuutons(int ID,const Vec2 &vec2)
{
	if (MenuItem[ID]->getBoundingBox().containsPoint(vec2))
	{
		/* 判断按钮编号 */
		switch (ID)
		{
		case 0:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Options2")->second)));
			break;
		case 1:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Help2")->second)));
			break;
		case 2:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Quit2")->second)));
			break;
		case 3:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_WoodSign2_press")->second)));
			break;
		}
		/* 如果没有播放音乐 */
		if (!PlayMusic[ID + 4])
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("bleep")->second), global->GlobalMusic.SoundEffect);
			PlayMusic[ID + 4] = true;
		}
	}
	else
	{
		if (PlayMusic[ID + 4]) PlayMusic[ID + 4] = false;
		switch (ID)
		{
		case 0:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Options1")->second)));
			break;
		case 1:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Help1")->second)));
			break;
		case 2:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Quit1")->second)));
			break;
		case 3:
			MenuItem[ID]->setNormalImage(Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_WoodSign2")->second)));
			break;
		}
	}
}

short MainMenu::CheckCurInButtons()
{
	/* 检测鼠标的位置是否在按钮上 */
	if ((cur.x >= 1210 && cur.x <= 1800 && cur.y >= 800 && cur.y <= 900) || (cur.x >= 1210 && cur.x <= 1600 && cur.y >= 900 && cur.y <= 930) ||
		(cur.x >= 1580 && cur.x <= 1790 && cur.y >= 730 && cur.y <= 800) || (cur.x >= 1390 && cur.x <= 1580 && cur.y >= 745 && cur.y <= 800))
	{
		return 1;
	}
	else if ((cur.x >= 1210 && cur.x <= 1770 && cur.y >= 630 && cur.y <= 700) || (cur.x >= 1210 && cur.x <= 1500 && cur.y >= 700 && cur.y <= 730) ||
		(cur.x >= 1210 && cur.x <= 1400 && cur.y >= 730 && cur.y <= 740) || (cur.x >= 1210 && cur.x <= 1350 && cur.y >= 740 && cur.y <= 760) ||
		(cur.x >= 1500 && cur.x <= 1600 && cur.y >= 700 && cur.y <= 720) || (cur.x >= 1350 && cur.x <= 1760 && cur.y >= 610 && cur.y <= 630) ||
		(cur.x >= 1450 && cur.x <= 1750 && cur.y >= 590 && cur.y <= 610) || (cur.x >= 1550 && cur.x <= 1750 && cur.y >= 570 && cur.y <= 590) ||
		(cur.x >= 1660 && cur.x <= 1750 && cur.y >= 550 && cur.y <= 570))
	{
		return 2;
	}
	else if ((cur.x >= 1220 && cur.x <= 1730 && cur.y >= 500 && cur.y <= 535) || (cur.x >= 1220 && cur.x <= 1600 && cur.y >= 535 && cur.y <= 560) ||
		(cur.x >= 1220 && cur.x <= 1450 && cur.y >= 560 && cur.y <= 590) || (cur.x >= 1220 && cur.x <= 1350 && cur.y >= 590 && cur.y <= 610) ||
		(cur.x >= 1350 && cur.x <= 1730 && cur.y >= 480 && cur.y <= 500) || (cur.x >= 1430 && cur.x <= 1720 && cur.y >= 460 && cur.y <= 480) ||
		(cur.x >= 1520 && cur.x <= 1720 && cur.y >= 440 && cur.y <= 460) || (cur.x >= 1590 && cur.x <= 1710 && cur.y >= 420 && cur.y <= 440))
	{
		return 3;
	}
	else if ((cur.x >= 1230 && cur.x <= 1300 && cur.y >= 380 && cur.y <= 485) || (cur.x >= 1300 && cur.x <= 1380 && cur.y >= 360 && cur.y <= 470) ||
		(cur.x >= 1380 && cur.x <= 1400 && cur.y >= 350 && cur.y <= 460) || (cur.x >= 1400 && cur.x <= 1500 && cur.y >= 330 && cur.y <= 440) ||
		(cur.x >= 1500 && cur.x <= 1550 && cur.y >= 310 && cur.y <= 430) || (cur.x >= 1550 && cur.x <= 1650 && cur.y >= 290 && cur.y <= 410) ||
		(cur.x >= 1650 && cur.x <= 1690 && cur.y >= 280 && cur.y <= 400))
	{
		return 4;
	}
	else
	{
		MainButton[1]->setPosition(Vec2(900, 830));
		MainButton[2]->setPosition(Vec2(880, 650));
		MainButton[3]->setPosition(Vec2(870, 510));
		MainButton[4]->setPosition(Vec2(850, 385));
		return 0;
	}
		
}

void MainMenu::CreateParticle()
{
	/* 创建烟雾 */
	this->CreateSmoke(0.3f, Vec2(87, 450));
	this->CreateSmoke(0.8f, Vec2(350, 690));
	this->CreateSmoke(0.1f, Vec2(15, 340));
	
	/* 创建雨 */
	auto Rain = ParticleRain::create();
	Rain->setEmissionRate(100);
	Rain->setStartSizeVar(5);
	Rain->setEndSizeVar(5);
	Rain->setLifeVar(20);
	Rain->setStartColorVar(Color4F(1, 1, 1, 1));
	Rain->setEndColor(Color4F::YELLOW);
    this->addChild(Rain, 1);

	
	/*auto my = ParticleSystemQuad::create("particle_texture.plist");
	my->setPosition(Vec2(300, 400));
	this->addChild(my, 10);*/
}

void MainMenu::CreateSmoke(const float &Scale,const Vec2 &vec2)
{
	/* 创建烟雾粒子特效 */
	auto Smoke = ParticleSmoke::create();
	Smoke->setScale(Scale);
	Smoke->setPosition(vec2);
	sprite[1]->addChild(Smoke, -1);
}

void MainMenu::CreateFlowers()
{
	this->CreateFlowers(1.5f, Vec2(1230, 220), global->ImagesPath.find("SelectorScreen_Flower3")->second, 3);
	this->CreateFlowers(2.0f, Vec2(1064, 260), global->ImagesPath.find("SelectorScreen_Flower2")->second, 1);
	this->CreateFlowers(1.5f, Vec2(1130, 290), global->ImagesPath.find("SelectorScreen_Flower1")->second, 2);
}

void MainMenu::CreateFlowers(const float &Scale, const Vec2 &vec2, const std::string &fileName, const int &ID)
{
	/* 创建花朵 */
	auto Flower = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(fileName));
	Flower->setScale(Scale);
	Flower->setPosition(vec2);
	sprite[3]->addChild(Flower, 1);

	/* 创建监听 */
	auto listener = EventListenerTouchOneByOne::create();
	/* 触摸函数 */
	listener->onTouchBegan = [=](Touch *t, Event *e)
	{
		auto points = t->getLocation();
		points.x -= 606;// x轴减去父图的相对坐标
		if (Flower->getBoundingBox().containsPoint(points))
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("shoop")->second), global->GlobalMusic.SoundEffect);
			switch (ID)
			{
			case 1:
			{
				auto point = PointArray::create(18);
				Vec2 vec[] = { {1064, 260},{1044, 230},{1024, 240},{1044, 210},{1064, 220},{1044, 190},{1024, 200},{1044, 170},
							   {1064, 180},{1044, 150},{1024, 160},{1044, 130},{1064, 140},{1044, 110},{1024, 120},{1044, 90},
							   {1064, 100},{1044, 70} };
				for (int i = 0; i < 18; i++)
				{
					point->addControlPoint(vec[i]);
				}
				Flower->runAction(Spawn::create(CardinalSplineTo::create(4.0f, point, 0), RotateBy::create(4.0f, 360), NULL));
			}
			break;
			case 2:
				Flower->runAction(Spawn::create(RotateBy::create(1.0f, 180), MoveBy::create(1.0f, Vec2(300, 10)), NULL));
				break;
			case 3:
				Flower->runAction(Spawn::create(RotateBy::create(5.0f, 360), JumpBy::create(5.0f, Vec2(-1200, 400), 300, 1), ScaleTo::create(5.0f, 0), NULL));
				break;
			}
			
			listener->setEnabled(false);
		}
		return false;
	};

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Flower);
}


void MainMenu::CreateAnimation()
{
	/* 创建小精灵动画 */
	auto iter = global->GlobalAnimationData.find("powerup");
	if (iter != global->GlobalAnimationData.end())/* 如果可以找到 */
	{
		auto Littlesprite = SkeletonAnimation::createWithData(iter->second);
		Littlesprite->setPosition(Vec2(300, 270));
		Littlesprite->setAnimation(0, "animation", true);
		this->addChild(Littlesprite);
	}
	
	/* 创建叶子动画 */
	iter = global->GlobalAnimationData.find("SelectorScreen_Leaves");
	if (iter != global->GlobalAnimationData.end())
	{
		auto Leaves = SkeletonAnimation::createWithData(iter->second);
		Leaves->setAnimation(0, "Leaves_action", 1);
		Leaves->setPosition(Vec2(-270, -110));
		Leaves->setAnchorPoint(Vec2(0, 0));
		Leaves->setScale(2.7f);
		Leaves->setTimeScale(0.5f);
		sprite[3]->addChild(Leaves);
	}

	/* 创建僵尸动画 */
	iter = global->GlobalAnimationData.find("Zombies");
	if (iter != global->GlobalAnimationData.end())
	{
		auto Zombies = SkeletonAnimation::createWithData(iter->second);
		auto Zombies1 = SkeletonAnimation::createWithData(iter->second);
		Zombies->setAnimation(0, "Zombies_Stand", 1);
		Zombies1->setAnimation(0, "Zombies_Stand1", 1);
		Zombies->setPosition(Vec2(880, 400));
		Zombies1->setPosition(Vec2(720, 450));
		Zombies->setAnchorPoint(Vec2(0, 0));
		Zombies->setScale(0.6f);
		Zombies1->setScale(0.5f);
		sprite[1]->addChild(Zombies);
		sprite[1]->addChild(Zombies1);
	}

	/* 创建植物动画 */
	iter = global->GlobalAnimationData.find("PeaShooter");
	if (iter != global->GlobalAnimationData.end())
	{
		auto PeaShooter = SkeletonAnimation::createWithData(iter->second);
		PeaShooter->setAnimation(0, "PeaShooter_Normal", 1);
		PeaShooter->setPosition(Vec2(450, 330));
		PeaShooter->setAnchorPoint(Vec2(0, 0));
		PeaShooter->setScale(0.6f);
		sprite[1]->addChild(PeaShooter);
	}
}

void MainMenu::CreateMouseListener()
{
	/* 创建鼠标监听 */
	Mouse = EventListenerMouse::create();

	/* 移动监听 */
	Mouse->onMouseMove = [=](Event *event) 
	{
		cur = ((EventMouse*)event)->getLocationInView(); 
	};

	/* 鼠标按下监听 */
	Mouse->onMouseDown = [&](Event *event) 
	{
		switch (this->CheckCurInButtons())
		{
		case 1: 
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			MainButton[1]->setPosition(Vec2(902, 828)); /* 冒险模式 */
			break;
		case 2: 
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			MainButton[2]->setPosition(Vec2(882, 648)); /* 解迷模式 */
			break;
		case 3:
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			MainButton[3]->setPosition(Vec2(872, 508)); /* 玩玩小游戏 */
			break;
		case 4:
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			MainButton[4]->setPosition(Vec2(852, 383)); /* 生存模式 */
			break;
		}
	};

	/* 鼠标松开监听 */
	Mouse->onMouseUp = [&](Event* event)
	{
		switch (this->CheckCurInButtons())
		{
		case 1:
			MainButton[1]->setPosition(Vec2(900, 830)); 
			BeginAdventureGame(); /* 冒险模式 */
			break;
		case 2:
			MainButton[2]->setPosition(Vec2(880, 650)); 
			BeginSurvivalGame();  /* 解迷模式 */
			break;
		case 3:
			MainButton[3]->setPosition(Vec2(870, 510)); 
			BeginChallengesGame(); /* 玩玩小游戏 */
			break;
		case 4:
			MainButton[4]->setPosition(Vec2(850, 385)); 
			BeginVasebreakerGame(); /* 生存模式 */
			break;
		}
	};

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(Mouse, this);
}

void MainMenu::CreateMainButton()
{
	/* 创建按钮 */
	MenuItem[0] = MenuItemImage::create(global->ImagesPath.find("SelectorScreen_Options1")->second, global->ImagesPath.find("SelectorScreen_Options2")->second, CC_CALLBACK_1(MainMenu::MenuOptionCallBack, this));
	MenuItem[1] = MenuItemImage::create(global->ImagesPath.find("SelectorScreen_Help1")->second, global->ImagesPath.find("SelectorScreen_Help2")->second, CC_CALLBACK_1(MainMenu::MenuHelpCallBack, this));
	MenuItem[2] = MenuItemImage::create(global->ImagesPath.find("SelectorScreen_Quit1")->second, global->ImagesPath.find("SelectorScreen_Quit2")->second, CC_CALLBACK_1(MainMenu::MenuQuitCallBack, this));
	MenuItem[3] = MenuItemImage::create(global->ImagesPath.find("SelectorScreen_WoodSign2")->second, global->ImagesPath.find("SelectorScreen_WoodSign2_press")->second, CC_CALLBACK_1(MainMenu::MenuDataCallBack, this));

	MenuItem[0]->setPosition(Vec2(960, 175));
	MenuItem[1]->setPosition(Vec2(1080, 130));
	MenuItem[2]->setPosition(Vec2(1210, 140));
	MenuItem[3]->setPosition(Vec2(145, 1080));

	MenuItem[0]->setScale(1.5f);
	MenuItem[1]->setScale(1.5f);
	MenuItem[2]->setScale(1.5f);
	MenuItem[3]->setScale(2.0f);

	MenuItem[3]->setAnchorPoint(Vec2(0, 0));
	MenuItem[3]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.1f, Vec2(0, -400))), NULL));

	menu[0] = Menu::create(MenuItem[0], MenuItem[1], MenuItem[2], NULL);
	menu[1] = Menu::create(MenuItem[3], NULL);
	
	menu[0]->setPosition(Vec2(0, 0));
	menu[1]->setPosition(Vec2(0, 0));

	sprite[3]->addChild(menu[0]);
	this->addChild(menu[1]);
}

void MainMenu::CreateMainSprite()
{
	const char* String[] =
	{
		{(global->ImagesPath.find("SelectorScreen_BG")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG1")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG2")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG3")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG4")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG5")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_BG_Center")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_BG_Left")->second).c_str()},
		{(global->ImagesPath.find("SelectorScreen_BG_Right")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_WoodSign1")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_WoodSign3")->second).c_str()},
	};
	srand(time(nullptr));
	sprite[0] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(String[rand() % 6]));
	for (int i = 6; i < 11; i++)
	{
		sprite[i - 5] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(String[i]));
	}

	Size size[] = { {1920, 1080},{1440, 700},{785, 1140},{1314, 1008},{586, 300},{184, 80} };
	for (int i = 0; i < 6; i++)
	{
		sprite[i]->setContentSize(size[i]);
		sprite[i]->setAnchorPoint(Vec2(0, 0));
	}
    this->addChild(sprite[0], -1); //天空
	
	this->CreateClouds();

	this->CreateParticle();

	Vec2 vec2[] = { {300, -100},{0, -60},{606, -1000},{150, 1200} ,{150, 1080} };
	int number[] = { 1,2,3,4,5 };
	for (int i = 1; i < 6; i++)
	{
		sprite[i]->setPosition(vec2[i - 1]);
		i == 5 ? this->addChild(sprite[number[i - 1]], 1) : this->addChild(sprite[number[i - 1]]);
	}

	sprite[1]->runAction(MoveBy::create(0.5f, Vec2(0, 60)));
	sprite[2]->runAction(MoveBy::create(0.5f, Vec2(0, 60)));
	sprite[3]->runAction(MoveBy::create(0.5f, Vec2(0, 1000)));
	sprite[4]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.0f, Vec2(0, -400))), nullptr));
	sprite[5]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.15f, Vec2(0, -440))), nullptr));

	/* 创建用户名称 */
	UserText = Text::create();
	UserText->setFontName("resources/fonts/fzse_gbk.ttf");
	UserText->setFontSize(30);
	UserText->setTextColor(Color4B::YELLOW);
	UserText->setPosition(Vec2(300, 90));
	sprite[4]->addChild(UserText);

	/* 如果用户名为空的话 */
	global->UserData.UserName.empty()? UserText->setString(global->GameText.find("愤怒的小僵尸")->second) : UserText->setString(global->UserData.UserName);
		
	this->scheduleOnce(schedule_selector(MainMenu::UpdateUserNameOnce), 0);

	MainButton[1] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Adventure_button")->second));
	MainButton[2] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Survival_button")->second));
	MainButton[3] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Challenges_button")->second));
	MainButton[4] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Vasebreaker_button")->second));
	MainButton[5] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Shadow_Adventure")->second));
	MainButton[6] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Shadow_Survival")->second));
	MainButton[7] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Shadow_Challenge")->second));
	MainButton[8] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(global->ImagesPath.find("SelectorScreen_Shadow_ZenGarden")->second));

	Vec2 Point[] = { {0,0},{900,830} ,{880,650},{870,510},{850,385},{910,826} ,{890,644},{880,506},{864,377} };
	for (int i = 8; i > 0; i--)
	{
		MainButton[i]->setPosition(Point[i]);
		MainButton[i]->setScale(1.8f);
		sprite[3]->addChild(MainButton[i]);
	}
}

void MainMenu::CreateClouds()
{
	srand(unsigned(time(NULL)));
	const char* String[] =
	{
		{(global->ImagesPath.find("SelectorScreen_Cloud1")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud2")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_Cloud3")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud4")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_Cloud5")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud6")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_Cloud7")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud8")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_Cloud9")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud10")->second).c_str()},
	    {(global->ImagesPath.find("SelectorScreen_Cloud11")->second).c_str()},{(global->ImagesPath.find("SelectorScreen_Cloud12")->second).c_str()}
	};
	Vec2 vec2[] = { {2500,1000},{2800,900},{2200,800},{2100,950},{3000,600},{2700,700},{2600,800},{3000,550},{2000,670},{2480,780},{2150,870},{2341,813} };
	Vec2 vec2_[] = { {0,1000},{0,900},{0,800},{0,950},{0,600},{0,700},{0,800},{0,550},{0,670},{0,780},{0,870},{0,813} };
	int Rotation[] = { 20,25,35,35,30,0,0,0,0,0,-20,-20 };
	for (int i = 6; i < 18; i++)
	{
		sprite[i] = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(String[i - 6]));
		sprite[i]->setPosition(300 + rand() % 1800, vec2[i - 6].y);
		sprite[i]->setRotation(Rotation[i - 6]);
		sprite[i]->setScale(1 + (rand() % 3) / 1.2f);
		sprite[i]->runAction(RepeatForever::create(Sequence::create(MoveTo::create(60 + rand() % 60, vec2_[i - 6]), CallFuncN::create(CC_CALLBACK_1(MainMenu::SetCloudPosition, this, i, vec2[i - 6])), NULL)));
		this->addChild(sprite[i]);
	}
}

void MainMenu::SetCloudPosition(Node *node,int ID,const Vec2 &vec2)
{
	/* 初始化云 */
	sprite[ID]->setPosition(vec2);
	sprite[ID]->setScale(1 + (rand() % 3) / 1.2f);
}

void MainMenu::MenuDataCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
	Director::getInstance()->pushScene(InputDataMenu::CreateInputDataScene());
}

void MainMenu::MenuHelpCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, HelpScene::createHelpScene()));
}

void MainMenu::MenuOptionCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
	Director::getInstance()->pushScene(OptionsMenu::CreateOptionsScene());
}

void MainMenu::MenuQuitCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
	Director::getInstance()->pushScene(QuitMenu::CreateQuitScene());
}

void MainMenu::BeginAdventureGame()
{
	global->MainToWorld = true;
	Director::getInstance()->replaceScene(SelectWorldScene::createScene());
}

void MainMenu::BeginSurvivalGame()
{
	Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
}

void MainMenu::BeginChallengesGame()
{
	Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
}

void MainMenu::BeginVasebreakerGame()
{
	Application::getInstance()->openURL("https://pan.baidu.com/s/1NlpRcQTv1FrN51UgAzGiHw&shfl=sharepset#/");
}