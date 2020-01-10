#include "UserLose.h"

Scene* UserLose::createScene()
{
	return UserLose::create();
}

bool UserLose::init()
{
	if (!Scene::init()) return false;

	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("losemusic")->second), global->GlobalMusic.SoundEffect);
	auto Brain = Sprite::create(global->ImagesPath.find("Brain")->second);
	Brain->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	this->addChild(Brain);

	auto lose = Sprite::create(global->ImagesPath.find(rand() % 2 == 0 ? "zombieswon" : "zombieswon1")->second);
	lose->setScale(2.0f);
	lose->setPosition(Vec2(2500, Director::getInstance()->getWinSize().height / 2.0f));
	lose->runAction(Sequence::create(
		JumpTo::create(1.0f, Director::getInstance()->getWinSize() / 2.0f, 250, 3),
		DelayTime::create(5),
		CallFunc::create([]()
			{
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, World_1::createScene()));
			}),
		nullptr));
	this->addChild(lose);

	return true;
}
