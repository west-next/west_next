#include "BackGroundLayer.h"

void BackGroundLayer::addScene(Node* node)
{
	/* 加入到场景中 */
	node->addChild(this, 0, "BackGroundLayer");
}


bool BackGroundLayer::init()
{
	if (!Layer::init()) return false;

	/* 创建背景 */
	this->CreateBackGroundEffect(this);

	return true;
}

Sprite* BackGroundLayer::CreateBackGroundEffect(Node* node, const int& ID)
{
	switch (global->BackGroundImageName)
	{
	case Global::_BackGroundImageName::Mordern:
		switch (global->NowPlayLevels.PlayLevels)
		{
		case 35:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background6")->second);
			global->NowPlayLevels.background->addChild(ParticleSnow::create());
			break;
		case 36:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background1")->second);
			break;
		case 37:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background4")->second);
			break;
		case 50:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background5")->second);
			break;
		case 51:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background3")->second);
			break;
		case 52:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background2")->second);
			break;
		default:
			global->NowPlayLevels.background = Sprite::create(global->ImagesPath.find("background")->second);
			break;
		}
		if (!ID)
		{
			//CreateLightWallEffect(global->NowPlayLevels.background);
		}
		break;
	case Global::_BackGroundImageName::DarkAges:
		CreateFireEffect(global->NowPlayLevels.background);
		break;
	case Global::_BackGroundImageName::NeonMixtapeTour:
		break;
	case Global::_BackGroundImageName::FrostibiteCaves:
		break;
	default:
		break;
	}

	if (global->NowPlayLevels.background != nullptr)
	{
		global->NowPlayLevels.background->setAnchorPoint(Point(0, 0));
		global->NowPlayLevels.background->setContentSize(Size(2930, 1081));
		ID == 0 ? global->NowPlayLevels.background->setPosition(Vec2(-220, 0)) : global->NowPlayLevels.background->setPosition(Vec2(0, 0));
		if (node != nullptr)
		{
			node->addChild(global->NowPlayLevels.background);
		}
		else
		{
			return global->NowPlayLevels.background;
		}
	}
	return nullptr;
}

void BackGroundLayer::CreateFireEffect(Sprite* name)
{
	auto fire = ParticleFire::create();
	fire->setPosition(Vec2(1900, 30));
	name->addChild(fire);

	auto fire1 = ParticleFire::create();
	fire1->setPosition(Vec2(1820, 1000));
	name->addChild(fire1);
}

void BackGroundLayer::CreateLightWallEffect(Node* name)
{
	for (int i = 0; i < 7; i++)
	{
		auto LightWall = Sprite::create(global->ImagesPath.find("LightWall")->second);
		LightWall->setPosition(Vec2(1920, 840 - 100 * i));
		LightWall->setOpacity(150);
		LightWall->setGlobalZOrder(12);
		/*LightWall->runAction(RepeatForever::create(Spawn::create(
			Sequence::create(FadeTo::create(1, 0), FadeTo::create(1 - rand() % 10 / 10.0f, 100), DelayTime::create(2.0f), nullptr), nullptr
		)));*/
		name->addChild(LightWall);
	}
}
