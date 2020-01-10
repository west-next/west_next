#include "Request.h"
#include "GameType.h"

GameType* GameType::_instance = nullptr;

GameType* GameType::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)GameType;
	}
	return _instance;
}

void GameType::Initialization()
{
	GameType::getInstance()->SunNumberRequriement.AllSunNumbers = 0;
	GameType::getInstance()->SunNumberRequriement.IsHaveSunNumberRequriement = false;
	GameType::getInstance()->SunNumberRequriement.IsUpdateImage = false;

	GameType::getInstance()->PlantsRequriement.IsHavePlantsRequriement = false;
	GameType::getInstance()->PlantsRequriement.SurPlusPlantsNumbers = -1;
	GameType::getInstance()->PlantsRequriement.UserPlantsNumbers = -1;
}

void GameType::CreateGameType(Node* UiLayer)
{
	for (unsigned int i = 0; i <OpenLevelData::getInstance()->LevelData.GameType.size(); i++)
	{
		switch (OpenLevelData::getInstance()->LevelData.GameType.at(i))
		{
		case OpenLevelData::GameTypes::AtLeastSunNumbers:
		{
			auto ShowNumber = Sprite::create(global->ImagesPath.find("ShowNumber")->second);
			ShowNumber->setPosition(Vec2(735, 1010));
			UiLayer->addChild(ShowNumber);

			auto LackSun = Sprite::create(global->ImagesPath.find("LackSun")->second);
			LackSun->setPosition(Vec2(640, 1010));
			LackSun->setName("LackSun");
			UiLayer->addChild(LackSun);

			GameType::getInstance()->SunNumberRequriement.IsHaveSunNumberRequriement = true;
			ShowNumbers(UiLayer, 1);
		}
			break;
		case OpenLevelData::GameTypes::UserPlantsNumbers:
		{
			auto ShowNumber = Sprite::create(global->ImagesPath.find("ShowNumber")->second);
			ShowNumber->setPosition(Vec2(735, 1010));
			UiLayer->addChild(ShowNumber);

			auto PlantsNumbers = Sprite::create(global->ImagesPath.find("MorePlants")->second);
			PlantsNumbers->setPosition(Vec2(640, 1010));
			PlantsNumbers->setName("MorePlants");
			UiLayer->addChild(PlantsNumbers);

			GameType::getInstance()->PlantsRequriement.IsHavePlantsRequriement = true;
			ShowNumbers(UiLayer, 2);
		}
		    break;
		case OpenLevelData::GameTypes::FlowerPosition:
		{
			auto Flowers = Sprite::create(global->ImagesPath.find("Flowers")->second);
			Flowers->setPosition(Vec2(OpenLevelData::getInstance()->LevelData.FlowerPosition, 600));
			Flowers->setScaleY(1.2f);
			UiLayer->addChild(Flowers);
			auto Flowers1 = Sprite::create(global->ImagesPath.find("Flowers")->second);
			Flowers1->setPosition(Vec2(OpenLevelData::getInstance()->LevelData.FlowerPosition, 300));
			Flowers1->setScaleY(1.2f);
			UiLayer->addChild(Flowers1);
		}
			break;
		case OpenLevelData::GameTypes::NoPlants:
		{
			for (auto sp : OpenLevelData::getInstance()->LevelData.NoPlantsPosition)
			{
				auto Plants = SkeletonAnimation::createWithData(global->GlobalAnimationData.find("Impurity")->second);
				Plants->setPosition(Vec2(570 + 122 * sp.x + 60, 110 + 138 * (sp.y + 1) - 90));
				Plants->setAnimation(0, "animation", true);
				UiLayer->addChild(Plants);
			}
		}
			break;
		default:
			break;
		}
	}
}

void GameType::UpdateRequirementNumbers(Node* node, const std::string name)
{
	if (!strcmp(name.c_str(), "植物数量增加"))
	{
		/* 如果有植物数量要求，则自加一 */
		if (GameType::getInstance()->PlantsRequriement.IsHavePlantsRequriement)
		{
			GameType::getInstance()->PlantsRequriement.SurPlusPlantsNumbers++;
			UpdateNumbers(node, 2);
		}
	}
	else if(!strcmp(name.c_str(), "植物数量减少"))
	{
		/* 如果有植物数量要求，则自减一 */
		if (GameType::getInstance()->PlantsRequriement.IsHavePlantsRequriement)
		{
			GameType::getInstance()->PlantsRequriement.SurPlusPlantsNumbers--;
			UpdateNumbers(node, 2);
		}
	}
	else if (!strcmp(name.c_str(), "阳光数量增加"))
	{
		if (GameType::getInstance()->SunNumberRequriement.IsHaveSunNumberRequriement)
		{
			GameType::getInstance()->SunNumberRequriement.AllSunNumbers += 50;
			UpdateNumbers(node, 1);
		}
	}
	else if(!strcmp(name.c_str(), "阳光数量减少"))
	{
		if (GameType::getInstance()->SunNumberRequriement.IsHaveSunNumberRequriement)
		{
			GameType::getInstance()->SunNumberRequriement.AllSunNumbers -= 50;
			UpdateNumbers(node, 1);
		}
	}
}

void GameType::ShowNumbers(Node* node, const int& ID)
{
	switch(ID)
	{
	case 1:
	{
		SunNumberRequriement.AllSunNumbersText = ui::Text::create();
		SunNumberRequriement.AllSunNumbersText->setFontSize(30);
		SunNumberRequriement.AllSunNumbersText->setFontName("resources/fonts/arial.ttf");
		SunNumberRequriement.AllSunNumbersText->setPosition(Vec2(737, 1012));
		SunNumberRequriement.AllSunNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
		SunNumberRequriement.AllSunNumbersText->setColor(Color3B::YELLOW);
		SunNumberRequriement.AtLeastSunNumbers = OpenLevelData::getInstance()->LevelData.AtLeastSunNumbers;
		UpdateNumbers(node, ID);
		node->addChild(SunNumberRequriement.AllSunNumbersText);
	}
		break;
	case 2:
	{
		PlantsRequriement.PalntsNumbersText= ui::Text::create();
		PlantsRequriement.PalntsNumbersText->setFontSize(30);
		PlantsRequriement.PalntsNumbersText->setFontName("resources/fonts/arial.ttf");
		PlantsRequriement.PalntsNumbersText->setPosition(Vec2(737, 1012));
		PlantsRequriement.PalntsNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
		PlantsRequriement.PalntsNumbersText->setColor(Color3B::YELLOW);
		PlantsRequriement.SurPlusPlantsNumbers = OpenLevelData::getInstance()->LevelData.UsePlantsNumbers;
		PlantsRequriement.UserPlantsNumbers = OpenLevelData::getInstance()->LevelData.UsePlantsNumbers;
		UpdateNumbers(node, ID);
		node->addChild(PlantsRequriement.PalntsNumbersText);
	}
		break;
	default:
		break;
	}
	
}

void GameType::UpdateNumbers(Node* node, const int& ID)
{
	char numbers[40];
	switch (ID)
	{
	case 1:
	{
		sprintf(numbers, "%d/%d", SunNumberRequriement.AtLeastSunNumbers, SunNumberRequriement.AllSunNumbers);
		SunNumberRequriement.AllSunNumbersText->setString(numbers);

		if (!SunNumberRequriement.IsUpdateImage && SunNumberRequriement.AllSunNumbers >= OpenLevelData::getInstance()->LevelData.AtLeastSunNumbers)
		{
			auto sprite = static_cast<Sprite*>(node->getChildByName("LackSun"));
			sprite->setTexture(global->ImagesPath.find("MoreSun")->second);

			SunNumberRequriement.IsUpdateImage = true;
		}
	}
		break;
	case 2:
	{
		sprintf(numbers, "%d / %d", PlantsRequriement.UserPlantsNumbers, PlantsRequriement.SurPlusPlantsNumbers);
		PlantsRequriement.PalntsNumbersText->setString(numbers);

		if (!PlantsRequriement.IsUpdateImage && PlantsRequriement.SurPlusPlantsNumbers <= 0)
		{
			auto sprite = static_cast<Sprite*>(node->getChildByName("MorePlants"));
			sprite->setTexture(global->ImagesPath.find("LackPlants")->second);

			SunNumberRequriement.IsUpdateImage = true;
		}
	}
		break;
	default:
		break;
	}	
}

void GameType::WaringPlantsNull()
{
	auto action = TintTo::create(0.1f, Color3B::RED);
	auto action1 = TintTo::create(0.1f, Color3B::YELLOW);
	PlantsRequriement.PalntsNumbersText->runAction(Repeat::create(Sequence::create(action, action1, CallFunc::create([=]()
		{
			PlantsRequriement.PalntsNumbersText->setColor(Color3B::YELLOW);
		}), nullptr), 5));
}