#include "Request.h"
#include "GameType.h"
#include "UserLose.h"

Scene* Request::CreateScene()
{
	return Request::create();
}

bool Request::init()
{
	if (!Scene::init())
	{
		return false;
	}

	CreateBackGroundImage();

	if (OpenLevelData::getInstance()->LevelData.FlowerPosition == -10000)
	{
		/* 通过要求没有达到 */
		OpenLevelData::getInstance()->LevelData.FlowerPosition = -1;
		UserWinRequirement::getInstance()->CreateDialogBox(this, 2, 1);
	}
	else
	{
		/* 查看通过要求 */
		UserWinRequirement::getInstance()->CreateDialogBox(this, -1, 2);
	}
	
	return true;
}

void Request::JudgeZombiesWin(map<int, Global::_Zombies>::iterator Iter)
{
	auto LevelData = OpenLevelData::getInstance();

	/* 玩家闯关失败 */
	if ((Iter->second.ZombiesAnimation->getPositionX() < LevelData->LevelData.UserLose &&                    /* 僵尸进入房间 */
		Iter->second.ZombiesAnimation->isVisible())                                                          /* 僵尸可见 */
		||
		(LevelData->LevelData.FlowerPosition >= 700 &&                                                       /* 有花坛要求 */
		Iter->second.ZombiesAnimation->isVisible() &&                                                        /* 僵尸可见 */
		fabs(Iter->second.ZombiesAnimation->getPositionX() - LevelData->LevelData.FlowerPosition) <= 10))    /* 僵尸踩坏花坛 */                                            
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f); /* 恢复播放倍数 */
		Global::getInstance()->CaveScene.Temporary = nullptr;        /* 失败后不存档 */
		/* 记录闯关失败个数 */
		UserDefault::getInstance()->setIntegerForKey("BREAKTHROUGH", ++global->UserGameData.BreakThrough);
		UserDefault::getInstance()->flush();

		if (Iter->second.ZombiesAnimation->getPositionX() < LevelData->LevelData.UserLose &&                  /* 僵尸进入房间 */
			Iter->second.ZombiesAnimation->isVisible())                                                       /* 僵尸可见 */
		{
			Director::getInstance()->replaceScene(UserLose::createScene());
		}

		if (LevelData->LevelData.FlowerPosition >= 700 &&                                                     /* 有花坛要求 */
			Iter->second.ZombiesAnimation->isVisible() &&                                                     /* 僵尸可见 */
			fabs(Iter->second.ZombiesAnimation->getPositionX() - LevelData->LevelData.FlowerPosition) <= 10)  /* 僵尸踩坏花坛 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("losemusic")->second), global->GlobalMusic.SoundEffect);
			LevelData->LevelData.FlowerPosition = -10000;
			Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Request::CreateScene()));
		}	
	}
}

int Request::JudgeUserWin()
{
	for (unsigned int i = 0; i < LevelData->LevelData.GameType.size(); i++)
	{
		switch (LevelData->LevelData.GameType.at(i))
		{
		case OpenLevelData::GameTypes::AtLeastSunNumbers:
			if (LevelData->LevelData.AtLeastSunNumbers > GameType::getInstance()->SunNumberRequriement.AllSunNumbers)
			{
				return OpenLevelData::GameTypes::AtLeastSunNumbers;
			}
			break;
		case OpenLevelData::GameTypes::CarNumbers:
			if (Car::getInstance()->Cars.size() < (unsigned int)LevelData->LevelData.CarNumbers)
			{
				return OpenLevelData::GameTypes::CarNumbers;
			}
			break;
		case OpenLevelData::GameTypes::FlowerPosition:
			if (LevelData->LevelData.FlowerPosition == -10000)
			{
				return OpenLevelData::GameTypes::FlowerPosition;
			}
			break;
		default:
			break;
		}
	}
	return 100;
}
