#include "GameEasterEggs.h"

Scene* GameEasterEggs::createScene()
{
	return GameEasterEggs::create();
}

bool GameEasterEggs::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto AudioID = global->changeBgMusic("ZombiesWinEggs", false);

	/* 文字彩蛋 */
	string buff = { "      真棒！！！首先祝贺你通过了此测试版的所有关卡。感谢你能够花费时间来关注此游戏。你们的关注是我开发游戏的最大动力。正是因为你们的关注，才使得我有足够的信念使我继续坚持下去，并把游戏做的更好。\n      植物大战僵尸这款游戏是我最喜欢的游戏之一，很久以前我就希望自己能够做出这样一款同人游戏，但是那个时候能力有限，直到去年暑假（2018年）我开始制作，暑假做了大约一个多月。但是由于能力有限，制作出的游戏根本没有可玩性。最后不得不放弃了制作。但是我并没有真正的放弃，时隔一年（2019年）的暑假我从新开始制作植物大战僵尸。经过两三个月的学习与开发，终于制作出了一个具有可玩性的测试版。游戏制作是一个困难而且艰辛的过程，期间遇到过无数问题，有过放弃，也有过兴奋。最终我坚持了下来！\n      下次可能的更新有增加植物以及僵尸种类，增加商店系统以及图鉴系统，开一个新的世界，增加游戏种类，新增小游戏等等。\n       目前有六种僵尸和八种植物，植物和僵尸的动画都是本人做的，由于做动画的能力有限，有些僵尸和植物动画不能实现，如果可以做动画并且愿意帮助我的人可以私聊我（动画是用spine软件制作的骨骼动画）。今后的更新是不定时的，可能会很慢。本人qq：2117610943\n\n" };
	
	auto Eggstext = Label::createWithTTF("", "resources/fonts/GameFont.ttf", 36);
	Eggstext->setString(buff + "游戏过程中经历 "+std::to_string(global->UserGameData.BreakThrough)+" 次闯关失败！\n游戏过程中使用 "+std::to_string(global->UserGameData.UsePlantsNumbers)+" 株植物进行防御！\n游戏过程中共有 " + std::to_string(global->UserGameData.KillAllZombies) + " 个僵尸牺牲了！");
	Eggstext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, -430));
	Eggstext->setColor(Color3B(0, 255, 255));
	Eggstext->setMaxLineWidth(1700);
	this->addChild(Eggstext);

	Eggstext->runAction(Sequence::create(MoveTo::create(25.0f, Director::getInstance()->getWinSize() / 2.0f), 
		CallFunc::create([=]() 
			{
				auto text = Label::createWithTTF("静静的享受音乐播放完毕", "resources/fonts/GameFont.ttf", 36);
				text->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 100));
				text->setColor(Color3B::WHITE);
				text->setName("Text");
				this->addChild(text);

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							static int I = 0;
							if (I % 3 == 0 || I % 5 == 0 || I % 7 == 0)I++;
							else I += 2;
							auto size = Eggstext->getStringLength();
							for (int i = 0; i < size; i++)
							{
								auto letter = Eggstext->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.05f * i), Spawn::create(JumpBy::create(0.7f, Vec2(0, 0), 50, 1), RotateBy::create(0.7f, 360), Sequence::create(TintTo::create(0.35f, Color3B::RED), TintTo::create(0.35f, I % 2 == 0 ? Color3B::GREEN : Color3B(0, 255, 255))), nullptr), nullptr));
								}
							}
						}), DelayTime::create(Eggstext->getStringLength() * 0.05f / 3.0f), nullptr)));

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							auto size1 = text->getStringLength();
							for (int i = 0; i < size1; i++)
							{
								auto letter = text->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.1f * i), JumpBy::create(0.5f, Vec2(0, 0), 30, 1), nullptr));
								}
							}
						}), DelayTime::create(text->getStringLength() * 0.1f), nullptr)));

			}),nullptr));

	AudioEngine::setFinishCallback(AudioID, [=](int i,string name) 
		{
			this->getChildByName("Text")->setVisible(false);

			global->GlobalMusic.BackGroundMusic.clear();
			global->GlobalMusic.BackGroundMusic.push_back(AudioEngine::play2d(global->MusicsPath.find("mainmusic")->second, true));
			AudioEngine::setVolume(*global->GlobalMusic.BackGroundMusic.begin(), global->GlobalMusic.BackGroundMusicVolume);

			/* 创建按钮 */
			auto button = ui::Button::create(global->ImagesPath.find("SeedChooser_Button2")->second, global->ImagesPath.find("SeedChooser_Button2_Glow")->second);
			button->setTitleText(global->GameText.find("退出")->second);
			button->setTitleColor(Color3B(0, 255, 255));
			button->setTitleFontSize(18);
			button->setScale(2.0f);
			button->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 100));
			this->addChild(button);
			button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
				{
					switch (type)
					{
					case ui::Widget::TouchEventType::BEGAN:
						AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap2")->second), global->GlobalMusic.SoundEffect);
						break;
					case ui::Widget::TouchEventType::ENDED:
						Director::getInstance()->popScene();
						break;
					}
				});
		});
	return true;
}

