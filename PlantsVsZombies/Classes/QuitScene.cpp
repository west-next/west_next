#include "QuitScene.h"
#include "GlobalVariable.h"

Scene* QuitMenu::CreateQuitScene()
{
	return QuitMenu::create();
}

bool QuitMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}

	this->CreateBackGroundImage();

	this->CreateDiglog();

	return true;
}

void QuitMenu::CreateDiglog()
{
	QuitDialog = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(global->ImagesPath.find("QuitDialog")->second));
	QuitDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	QuitDialog->setScale(2.0f);
	this->addChild(QuitDialog);

	this->CreateButtons(global->GameText.find("退出游戏")->second, Vec2(90, 65), 1);
	this->CreateButtons(global->GameText.find("取消")->second, Vec2(225, 65), 2);

	/* 创建触摸监听 */
	this->CreateTouchtListener(QuitDialog);
}

void QuitMenu::CreateButtons(const std::string &Label, Vec2 &vec2,const int& ID)
{
	auto button = Button::create(global->ImagesPath.find("button")->second, global->ImagesPath.find("button_down")->second);
	button->setTitleLabel(label(Label, 20));
	button->setPosition(vec2);
	QuitDialog->addChild(button);

	QuitDialog->addChild(label(global->GameText.find("退出")->second, 25, Vec2(155, 160), 0, Color3B::YELLOW));
	QuitDialog->addChild(label(global->GameText.find("确定要退出游戏吗？")->second, 15, Vec2(160, 130), 0, Color3B::YELLOW));

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("gravebutton")->second), global->GlobalMusic.SoundEffect);
			break;
		case Widget::TouchEventType::ENDED:
			switch (ID)
			{
			case 1:
				Director::getInstance()->end();
				break;
			case 2:
				Director::getInstance()->popScene();
				break;
			}
		}
	});
}