#include "Plants.h"
#include "Zombies.h"

Plants* Plant = new Plants();

Sprite* Plants::CreateImage(const std::string& name, const Vec2& position, Node* node)
{
	auto PlantsImage = Sprite::create(global->ImagesPath.find(name)->second);
	PlantsImage->setPosition(position);
	PlantsImage->setName("Preview");
	PlantsImage->setOpacity(150);
	node->addChild(PlantsImage, 99);
	return PlantsImage;
}

void Plants::SetImagePosition(Node* node,const Vec2& vec2)
{
	static_cast<Sprite*>(node->getChildByName("Preview"))->setPosition(vec2);
}

SkeletonAnimation* Plants::CreateAnimation(const std::string& name, const std::string& name1, const Vec2& position, const float& scale, const float& scale1, bool loop)
{
	auto iter = global->GlobalAnimationData.find(name);
	if (iter != global->GlobalAnimationData.end())/* 如果可以找到 */
	{
		auto Plants = SkeletonAnimation::createWithData(iter->second);
		Plants->setPosition(position);
		Plants->setAnimation(0, name1, loop);
		Plants->setGlobalZOrder(SetZOrder(position.y));

		/* 创建种植泥土飞溅动画 */
		auto sprite = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("SplashOfSoil")->second);
		sprite->setPosition(Vec2(0, -50));
		sprite->setAnimation(0, "SplashOfSoil", false);
		sprite->setScale(scale);
		sprite->setGlobalZOrder(Plants->getGlobalZOrder());
		Plants->addChild(sprite);

		/* 创建植物影子 */
		auto plantshadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
		plantshadow->setScale(scale1);
		plantshadow->setPosition(Vec2(-5, 15));
		plantshadow->setName("plantshadow");
		Plants->addChild(plantshadow, -1);

		return Plants;
	}
	return nullptr;
}

void Plants::ZombiesEatPlants(map<int, Global::_Zombies>::iterator Iter, float* plantbloodvolum, SkeletonAnimation* plantanimation)
{
	if (Iter->second.BloodVolume > 0 && !Iter->second.IsEat) /* 僵尸血量大于零并且还没有播放吃植物动画 */
	{
		const string eateffect[3] = { "chomp","chomp2","chompsoft" };
		auto zombies = Iter;
		Iter->second.Speed = 0;
		Iter->second.IsEat = true;
		Iter->second.ZombiesAnimation->setMix(Zombie::getInstance()->ZombiesTypeAniamtion(Iter->second.ZombiesType), "Zombies_Eat", 1.5f);
		Iter->second.ZombiesAnimation->addAnimation(1, "Zombies_Eat", true);
		Iter->second.ZombiesAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
			{
				if (!strcmp(event->data->name, "eat"))
				{
					if (event->intValue == 1)
					{
						*plantbloodvolum -= 100;
						PlaySoundEffect(eateffect[rand() % 3]);
					}
					/*if (event->intValue == 2)
					{
						if (*plantbloodvolum >= 500)
						{
							plantanimation->runAction(Sequence::create(TintTo::create(0.125f, Color3B(100, 50, 100)), TintTo::create(0.125f, Color3B::WHITE), nullptr));
						}
					}*/
				}
			});
	}
}

float Plants::SetZOrder(const float& pos_y)
{
	const int pos[5] = { 675,537,399,261,123 };
	for (int i = 0; i < 5; i++)
		if (pos[i] == pos_y)
			return i * 2 + 1;
	return 0;
}

void Plants::PlaySoundEffect(const std::string& MusicName)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find(MusicName)->second), global->GlobalMusic.SoundEffect);
}

