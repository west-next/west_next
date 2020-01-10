#include "World_1.h"
#include "JudgeProFile.h"
#include "BackGroundLayer.h"
#include "SelectWorldScene.h"

Scene* World_1::createScene()
{
	return World_1::create();
}

bool World_1::init()
{
	if (!Scene::init())
	{
		return false;
	}
	srand(time(nullptr));

	/* 播放音乐 */
	global->changeBgMusic("mainmusic2", true);

	auto layer = LayerColor::create(Color4B::BLACK);
	layer->setName("BlackLayer");
	layer->setGlobalZOrder(10);
	this->addChild(layer);

	this->runAction(Sequence::create(DelayTime::create(0.5f),
		CallFunc::create([=]()
			{
				/* 读取该世界关卡数据 */
				if (!global->NowPlayLevels.IsReadWoldInformation[0])
				{
					OpenLevelData::getInstance()->OpenLevelsData(global->TextPath.find("GAMEWORLD_1DATAS")->second);
					global->NowPlayLevels.IsReadWoldInformation[0] = true;
				}
			}),
		CallFunc::create([=]()
			{
				layer->runAction(FadeOut::create(0.3f));
			}),
				DelayTime::create(1.0f),
				CallFunc::create([=]()
					{
						this->removeChildByName("BlackLayer");
					}),
				nullptr));

	global->NowLevels[1].Levels = UserDefault::getInstance()->getIntegerForKey(global->UserGameData.UserDataName[global->UserGameData.UserDataNumber].c_str());
	if (global->NowLevels[1].Levels == 0)
	{
		global->NowLevels[1].Levels = 1;
	}
	/* 读取初始化位置 */
	WorldPosition = UserDefault::getInstance()->getIntegerForKey("WORLD_1_POSITION");

	this->CreateBackground();

	this->CreateScrollView();

	this->CreateGoBack();

	return true;
}

void  World_1::CreateBackground()
{
	auto background = Sprite::create(global->ImagesPath.find("WorldBackground")->second);
	background->setPosition(backgroundSize / 2.0f);
	this->addChild(background);

	auto rotate = Sprite::create(global->ImagesPath.find("RunBackground")->second);
	rotate->setScale(3.0f);
	rotate->setPosition(backgroundSize / 2.0f);
	this->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto World1_34 = Sprite::create(global->ImagesPath.find("World1_34")->second);
	World1_34->setScale(0.6f);
	World1_34->setPosition(backgroundSize / 2.0f);
	this->addChild(World1_34);

	auto World1_33 = Sprite::create(global->ImagesPath.find("World1_33")->second);
	World1_33->setPosition(Vec2(800, 600));
	World1_34->addChild(World1_33);

	Parallax = ParallaxNode::create();

	auto WorldBackgroundBlack = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	auto WorldBackgroundBlack1 = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	WorldBackgroundBlack1->setFlippedY(true);
	Parallax->addChild(WorldBackgroundBlack, 2, Vec2(0, 0), Vec2(0, 1140));
	Parallax->addChild(WorldBackgroundBlack1, 2, Vec2(0, 0), Vec2(0, -60));

	for (int i = 0; i < 8; i++)
	{
		switch (rand() % 2)
		{
		case 0:
		{
			auto World1_28 = Sprite::create(global->ImagesPath.find("World1_28")->second);
			Parallax->addChild(World1_28, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
		}
		break;
		case 1:
		{
			auto World1_29 = Sprite::create(global->ImagesPath.find("World1_29")->second);
			Parallax->addChild(World1_29, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
		}
		break;
		}
	}
}

void World_1::CreateScrollView()
{
	scrollView = ui::ScrollView::create();
	scrollView->setBounceEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setContentSize(Director::getInstance()->getWinSize());
	scrollView->setInnerContainerSize(Size(33100, 1080));
	scrollView->setPosition(Vec2(0, 0));
	scrollView->setScrollBarAutoHideEnabled(false);
	scrollView->jumpToPercentHorizontal(WorldPosition);
	this->addChild(scrollView);

	scrollView->addChild(Parallax);

	this->AddScrollView();

}

void World_1::AddScrollView()
{
	/* 创建背景物品 */
	const string name[5] = { {"World1_32"},{"World1_31"},{"World1_36"},{"World1_30"},{"World1_27"} };
	const string name1[2] = { {"World1_39"},{"World1_40"}};

	for (int i = 0; i < 10; i++)
	{
		const auto world_32 = CreateSprite(name[i % 5], Vec2(500 + 1000 * i, i % 2 == 0 ? 850 : 350), rand() % 4/ 10.0f + 0.3f, 1, 0.2f, true);
		world_32->runAction(RepeatForever::create(RotateBy::create(10, 30)));
	}

	for (int i = 0; i < 8; i++)
	{
		const auto world_32 = CreateSprite(name1[i % 2], Vec2(500 + 2000 * i, 180), rand() % 3 / 10.0f + 0.5f, 3, 0.7f, true);
		world_32->setAnchorPoint(Vec2(0.5f, 0.5f));
		if (i % 2 != 0)
		{
			world_32->runAction(RepeatForever::create(RotateBy::create(25, 30)));
		}
	}

	/* 创建线条 */
	auto draw = DrawNode::create();
	Vec2 BeginPoint[53] =
	{
		Vec2(575, 450) ,Vec2(1050, 580) ,Vec2(1370, 730) ,Vec2(1650, 510) ,Vec2(2050, 440) ,Vec2(2300, 370) ,Vec2(2650, 520) ,Vec2(2900,740),
		Vec2(3230, 560) ,Vec2(3550, 520) ,Vec2(3920, 580) ,Vec2(4250, 750) ,Vec2(4620, 660) ,Vec2(4960, 590) ,Vec2(5250, 460) ,Vec2(5620,620),
		Vec2(5950, 665) ,Vec2(6270, 770) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 420) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8100, 790),
		Vec2(8400, 725) ,Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690),
		Vec2(10900, 620) ,Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 410) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520),
		Vec2(13300, 620) ,Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680),
		Vec2(15800, 730) ,Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16700, 390),Vec2(16900,480)
	};
	Vec2 EndPoint[53] =
	{
		Vec2(1000, 560) ,Vec2(1300, 700) ,Vec2(1600, 520) ,Vec2(2000, 440) ,Vec2(2250, 380) ,Vec2(2600, 510) ,Vec2(2900, 720) ,Vec2(3190,560),
		Vec2(3530, 520) ,Vec2(3900, 580) ,Vec2(4250, 760) ,Vec2(4600, 650) ,Vec2(4960, 590) ,Vec2(5250, 460) ,Vec2(5600, 620) ,Vec2(5950,670),
		Vec2(6270, 750) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 440) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8070, 795) ,Vec2(8400, 720),
		Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690) ,Vec2(10900, 620),
		Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 410) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520) ,Vec2(13300, 620),
		Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680) ,Vec2(15800, 730),
		Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16700, 400) ,Vec2(16900, 480),Vec2(17000,500)
	};
	for (int i = 0; i < global->NowLevels[1].Levels; i++)
	{
		draw->drawSegment(BeginPoint[i], EndPoint[i], 2, Color4F(0, 1, 1, 0.6f));
		draw->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(0.1f, 0.1f)), MoveBy::create(0.05f, Vec2(-0.1f, -0.1f)), nullptr)));
		auto LineAction = Sprite::create(global->ImagesPath.find("LineAction")->second);
		LineAction->setPosition(EndPoint[i]);
		draw->addChild(LineAction);
		LineAction->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, EndPoint[i]),
			CallFunc::create([=]()
				{
					LineAction->setPosition(BeginPoint[i]);
				}), nullptr)));
	}
	Parallax->addChild(draw, 3, Vec2(0.5f, 0), Vec2(0, 0));

	/* 创建关卡 */
	auto World1_38 = CreateSprite("World1_38", Vec2(10, backgroundSize.height / 2.0f), 0.8f, 2, 0.5f, false);
	auto sprite = CreateSprite(World1_38, "begingame", Vec2(550, 450), 1.0f, 0);

	auto wgfd = Sprite::create(global->ImagesPath.find("wgfd")->second);
	wgfd->setPosition(Vec2(80, 30));
	wgfd->setScale(0.7f);
	sprite->addChild(wgfd, -1);

	auto Level_1 = CreateSprite("World1_26", Vec2(900, 500), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_1, "World1_4", Vec2(0, 280), 0.8f, 1, true);
	CreateSprite(Level_1, "World1_6", Vec2(100, 380), 0.8f, 0, true);
	CreateButton(Level_1, "begingame", Vec2(200, 300));

	auto Level_2 = CreateSprite("World1_26", Vec2(1200, 600), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_2, "World1_10", Vec2(20, 280), 0.8f, 0, true);
	CreateSprite(Level_2, "World1_4", Vec2(0, 320), 0.8f, 1, true);
	CreateSprite(Level_2, "World1_16", Vec2(90, 500), 0.7f, 2, true);
	CreateButton(Level_2, "begingame", Vec2(180, 320));

	auto Level_3 = CreateSprite("World1_26", Vec2(1500, 450), 0.7f, 2, 0.5f, true);
	CreateSprite(Level_3, "World1_13", Vec2(20, 280), 0.9f, 0, true);
	CreateSprite(Level_3, "World1_8", Vec2(170, 360), 0.7f, 1, true);
	CreateButton(Level_3, "begingame", Vec2(180, 270));

	auto Level_4 = CreateSprite("World1_9", Vec2(1900, 350), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_4, "World1_7", Vec2(0, 230), 0.8f, 0, true);
	CreateSprite(Level_4, "World1_4", Vec2(-50, 290), 0.8f, 1, true);
	CreateSprite(Level_4, "World1_18", Vec2(-50, 520), 0.7f, 2, true);
	CreateButton(Level_4, "begingame", Vec2(150, 300));

	auto Level_5 = CreateSprite("World1_9", Vec2(2200, 300), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_5, "World1_10", Vec2(0, 250), 0.8f, 0, true);
	CreateSprite(Level_5, "World1_4", Vec2(0, 280), 0.7f, 1, true);
	CreateButton(Level_5, "begingame", Vec2(150, 300));

	auto Level_6 = CreateSprite("World1_26", Vec2(2500, 450), 0.7f, 2, 0.5f, true);
	CreateSprite(Level_6, "World1_13", Vec2(20, 280), 0.9f, 0, true);
	CreateSprite(Level_6, "World1_8", Vec2(170, 360), 0.7f, 1, true);
	CreateButton(Level_6, "begingame", Vec2(180, 280));

	auto Level_7 = CreateSprite("World1_9", Vec2(2800, 600), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_7, "World1_10", Vec2(0, 280), 0.8f, 1, true);
	CreateSprite(Level_7, "World1_13", Vec2(-20, 320), 0.8f, 2, true);
	CreateSprite(Level_7, "World1_11", Vec2(40, 400), 0.7f, 0, true);
	CreateButton(Level_7, "begingame", Vec2(150, 330));

	auto Level_8 = CreateSprite("World1_26", Vec2(3100, 500), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_8, "World1_13", Vec2(10, 280), 1.0f, 1, true);
	CreateSprite(Level_8, "World1_12", Vec2(40, 270), 0.9f, 2, true);
	CreateButton(Level_8, "begingame", Vec2(170, 300));

	auto Level_9 = CreateSprite("World1_26", Vec2(3400, 400), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_9, "World1_10", Vec2(20, 270), 0.9f, 1, true);
	CreateSprite(Level_9, "World1_7", Vec2(20, 290), 0.9f, 2, true);
	CreateSprite(Level_9, "World1_13", Vec2(-10, 320), 1.1f, 3, true);
	CreateSprite(Level_9, "World1_23", Vec2(40, 340), 0.6f, 4, true);
	CreateButton(Level_9, "begingame", Vec2(200, 330));

	auto Level_10 = CreateSprite("World1_9", Vec2(3800, 500), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_10, "World1_10", Vec2(-10, 270), 0.9f, 1, true);
	CreateSprite(Level_10, "World1_7", Vec2(0, 290), 0.9f, 2, true);
	CreateSprite(Level_10, "World1_4", Vec2(-30, 310), 0.9f, 3, true);
	CreateSprite(Level_10, "World1_8", Vec2(0, 370), 0.8f, 4, true);
	CreateButton(Level_10, "begingame", Vec2(200, 330));

	auto Level_11 = CreateSprite("World1_26", Vec2(4100, 650), 0.7f, 2, 0.5f, true);
	CreateSprite(Level_11, "World1_4", Vec2(-20, 290), 0.9f, 2, true);
	CreateSprite(Level_11, "World1_6", Vec2(70, 380), 0.8f, 1, true);
	CreateButton(Level_11, "begingame", Vec2(200, 320));

	auto Level_12 = CreateSprite("World1_9", Vec2(4500, 550), 0.65f, 2, 0.5f, true);
	CreateSprite(Level_12, "World1_10", Vec2(-20, 260), 0.9f, 1, true);
	CreateSprite(Level_12, "World1_13", Vec2(-60, 330), 1.2f, 2, true);
	CreateSprite(Level_12, "World1_12", Vec2(50, 300), 0.8f, 3, true);
	CreateButton(Level_12, "begingame", Vec2(180, 330));

	auto Level_13 = CreateSprite("World1_9", Vec2(4900, 500), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_13, "World1_10", Vec2(-20, 280), 0.9f, 1, true);
	CreateSprite(Level_13, "World1_4", Vec2(-50, 310), 0.9f, 2, true);
	CreateButton(Level_13, "begingame", Vec2(180, 330));

	auto Level_14 = CreateSprite("World1_26", Vec2(5100, 350), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_14, "World1_10", Vec2(70, 280), 0.9f, 1, true);
	CreateSprite(Level_14, "World1_13", Vec2(-30, 340), 1.2f, 2, true);
	CreateSprite(Level_14, "World1_18", Vec2(-30, 550), 0.7f, 3, true);
	CreateButton(Level_14, "begingame", Vec2(200, 330));

	auto Level_15 = CreateSprite("World1_26", Vec2(5500, 500), 0.7f, 2, 0.5f, true);
	CreateSprite(Level_15, "World1_10", Vec2(10, 280), 0.9f, 1, true);
	CreateSprite(Level_15, "World1_7", Vec2(-10, 330), 1.2f, 2, true);
	CreateSprite(Level_15, "World1_4", Vec2(-40, 360), 1.0f, 2, true);
	CreateButton(Level_15, "begingame", Vec2(200, 360));

	auto Level_16 = CreateSprite("World1_9", Vec2(5900, 600), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_16, "World1_6", Vec2(20, 340), 0.9f, 1, true);
	CreateSprite(Level_16, "World1_4", Vec2(-40, 260), 0.8f, 2, true);
	CreateButton(Level_16, "begingame", Vec2(160, 290));

	auto Level_17 = CreateSprite("World1_26", Vec2(6200, 700), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_17, "World1_23", Vec2(60, 320), 0.6f, 2, true);
	CreateSprite(Level_17, "World1_4", Vec2(0, 260), 0.8f, 1, true);
	CreateButton(Level_17, "begingame", Vec2(180, 290));

	auto Level_18 = CreateSprite("World1_9", Vec2(6500, 500), 0.55f, 2, 0.5f, true);
	CreateSprite(Level_18, "World1_10", Vec2(-20, 270), 0.9f, 1, true);
	CreateSprite(Level_18, "World1_13", Vec2(-30, 330), 1.0f, 2, true);
	CreateSprite(Level_18, "World1_15", Vec2(140, 430), 1.0f, 3, true);
	CreateButton(Level_18, "begingame", Vec2(170, 330));

	auto Level_19 = CreateSprite("World1_14", Vec2(6800, 400), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_19, "World1_5", Vec2(-50, 250), 1.2f, 1, true);
	CreateSprite(Level_19, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	CreateButton(Level_19, "begingame", Vec2(230, 300));

	auto Level_20 = CreateSprite("World1_25", Vec2(7100, 350), 0.65f, 2, 0.5f, true);
	CreateSprite(Level_20, "World1_3", Vec2(-10, 280), 1.0f, 1, true);
	CreateButton(Level_20, "begingame", Vec2(210, 310));

	auto Level_21 = CreateSprite("World1_20", Vec2(7400, 450), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_21, "World1_5", Vec2(-70, 210), 1.0f, 1, true);
	CreateSprite(Level_21, "World1_35", Vec2(30, 410), 1.0f, 2, true);
	CreateButton(Level_21, "begingame", Vec2(160, 260));

	auto Level_22 = CreateSprite("World1_14", Vec2(7700, 600), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_22, "World1_3", Vec2(-10, 280), 1.0f, 1, true);
	CreateButton(Level_22, "begingame", Vec2(180, 280));

	auto Level_23 = CreateSprite("World1_14", Vec2(8000, 750), 0.4f, 2, 0.5f, true);
	CreateSprite(Level_23, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	CreateSprite(Level_23, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	CreateButton(Level_23, "begingame", Vec2(190, 260));

	auto Level_24 = CreateSprite("World1_25", Vec2(8300, 650), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_24, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	CreateButton(Level_24, "begingame", Vec2(210, 300));

	auto Level_25 = CreateSprite("World1_14", Vec2(8600, 550), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_25, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	CreateButton(Level_25, "begingame", Vec2(190, 260));

	auto Level_26 = CreateSprite("World1_25", Vec2(8900, 450), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_26, "World1_5", Vec2(-50, 250), 1.2f, 1, true);
	CreateSprite(Level_26, "World1_15", Vec2(120, 470), 1.0f, 3, true);
	CreateButton(Level_26, "begingame", Vec2(190, 300));

	auto Level_27 = CreateSprite("World1_20", Vec2(9200, 400), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_27, "World1_3", Vec2(-70, 210), 1.0f, 1, true);
	CreateButton(Level_27, "begingame", Vec2(160, 250));

	auto Level_28 = CreateSprite("World1_14", Vec2(9500, 500), 0.4f, 2, 0.5f, true);
	CreateSprite(Level_28, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	CreateSprite(Level_28, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	CreateButton(Level_28, "begingame", Vec2(200, 260));

	auto Level_29 = CreateSprite("World1_25", Vec2(9800, 600), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_29, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	CreateSprite(Level_29, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	CreateButton(Level_29, "begingame", Vec2(230, 300));

	auto Level_30 = CreateSprite("World1_14", Vec2(10200, 700), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_30, "World1_3", Vec2(-30, 250), 1.0f, 1, true);
	CreateButton(Level_30, "begingame", Vec2(190, 260));

	auto Level_31 = CreateSprite("World1_14", Vec2(10500, 650), 0.4f, 2, 0.5f, true);
	CreateSprite(Level_31, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	CreateSprite(Level_31, "World1_17", Vec2(100, 380), 1.0f, 2, false);
	CreateButton(Level_31, "begingame", Vec2(200, 260));

	auto Level_32 = CreateSprite("World1_20", Vec2(10800, 550), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_32, "World1_3", Vec2(-70, 210), 1.0f, 1, true);
	CreateSprite(Level_32, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	CreateButton(Level_32, "begingame", Vec2(160, 250));

	auto Level_33 = CreateSprite("World1_25", Vec2(11100, 450), 0.5f, 2, 0.5f, true);
	CreateSprite(Level_33, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	CreateSprite(Level_33, "World1_21", Vec2(0, 300), 1.0f, 2, true);
	CreateSprite(Level_33, "World1_24", Vec2(170, 390), 1.0f, 3, true);
	CreateButton(Level_33, "begingame", Vec2(190, 300));

	auto Level_34 = CreateSprite("World1_14", Vec2(11400, 350), 0.4f, 2, 0.5f, true);
	CreateSprite(Level_34, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	CreateSprite(Level_34, "World1_35", Vec2(30, 430), 1.0f, 2, true);
	CreateButton(Level_34, "begingame", Vec2(200, 300));

	auto Level_35 = CreateSprite("World1_2", Vec2(11800, 500), 0.9f, 2, 0.5f, true);
	CreateButton(Level_35, "begingame", Vec2(124, 350));
	CreateButton(Level_35, "begingame", Vec2(345, 260));
	CreateButton(Level_35, "begingame", Vec2(574, 350));

	auto Level_38 = CreateSprite("World1_26", Vec2(12600, 350), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_38, "World1_5", Vec2(-20, 260), 1.0f, 1, true);
	CreateButton(Level_38, "begingame", Vec2(190, 300));

	auto Level_39 = CreateSprite("World1_26", Vec2(12900, 450), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_39, "World1_3", Vec2(0, 280), 1.0f, 1, true);
	CreateButton(Level_39, "begingame", Vec2(180, 290));

	auto Level_40 = CreateSprite("World1_9", Vec2(13200, 550), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_40, "World1_5", Vec2(-70, 250), 1.0f, 1, false);
	CreateSprite(Level_40, "World1_21", Vec2(50, 300), 1.0f, 2, true);
	CreateButton(Level_40, "begingame", Vec2(170, 290));

	auto Level_41 = CreateSprite("World1_9", Vec2(13500, 650), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_41, "World1_19", Vec2(-50, 250), 1.0f, 1, true);
	CreateButton(Level_41, "begingame", Vec2(170, 290));

	auto Level_42 = CreateSprite("World1_25", Vec2(13800, 600), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_42, "World1_4", Vec2(-30, 260), 0.9f, 1, false);
	CreateSprite(Level_42, "World1_8", Vec2(0, 370), 0.8f, 4, true);
	CreateButton(Level_42, "begingame", Vec2(170, 290));

	auto Level_43 = CreateSprite("World1_25", Vec2(14100, 500), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_43, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	CreateButton(Level_43, "begingame", Vec2(170, 280));

	auto Level_44 = CreateSprite("World1_14", Vec2(14400, 400), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_44, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	CreateButton(Level_44, "begingame", Vec2(170, 280));

	auto Level_45 = CreateSprite("World1_20", Vec2(14700, 300), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_45, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	CreateSprite(Level_45, "World1_12", Vec2(50, 300), 0.8f, 3, true);
	CreateButton(Level_45, "begingame", Vec2(170, 280));

	auto Level_46 = CreateSprite("World1_26", Vec2(15000, 450), 0.7f, 2, 0.5f, true);
	CreateSprite(Level_46, "World1_4", Vec2(-20, 290), 0.9f, 2, true);
	CreateSprite(Level_46, "World1_6", Vec2(70, 380), 0.8f, 1, true);
	CreateButton(Level_46, "begingame", Vec2(200, 320));

	auto Level_47 = CreateSprite("World1_26", Vec2(15300, 550), 0.8f, 2, 0.5f, true);
	CreateSprite(Level_47, "World1_10", Vec2(20, 270), 0.9f, 1, true);
	CreateSprite(Level_47, "World1_7", Vec2(20, 290), 0.9f, 2, true);
	CreateSprite(Level_47, "World1_13", Vec2(-10, 320), 1.1f, 3, true);
	CreateSprite(Level_47, "World1_23", Vec2(40, 340), 0.6f, 4, true);
	CreateButton(Level_47, "begingame", Vec2(200, 330));

	auto Level_48 = CreateSprite("World1_26", Vec2(15700, 650), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_48, "World1_4", Vec2(0, 280), 0.8f, 1, true);
	CreateSprite(Level_48, "World1_6", Vec2(100, 380), 0.8f, 0, true);
	CreateButton(Level_48, "begingame", Vec2(200, 300));

	auto Level_49 = CreateSprite("World1_25", Vec2(16050, 600), 0.6f, 2, 0.5f, true);
	CreateSprite(Level_49, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	CreateSprite(Level_49, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	CreateButton(Level_49, "begingame", Vec2(230, 300));

	auto Level_50 = CreateSprite("World1_2", Vec2(16400, 500), 0.9f, 2, 0.5f, true);
	CreateButton(Level_50, "begingame", Vec2(124, 350));
	CreateButton(Level_50, "begingame", Vec2(345, 260));
	CreateButton(Level_50, "begingame", Vec2(573, 350));
}

Sprite* World_1::CreateSprite(const std::string& name, const Vec2& position, const float& scale, const int& zorder, const float& speed, bool IsFlipped)
{
	auto sprite = Sprite::create(global->ImagesPath.find(name)->second);
	sprite->setAnchorPoint(Vec2(0, 0.5f));
	sprite->setScale(scale);
	sprite->setPosition(position);
	IsFlipped == true ? sprite->setFlippedX(rand() % 2) : IsFlipped;
	Parallax->addChild(sprite, zorder, Vec2(speed, 0), position);
	return sprite;
}

Sprite* World_1::CreateSprite(Node* node, const std::string& name, const Vec2& position, const float& scale, const int& zorder, bool IsFlipped)
{
	auto sprite = Sprite::create(global->ImagesPath.find(name)->second);
	sprite->setAnchorPoint(Vec2(0, 0.5f));
	sprite->setScale(scale);
	sprite->setPosition(position);
	IsFlipped == true ? sprite->setFlippedX(rand() % 2) : IsFlipped;
	node->addChild(sprite, zorder);
	return sprite;
}

void World_1::CreateButton(Node* node, const std::string& name, const Vec2& position)
{
	auto sprite4 = ui::Button::create(global->ImagesPath.find(name)->second);
	sprite4->setPosition(position);
	sprite4->setGlobalZOrder(1);
	node->addChild(sprite4);

	string buff = std::to_string(++Level);
	auto Text = Text::create();
	Text->setFontName("resources/fonts/arial.ttf");
	Text->setFontSize(60);
	Text->setString(buff);
	Text->setGlobalZOrder(1);
	Text->setTextColor(Color4B::RED);
	Text->setPosition(Vec2(80, 70));
	sprite4->addChild(Text);

	if (global->NowLevels[1].Levels < Level)
	{
		sprite4->setEnabled(false);
	}
	if (global->NowLevels[1].Levels == Level)
	{
		auto wgfd = Sprite::create(global->ImagesPath.find("wgfd")->second);
		wgfd->setPosition(Vec2(80, 30));
		wgfd->setScale(0.7f);
		wgfd->setGlobalZOrder(1);
		sprite4->addChild(wgfd, -1);

		wgfd->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2f), FadeIn::create(0.2f), nullptr)));
		sprite4->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.1f, Color3B(100, 100, 100)), TintTo::create(0.1f, Color3B(255, 255, 255)), nullptr)));
	}
	if (global->NowLevels[1].Levels > Level)
	{
		auto wgfd = Sprite::create(global->ImagesPath.find("wgfd")->second);
		wgfd->setPosition(Vec2(80, 30));
		wgfd->setScale(0.7f);
		wgfd->setGlobalZOrder(1);
		sprite4->addChild(wgfd, -1);

		sprite4->setColor(Color3B(0, 255, 255));
	}

	if (global->NowLevels[1].Levels >= 52 && !global->NowLevels[0].IsBeginShowEggs)
	{
		global->NowLevels[0].IsBeginShowEggs = true;
		UserDefault::getInstance()->setBoolForKey("ISBEGINSHOWEGGS", true);
		UserDefault::getInstance()->flush();
	}

	CreateButtonListener(sprite4, Level);
}

void World_1::CreateButtonListener(ui::Button* button, const int& ID)
{
	char LevelName[15] = {};
	sprintf(LevelName, "Level_%d", ID);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("tap")->second), global->GlobalMusic.SoundEffect);
				break;
			case ui::Widget::TouchEventType::ENDED:
				
				UserDefault::getInstance()->setFloatForKey("WORLD_1_POSITION", scrollView->getScrolledPercentHorizontal()); /* 记录位置 */
				UserDefault::getInstance()->flush();

				OpenLevelData::getInstance()->ReadLevelData((char*)LevelName);
				
				/* 如果有存档 */
				if (global->CaveScene.Temporary != nullptr && global->NowPlayLevels.PlayLevels == ID)
				{
					Director::getInstance()->replaceScene(TransitionFade::create(1.0f, JudgeProFile::CreateScene()));
				}
				else
				{
					global->CaveScene.Temporary = nullptr;
					global->NowPlayLevels.PlayLevels = ID;
					global->NowPlayLevels.PlayWorlds = 1;
					strcpy(global->NowPlayLevels.PlayWorldName, " - 现代世界 - ");

					Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SelectPlantsGameReadyScene::createScene()));
				}

				break;
			}
		});
}

void World_1::CreateGoBack()
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

				UserDefault::getInstance()->setFloatForKey("WORLD_1_POSITION", scrollView->getScrolledPercentHorizontal()); /* 记录位置 */
				UserDefault::getInstance()->flush();

				global->MainToWorld = false;
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectWorldScene::createScene()));
				break;
			}
		});
}