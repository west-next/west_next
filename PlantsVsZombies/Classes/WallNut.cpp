#include "WallNut.h"

WallNut* WallNut::_instance = nullptr;

WallNut* WallNut::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)WallNut;
	}
	return _instance;
}

void WallNut::Initialization()
{
	WallNut::getInstance()->WallNutAnimations.clear();
}

void WallNut::CreateWallNutImage(const Vec2& position,Node *node)
{
	WallNutImage = CreateImage("Wallnut_body", position, node);
	WallNutImage->setScale(1.3f);
}

void WallNut::CreateWallNutAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	auto WallNutAnimation = CreateAnimation("WallNut", "WallNut_Nornal", position, 1.0f, 2.2f);
	WallNutAnimation->setSkin("WallNut_Normal");
	WallNutAnimation->setScale(0.8f);
	node->addChild(WallNutAnimation, order, tag);

	_WallNutAnimation animation;
	animation.Animation = WallNutAnimation;
	animation.BloodVolume = 3000;
	animation.tag = tag;
	animation.skinID = 1;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	WallNutAnimations.insert(pair<int, _WallNutAnimation>(tag, animation));
}

void WallNut::CreateWallNutListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(WallNut::Update, this, node), "UpdateWallNut");
}

void WallNut::Update(float Time, Node* node)
{
	JudgeWallNutAndZombies(node);
}

void WallNut::JudgeWallNutAndZombies(Node* node)
{
	for (WallNutIter = WallNutAnimations.begin(); WallNutIter != WallNutAnimations.end(); /*++WallNutIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                          /* 僵尸没有死亡 */
				global->Iter->second.ZombiesAnimation->getPositionX() >= WallNutIter->second.position.x &&                             /* 僵尸在植物的前方 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - WallNutIter->second.position.y) <= 10 &&           /* 僵尸与植物在同一行 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() <= 1730))                                                   /* 僵尸进入地图 */
			{
				if (WallNutIter->second.BloodVolume > 0 &&                                                                             /* 植物没有死 */
					fabs(global->Iter->second.ZombiesAnimation->getPositionX() - WallNutIter->second.position.x) <= rand() % 30 + 70)  /* 僵尸遇到植物 */
				{
					ZombiesEatPlants(global->Iter, &WallNutIter->second.BloodVolume, WallNutIter->second.Animation);
				}
				if (WallNutIter->second.BloodVolume <= 0 && !global->Iter->second.Speed && global->Iter->second.IsEat)
				{
					global->Iter->second.IsEat = false;
					if (global->Iter->second.AnimationID != 10)
					{
						global->Iter->second.ZombiesAnimation->setMix("Zombies_Eat", Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), 1.5f);
						global->Iter->second.ZombiesAnimation->addAnimation(1, Zombie::getInstance()->ZombiesTypeAniamtion(global->Iter->second.ZombiesType), true);
						global->Iter->second.Speed = global->Iter->second.CaveSpeed;
					}
				}
			}
		}
		if (WallNutIter->second.BloodVolume <= 2000 && WallNutIter->second.skinID == 1)
		{
			WallNutIter->second.Animation->setSkin("WallNut_Cracked1");
			WallNutIter->second.skinID = 2;
		}
		if (WallNutIter->second.BloodVolume <= 1000 && WallNutIter->second.skinID == 2)
		{
			WallNutIter->second.Animation->setSkin("WallNut_Cracked2");
			WallNutIter->second.skinID = 3;
		}
		if (WallNutIter->second.BloodVolume <= 0 && WallNutIter->second.skinID == 3)
		{
			WallNutIter->second.Animation->pause();
			WallNutIter->second.Animation->setVisible(false);
			WallNutIter->second.skinID = 4;
		}
		/* 删除 */
		if (!WallNutIter->second.Animation->isVisible() || WallNutIter->second.BloodVolume <= 0)
		{
			global->MapSize.PlantsMap[(int)WallNutIter->second.RowAndColumn.y][(int)WallNutIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(WallNutIter->second.tag);
			WallNutAnimations.erase(WallNutIter++);
		}
		else
		{
			WallNutIter++;
		}
	}
}