#include "World_1.h"
#include "MainMenu.h"
#include "SelectWorldScene.h"

Scene* SelectWorldScene::createScene()
{
	return SelectWorldScene::create();
}

bool SelectWorldScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	if (global->MainToWorld)
	{
		/* 播放音乐 */
		global->changeBgMusic("mainmusic2", true);

		auto Clocks = AudioEngine::play2d(global->MusicsPath.find("Clocks")->second);
		AudioEngine::setVolume(Clocks, global->GlobalMusic.SoundEffect);
		auto TimeTraveller = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("TimeTraveller")->second);
		TimeTraveller->setAnimation(0, "animation", true);
		TimeTraveller->setContentSize(Size(1920, 1080));
		TimeTraveller->setPosition(Vec2(700, 598));
		TimeTraveller->setName("TimeTraveller");
		TimeTraveller->setGlobalZOrder(1);
		this->addChild(TimeTraveller);

		TimeTraveller->runAction(Sequence::create(DelayTime::create(2), FadeOut::create(1.0f),
			CallFunc::create([=]()
				{
					AudioEngine::stop(Clocks);
					this->removeChildByName("TimeTraveller");
					World[0]->setEnabled(true);
				}), nullptr));
	}


	Size backgroundSize = Director::getInstance()->getWinSize();

	auto background = Sprite::create(global->ImagesPath.find("WorldBackground")->second);
	background->setPosition(backgroundSize / 2.0f);
	this->addChild(background);

	auto rotate = Sprite::create(global->ImagesPath.find("RunBackground")->second);
	rotate->setScale(3.5f);
	rotate->setPosition(backgroundSize / 2.0f);
	this->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto WorldBackgroundBlack = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	auto WorldBackgroundBlack1 = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	WorldBackgroundBlack->setPosition(Vec2(0, 1140));
	WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	WorldBackgroundBlack1->setPosition(Vec2(0, -60));
	WorldBackgroundBlack1->setFlippedY(true);
	this->addChild(WorldBackgroundBlack);
	this->addChild(WorldBackgroundBlack1);

	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setBounceEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setContentSize(Director::getInstance()->getWinSize());
	scrollView->setInnerContainerSize(Size(11000, 1080));
	scrollView->setPosition(Vec2(0,0));
	this->addChild(scrollView);

	std::string WorldImageName[] = { {"World1"},{"World2"} ,{"World3"} ,{"World4"} ,{"World5"} ,{"World6"} ,{"World7"} ,{"World8"} ,{"World9"} ,{"World10"} ,{"World11"},{"World12"} };
	std::string WorldName[] = { {"现代世界"},{"远古埃及"} ,{"黑暗时代"} ,{"海盗港湾"} ,{"狂野西部"} ,{"冰河世纪"} ,{"未来世界"} ,{"侏罗纪世界"} ,{"大浪沙滩"} ,{"魔音时代"} ,{"失落之城"},{"尽情期待"} };
	for (int i = 0; i < 12; i++)
	{
		World[i] = ui::Button::create(global->ImagesPath.find(WorldImageName[i])->second);
		World[i]->setPosition(Vec2(1000 + 800 * i, backgroundSize.height / 2.0f));
		if (global->MainToWorld) World[i]->setEnabled(false);
		scrollView->addChild(World[i]);

		auto worldname = Text::create();
		worldname->setPosition((Vec2)(World[i]->getContentSize() / 2.0f) - Vec2(0, 100));
		worldname->setFontName("resources/fonts/fzse_gbk.ttf");
		worldname->setFontSize(80);
		worldname->setColor(Color3B(0, 255, 255));
		worldname->setString(WorldName[i]);
		World[i]->addChild(worldname);

		if (i > 0)
		{
			World[i]->setEnabled(false);
			worldname->setString("尽请期待");
		}

		World[i]->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
					break;
				case ui::Widget::TouchEventType::ENDED:
					switch (i)
					{
					case 0:
						global->BackGroundImageName = Global::_BackGroundImageName::Mordern; /* 初始化背景 */
						Director::getInstance()->replaceScene(TransitionFade::create(0.5f, World_1::createScene()));
						break;
					default:
						break;
					}
					break;
				}
			});
	}

	this->CreateGoBack();
	
	return true;
}

void SelectWorldScene::CreateGoBack()
{
	auto back = ui::Button::create(global->ImagesPath.find("back")->second, global->ImagesPath.find("back1")->second);
	back->setScale(0.7f);
	back->setAnchorPoint(Vec2(0, 1));
	back->setPosition(Vec2(0, 1080));
	this->addChild(back);

	back->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				break;
			}
		});
}
