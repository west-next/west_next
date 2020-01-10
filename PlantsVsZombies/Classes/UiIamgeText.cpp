#include "Request.h"
#include "UiImageText.h"
#include "SelectPlantsGameReadyScene.h"

UiImageText* UiImageText::_instance = nullptr;

UiImageText* UiImageText::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)UiImageText;
	}
	return _instance;
}

ProgressTimer* UiImageText::CreateProgressBar(Node* UiLayer)
{
	auto ProgressBar = Sprite::create(global->ImagesPath.find("ProgressBar")->second);
	ProgressBar->setContentSize(Size(465, 40));
	ProgressBar->setName("ProgressBar");
	ProgressBar->setAnchorPoint(Vec2(1, 0.3f));
	ProgressBar->setPosition(Vec2(1300, 1005));
	UiLayer->addChild(ProgressBar);
	auto progressbar = ProgressTimer::create(Sprite::create(global->ImagesPath.find("ProgressBar1")->second));
	progressbar->setScaleX(3.0f);
	progressbar->setScaleY(2.0f);
	progressbar->setPosition(Vec2(240, 20));
	progressbar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	progressbar->setBarChangeRate(Vec2(1, 0));       //设置横向
	progressbar->setMidpoint(Vec2(1, 0));            //设置从左往右
	//progressbar->setDirection(LoadingBar::Direction::RIGHT);
	ProgressBar->addChild(progressbar, -1);
	auto progressbarhead = Sprite::create(global->ImagesPath.find("CommonZombieHead")->second);
	progressbarhead->setScale(0.9f);
	progressbarhead->setPosition(Vec2(470, 22));
	progressbarhead->setName("progressbarhead");
	ProgressBar->addChild(progressbarhead, 1);

	auto LevelData = OpenLevelData::getInstance();
	for (unsigned int i = 0; i < LevelData->LevelData.MunchZombiesFrequency.size(); i++)
	{
		char name[20] = { "" };
		auto progressbarflag = Sprite::create(global->ImagesPath.find("ProgressBarFlag")->second);
		float precent = LevelData->LevelData.MunchZombiesFrequency.at(i) / (double)LevelData->LevelData.ZombiesFrequency;
		progressbarflag->setPosition(Vec2(465 - 465 * precent, 0));
		progressbarflag->setScale(1.6f);
		sprintf(name, "progressbarflag%d", i);
		progressbarflag->setName(name);
		progressbarflag->setAnchorPoint(Vec2(0, 0));
		ProgressBar->addChild(progressbarflag);
	}
	auto progressbarflag = Sprite::create(global->ImagesPath.find("ProgressBarFlag")->second);
	progressbarflag->setPosition(Vec2(0, 0));
	progressbarflag->setScale(1.6f);
	progressbarflag->setName("_progressbarflag");
	progressbarflag->setAnchorPoint(Vec2(0, 0));
	ProgressBar->addChild(progressbarflag);

	return progressbar;
}

void UiImageText::CreateSunBank(Node* UiLayer)
{
	auto SunBank = Sprite::create(global->ImagesPath.find("SunBank")->second);
	SunBank->setScale(0.8f);
	SunBank->setAnchorPoint(Vec2(0, 1));
	SunBank->setName("SunBank");
	SunBank->setPosition(Vec2(235, 1065));
	UiLayer->addChild(SunBank);
}

void UiImageText::CreateSeedBank(Node* UiLayer)
{
	auto SeedBank = Scale9Sprite::create(global->ImagesPath.find("SeedBank")->second);
	SeedBank->setPosition(Vec2(0, 1080));
	SeedBank->setAnchorPoint(Vec2(0, 1));
	SeedBank->setCapInsets(Rect(0, 5, 87, 373));
	SeedBank->setContentSize(Size(217.5f, 980));
	UiLayer->addChild(SeedBank);
}

void UiImageText::CreateCoinbank(Node* UiLayer)
{
	auto coinbank = Sprite::create(global->ImagesPath.find("coinbank")->second);
	coinbank->setScale(0.8f);
	coinbank->setPosition(Vec2(1750, 50));
	UiLayer->addChild(coinbank);
}

void UiImageText::CreateShovelBank(Node* UiLayer, bool* IsSelectShovel)
{
	auto ShovelBank = ui::Button::create(global->ImagesPath.find("ShovelBank")->second);
	ShovelBank->setPosition(Vec2(1420, 1080));
	ShovelBank->setScale(0.6f);
	ShovelBank->setAnchorPoint(Vec2(0, 1));
	ShovelBank->setName("ShovelBank");
	UiLayer->addChild(ShovelBank);

	/* 铲子监听 */
	ShovelBank->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("shovel")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (*IsSelectShovel)
				{
				case true:
					Director::getInstance()->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);
					*IsSelectShovel = false;
					break;
				case false:
					if (global->GlobalParameter.Global_CursorNotHide == cocos2d::ui::CheckBox::EventType::UNSELECTED)
					{
						Director::getInstance()->getOpenGLView()->setCursor(global->ImagesPath.find("Shovel_hi_res")->second, Point::ANCHOR_BOTTOM_LEFT);
					}
					else
					{
						Director::getInstance()->getOpenGLView()->setCursor(global->ImagesPath.find("Shovel_hi_res_1")->second, Vec2(0.125, 0.2f));
					}
					*IsSelectShovel = true;
					break;
				}
				break;
			}
		});
}

void UiImageText::CreateText(Node* UiLayer)
{
	char buff[128];
	sprintf(buff, "第 %d 天", global->NowPlayLevels.PlayLevels);
	auto username = ui::Text::create();
	username->setString("“" + global->UserData.UserName + "”" + "的时空冒险之旅" + global->NowPlayLevels.PlayWorldName + buff);
	username->setFontName("resources/fonts/GameFont.ttf");
	username->setFontSize(30);
	username->setColor(Color3B(0, 255, 255));
	username->setGlobalZOrder(12);
	username->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 850));
	UiLayer->addChild(username);
}

void UiImageText::CreateZombiesDieBank(Node* UiLayer)
{
	auto ZombiesDie = Sprite::create(global->ImagesPath.find("ZombiesDie")->second);
	ZombiesDie->setPosition(Vec2(1500,55));
	UiLayer->addChild(ZombiesDie);

	ZombiesDieText = Text::create();
	ZombiesDieText->setPosition(Vec2(130, 30));
	ZombiesDieText->setFontName("resources/fonts/GameFont.ttf");
	ZombiesDieText->setFontSize(30);
	ZombiesDieText->setColor(Color3B(190,0,190));
	ZombiesDie->addChild(ZombiesDieText);
	UpdateZombiesDieNumbers();
}

void UiImageText::CreateControlSpeed(Node* node)
{
	auto AddSpeed = ui::Button::create(global->ImagesPath.find("SpeedButton")->second, global->ImagesPath.find("SpeedButtonDown")->second);
	AddSpeed->setPosition(Vec2(1770, 1030));
	AddSpeed->setScale(0.7f);
	AddSpeed->setName("AddSpeed");
	node->addChild(AddSpeed);
	auto SubSpeed = ui::Button::create(global->ImagesPath.find("SpeedButton")->second, global->ImagesPath.find("SpeedButtonDown")->second);
	SubSpeed->setPosition(Vec2(1670, 1030));
	SubSpeed->setScale(0.7f);
	SubSpeed->setFlippedX(true);
	SubSpeed->setName("SubSpeed");
	node->addChild(SubSpeed);

	AddSpeed->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				if (Director::getInstance()->getScheduler()->getTimeScale() == 1.5f)
				{
					Director::getInstance()->getScheduler()->setTimeScale(1.0f);
					AddSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
				}
				else
				{
					Director::getInstance()->getScheduler()->setTimeScale(1.5f);
					SubSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
					AddSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButtonDown")->second);
				}
				break;
			}
		}); 
	SubSpeed->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				if (Director::getInstance()->getScheduler()->getTimeScale() == 0.5f)
				{
					Director::getInstance()->getScheduler()->setTimeScale(1.0f);
					SubSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
				}
				else
				{
					Director::getInstance()->getScheduler()->setTimeScale(0.5f);
					AddSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButton")->second);
					SubSpeed->loadTextureNormal(global->ImagesPath.find("SpeedButtonDown")->second);
				}
				break;
			}
		});

	/* 如果有要求则显示通过要求 */
	if (OpenLevelData::getInstance()->LevelData.GameType.size())
	{
		auto Requirement = ui::Button::create(global->ImagesPath.find("Requirement")->second, global->ImagesPath.find("RequirementDown")->second);
		Requirement->setPosition(Vec2(1570, 1030));
		Requirement->setScale(0.7f);
		node->addChild(Requirement);

		Requirement->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
					break;
				case ui::Widget::TouchEventType::ENDED:
					Director::getInstance()->pushScene(Request::CreateScene());
					break;
				}
			});
	}
}

void UiImageText::UpdateZombiesDieNumbers()
{
	char buff[128];
	sprintf(buff, "%d", global->UserGameData.KillAllZombies);
	ZombiesDieText->setString(buff);
}

void UiImageText::CreateCar(Node* node)
{
	const int carpositions[5] = { 180,318,456,594,732 };
	for (int i = 0; i < 5; i++)
	{
		node->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
			{
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("plastichit2")->second), global->GlobalMusic.SoundEffect);
				Car::getInstance()->CreateCarAnimation(node, Vec2(455, carpositions[i]));
			}), nullptr));
	}
	Car::getInstance()->CreateCarListener(node);
}

void UiImageText::CreatePromptText(Node* node, const string& PromptWords)
{
	auto PromptText = Sprite::create(global->ImagesPath.find("PromptText")->second);
	PromptText->setColor(Color3B::BLACK);
	PromptText->setOpacity(150);
	PromptText->setScaleX(1.3f);
	PromptText->setGlobalZOrder(100);
	PromptText->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 300));
	PromptText->setName("PromptText");
	node->addChild(PromptText);

	auto requiretext = Label::createWithTTF(PromptWords, "resources/fonts/GameFont.ttf", 40);
	requiretext->setColor(Color3B::WHITE);
	requiretext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 300));
	requiretext->setGlobalZOrder(100);
	requiretext->setName("requiretext");
	node->addChild(requiretext);

	PromptText->runAction(Sequence::create(DelayTime::create(4.0f),FadeOut::create(1.0f), nullptr));
	requiretext->runAction(Sequence::create(DelayTime::create(4.0f), FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				node->removeChildByName("PromptText");
				node->removeChildByName("requiretext");
			}), nullptr));
}

void UiImageText::ShowIsResume(Node* node)
{
	auto LevelObjiectives = ui::Scale9Sprite::create(global->ImagesPath.find("LevelObjiectives")->second);
	LevelObjiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	LevelObjiectives->setGlobalZOrder(30);
	node->addChild(LevelObjiectives);

	auto LevelObjiectivesText = ui::Text::create();
	LevelObjiectivesText->setString(global->GameText.find("存档")->second);
	LevelObjiectivesText->setFontName("resources/fonts/fzse_gbk.ttf");
	LevelObjiectivesText->setFontSize(50);
	LevelObjiectivesText->setColor(Color3B(0, 255, 255));
	LevelObjiectivesText->setPosition(Vec2(LevelObjiectives->getContentSize().width / 2, 490));
	LevelObjiectivesText->setGlobalZOrder(30);
	LevelObjiectives->addChild(LevelObjiectivesText);

	auto LevelObjiectivesText1 = ui::Text::create();
	LevelObjiectivesText1->setString(global->GameText.find("已有先前存档，是否继续？")->second);
	LevelObjiectivesText1->setFontName("resources/fonts/fzse_gbk.ttf");
	LevelObjiectivesText1->setFontSize(40);
	LevelObjiectivesText1->setColor(Color3B::BLACK);
	LevelObjiectivesText1->setPosition(Vec2(LevelObjiectives->getContentSize().width / 2, 270));
	LevelObjiectivesText1->setGlobalZOrder(30);
	LevelObjiectives->addChild(LevelObjiectivesText1);

	auto ContinuesGame = ui::Button::create(global->ImagesPath.find("Continue1")->second, global->ImagesPath.find("Continue")->second);
	ContinuesGame->setTitleText(global->GameText.find("是")->second);
	ContinuesGame->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	ContinuesGame->setTitleFontSize(30);
	ContinuesGame->setTitleColor(Color3B::YELLOW);
	ContinuesGame->setPosition(Vec2(LevelObjiectives->getContentSize().width / 2 - 200, 20));
	ContinuesGame->setGlobalZOrder(30);
	LevelObjiectives->addChild(ContinuesGame);

	auto ContinuesGame1 = ui::Button::create(global->ImagesPath.find("Continue1")->second, global->ImagesPath.find("Continue")->second);
	ContinuesGame1->setTitleText(global->GameText.find("否")->second);
	ContinuesGame1->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	ContinuesGame1->setTitleFontSize(30);
	ContinuesGame1->setTitleColor(Color3B::YELLOW);
	ContinuesGame1->setPosition(Vec2(LevelObjiectives->getContentSize().width / 2 + 200, 20));
	ContinuesGame1->setGlobalZOrder(30);
	LevelObjiectives->addChild(ContinuesGame1);

	ContinuesGame->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				if (global->CaveScene.Temporary != nullptr)
				{
					Director::getInstance()->replaceScene(global->CaveScene.Temporary);
				}
				break;
			}
		});
	ContinuesGame1->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(SelectPlantsGameReadyScene::createScene());
				break;
			}
		});

	/* 创建触摸监听 */
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* t, Event* e)
	{
		if (LevelObjiectives->getBoundingBox().containsPoint(t->getLocation()))
		{
			PhasePosition = t->getLocation() - LevelObjiectives->getPosition();
			return true;
		}
		else return false;
	};
	listener->onTouchMoved = [=](Touch* t, Event* e)
	{
		LevelObjiectives->setPosition(t->getLocation() - PhasePosition);
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, LevelObjiectives);
}
