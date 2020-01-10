#include "Dialog.h"

using namespace cocos2d;

void Dialog::CreateBackGroundImage()
{
	/* 创建背景 */
	auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(global->ImagesPath.find("WorldBackground")->second));
	sprite->setPosition(Director::getInstance()->getWinSize() / 2);
	this->addChild(sprite);

	auto rotate = Sprite::create(global->ImagesPath.find("RunBackground")->second);
	rotate->setScale(3.5f);
	rotate->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	this->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto WorldBackgroundBlack = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	auto WorldBackgroundBlack1 = Sprite::create(global->ImagesPath.find("WorldBackgroundBlack")->second);
	WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	WorldBackgroundBlack->setPosition(Vec2(0, 1140));
	WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	WorldBackgroundBlack1->setPosition(Vec2(0, -60));
	WorldBackgroundBlack1->setFlippedY(true);
	this->addChild(WorldBackgroundBlack);
	this->addChild(WorldBackgroundBlack1);
}

EventListenerTouchOneByOne* Dialog::CreateTouchtListener(Sprite* sprite)
{
	/* 创建触摸监听 */
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch *t, Event *e) {
		if (sprite->getBoundingBox().containsPoint(t->getLocation()))
		{
			PhasePosition = t->getLocation() - sprite->getPosition();
			return true;
		}
		else return false;
	};
	listener->onTouchMoved = [=](Touch *t, Event *e) {
		sprite->setPosition(t->getLocation() - PhasePosition);
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);

	return listener;
}

Label* Dialog::label(const std::string &name, const float& fontsize, Vec2 &vec2, const float& space, const Color3B& color, const float& scale)
{
	auto label = Label::createWithTTF(name, "resources/fonts/fzse_gbk.ttf", fontsize);
	label->setScaleX(scale);
	label->setColor(color);
	label->setAdditionalKerning(space);//设置列间距
	label->enableShadow(Color4B(100, 20, 100, 255));//设置阴影
	label->setPosition(vec2);
	return label;
}

void Dialog::CreateLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, Vec2 &vec2, float& fontsize, const float& space, const Color3B& color)
{
	auto label = Label::createWithTTF(name, "resources/fonts/fzse_gbk.ttf", fontsize);
	label->setPosition(vec2);
	label->setColor(color);
	label->enableShadow(Color4B(100, 25, 100, 255));//设置阴影
	label->setAdditionalKerning(space);//设置列间距
	MenuItem->addChild(label);

	auto Buttons = Menu::create(MenuItem, NULL);
	Buttons->setPosition(Vec2(0, 0));
	sprite->addChild(Buttons);
}