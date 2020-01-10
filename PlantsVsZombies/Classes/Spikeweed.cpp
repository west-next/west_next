#include "Spikeweed.h"

Spikeweed* Spikeweed::_instance = nullptr;

Spikeweed* Spikeweed::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Spikeweed;
	}
	return _instance;
}

void Spikeweed::Initialization()
{
	Spikeweed::getInstance()->SpikeweedAnimation.clear();
}

void Spikeweed::CreateSpikeweedImage(const Vec2& position, Node* node)
{
	SpikeweedImage = CreateImage("Spikeweed", position, node);
	SpikeweedImage->setScale(1.0f);
}

void Spikeweed::CreateSpikeweedAnimation(const Vec2& RowAndColumn, const Vec2& position, Node* node, const int& order, const int& tag)
{
	/* 创建动画 */
	auto SpikeweedAnimations = CreateAnimation("Spikeweed", "Spikeweed_Normal", position + Vec2(0, -10), 0.8f, 1.0f);
	SpikeweedAnimations->setAnchorPoint(Vec2(0, 0));
	SpikeweedAnimations->setScale(0.8f);
	node->addChild(SpikeweedAnimations, order, tag);

	_SpikeweedAnimation animation;
	animation.Animation = SpikeweedAnimations;
	animation.BloodVolume = 300;
	animation.tag = tag;
	animation.RowAndColumn = RowAndColumn;
	animation.position = position;
	animation.Attack = 20;
	animation.IsAttack = true;
	animation.IsChanged = false;
	animation.IsHaveZombies = false;
	SpikeweedAnimation.insert(pair<int, _SpikeweedAnimation>(tag, animation));
}

void Spikeweed::CreateSpikeweedListener(Node* node)
{
	node->schedule(CC_CALLBACK_1(Spikeweed::Update, this, node), "UpdateSpikeweed");
}

void Spikeweed::Update(float Time, Node* node)
{
	JudgePeaShooterAndZombiesPosition(node);
}

void Spikeweed::JudgePeaShooterAndZombiesPosition(Node* node)
{
	for (SpikeweedIter = SpikeweedAnimation.begin(); SpikeweedIter != SpikeweedAnimation.end(); /*++SpikeweedIter*/)
	{
		for (global->Iter = global->Zombies.begin(); global->Iter != global->Zombies.end(); ++global->Iter)
		{
			if (global->Iter->second.BloodVolume > 0 && global->Iter->second.ZombiesAnimation->isVisible() &&                                   /* 僵尸没有死亡 */
				fabs(global->Iter->second.ZombiesAnimation->getPositionX() - SpikeweedIter->second.Animation->getPositionX()) <= 90 &&          /* 僵尸遇到植物 */
				fabs((global->Iter->second.ZombiesAnimation->getPositionY() - 10) - SpikeweedIter->second.Animation->getPositionY()) <= 10)     /* 僵尸与植物在同一行 */
			{
				SpikeweedIter->second.IsHaveZombies = true; /* 有僵尸标记 */
				if (!SpikeweedIter->second.IsChanged)       /* 判断动画是否已经改变 */
				{
					SpikeweedIter->second.Animation->addAnimation(0, "Spikeweed_Attack", true);
					SpikeweedIter->second.IsChanged = true;
				}

				auto attack = SpikeweedIter;
				SpikeweedIter->second.Animation->setEventListener([=](spTrackEntry* entry, spEvent* event)
					{
						if (strcmp(event->data->name, "Attack") == 0)
						{
							attack->second.IsAttack = false;
						}
					});
				if (!SpikeweedIter->second.IsAttack)
				{
					PlayAttractMusic(global->Iter->second.MusicType);

					global->Iter->second.ZombiesAnimation->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

					if (global->Iter->second.ShieldVolume1 > 0)
					{
						global->Iter->second.ShieldVolume1 -= SpikeweedIter->second.Attack;
					}
					else if (global->Iter->second.ShieldVolume > 0)
					{
						global->Iter->second.ShieldVolume -= SpikeweedIter->second.Attack;
					}
					else
					{
						global->Iter->second.BloodVolume -= SpikeweedIter->second.Attack;
					}
					//SpikeweedIter->second.IsAttack = true;
				}
			}
		}
		if (!SpikeweedIter->second.IsHaveZombies &&  /* 没有僵尸 */
			SpikeweedIter->second.IsChanged)     
		{
			SpikeweedIter->second.Animation->addAnimation(0, "Spikeweed_Normal", true);
			SpikeweedIter->second.IsChanged = false;
		}
		SpikeweedIter->second.IsHaveZombies = false;
		SpikeweedIter->second.IsAttack = true;
		
		/* 删除地刺 */
		if (SpikeweedIter->second.BloodVolume <= 0)
		{
			global->MapSize.PlantsMap[(int)SpikeweedIter->second.RowAndColumn.y][(int)SpikeweedIter->second.RowAndColumn.x] = -1;
			node->getParent()->removeChildByTag(SpikeweedIter->second.tag);
			SpikeweedAnimation.erase(SpikeweedIter++);
		}
		else
		{
			SpikeweedIter++;
		}
	}
}

void Spikeweed::PlayAttractMusic(const int& MusicType)
{
	switch (MusicType)
	{
	case 0:
		rand() % 2 == 0 ? PlaySoundEffect("kernelpult") : PlaySoundEffect("kernelpult2");
		break;
	case 1:
		rand() % 2 == 0 ? PlaySoundEffect("shieldhit") : PlaySoundEffect("shieldhit2");
		break;
	}
}
