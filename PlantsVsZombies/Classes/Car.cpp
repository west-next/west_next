#include "car.h"

Car* Car::_instance = nullptr;

Car* Car::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Car;
	}
	return _instance;
}

void Car::Initialization()
{
	Car::getInstance()->Cars.clear();
}

void Car::CreateCarAnimation(Node* node, const Vec2& position)
{
	static int cartag = 0;
	cartag++;

	/* 创建动画 */
	auto car = Sprite::create(global->ImagesPath.find("Car")->second);
	car->setPosition(position);
	car->setScale(0.1f);
	car->setGlobalZOrder(SetZOrder(position.y));
	node->addChild(car);
	car->runAction(ScaleTo::create(0.2f, 0.8f));

	_Car cars;
	cars.Image = car;
	cars.position = position;
	cars.IsLive = false;
	cars.AnimationID = 1;
	cars.tag = cartag;
	Cars.insert(pair<int, _Car>(cartag, cars));

	auto fog = Sprite::create(global->ImagesPath.find("AwardPickupGlow")->second);
	fog->setScale(4.0f);
	fog->setName("fog");
	fog->setPosition(Vec2(90, 30));
	car->addChild(fog);
	fog->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f),
		CallFunc::create([=]()
			{
				car->removeChildByName("fog");
			}), nullptr));

	auto shadow = Sprite::create(global->ImagesPath.find("plantshadow")->second);
	shadow->setScaleX(4.0f);
	shadow->setScaleY(3.0f);
	shadow->setPosition(Vec2(120, 30));
	car->addChild(shadow, -1);
}

void Car::CreateCarListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(Car::Update, this, node), 0.1f, "UpdateCar");
}

void Car::Update(float Time, Node* node)
{
	JudgeCarAndZombies(node);

	CarMove(node);
}

void Car::JudgeCarAndZombies(Node* node)
{
	for (cariter = Cars.begin(); cariter != Cars.end(); ++cariter)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume >= 0 && global->Iter->second.ZombiesAnimation->isVisible() &&             /* 僵尸没有死亡 */                                                                  /* 僵尸血量大于零 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() + 50) - cariter->second.position.y) <= 10 &&   /* 僵尸与小车在同一行 */
				global->Iter->second.ZombiesAnimation->getPositionX() - 80 < cariter->second.Image->getPositionX() &&      /* 僵尸遇到小车 */
				global->Iter->second.ZombiesAnimation->getPositionX() < 1730)                                              /* 进入地图的僵尸 */
			{
				cariter->second.IsLive = true;

				Zombie::getInstance()->ZombiesKilledByCar(node, global->Iter);
			}
		}
	}
}

void Car::CarMove(Node* node)
{
	for (cariter = Cars.begin(); cariter != Cars.end(); /*++cariter*/)
	{
		if (cariter->second.IsLive)
		{
			if (cariter->second.AnimationID == 1)
			{
				cariter->second.AnimationID = 2;
				AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("lawnmower")->second), global->GlobalMusic.SoundEffect);
				cariter->second.Image->runAction(MoveBy::create(2.5f, Vec2(1900, 0)));
			}	
		}

		if (cariter->second.Image->getPositionX() > 2100)
		{
			cariter->second.Image->setVisible(false);
		}

		if (!cariter->second.Image->isVisible())
		{
			node->removeChildByTag(cariter->second.tag);
			Cars.erase(cariter++);
		}
		else
		{
			cariter++;
		}
	}
}

float Car::SetZOrder(const float& pos_y)
{
	int const pos[5] = { 672,534,396,258,120 };
	for (int i = 0; i < 5; i++)
		if (pos[i] == pos_y)
			return (i + 1) * 2;
	return 0;
}
