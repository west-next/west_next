#include "ui/CocosGUI.h"
#include "LoadingScene.h"
#include "GlobalVariable.h" 
#include "tinyxml2/tinyxml2.h"

#define MYDEBUG 0

using namespace ui;

Scene* LoadingScene::CreateLaodingScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	/* 计算文件总数 */
	this->CalculateFileNumbers(); 

	/* 设置系统参数 */
	this->SetSystem();

	/* 加载用户信息 */
	this->LoadUserData();

	/* 展示加载界面 */
	this->ShowLoadingBackGround();
	
	//wglSwapIntervalEXT(0);
	return true;
}

void LoadingScene::SetSystem()
{
	/* 设置光标 */
	director->getOpenGLView()->setCursor(global->ImagesPath.find("cursor")->second, Point::ANCHOR_TOP_LEFT);

	/* 设置图标 */
	director->getOpenGLView()->setIcon(global->ImagesPath.find("PlantsVsZombies")->second);

}

void LoadingScene::LoadUserData()
{
	auto userdefault = UserDefault::getInstance();

	/* 杀死僵尸数 */
	global->UserGameData.KillAllZombies = userdefault->getIntegerForKey("KILLALLZOMBIES");
	/* 使用植物数量 */
	global->UserGameData.UsePlantsNumbers = userdefault->getIntegerForKey("USEPLANTSNUMBERS");
	/* 闯关失败个数 */
	global->UserGameData.BreakThrough = userdefault->getIntegerForKey("BREAKTHROUGH");
	/* 存档编号 */
	global->UserGameData.UserDataNumber = userdefault->getIntegerForKey("USERDATANUMBER");

	/* 显示彩蛋 */
	global->NowLevels[0].IsBeginShowEggs = userdefault->getBoolForKey("ISBEGINSHOWEGGS");

	/* 金币数 */
	global->CoinNumbers = userdefault->getIntegerForKey("COINNUMBERS");

	/* 音效 */
	global->GlobalMusic.SoundEffect = userdefault->getFloatForKey("SOUNDEFFECT");

	/* 音乐 */
	global->GlobalMusic.BackGroundMusicVolume= userdefault->getFloatForKey("GLOBALMUSIC");

	/* 读取用户存档名称 */
	for (int i = 0; i < 8; i++)
	{
		if (!userdefault->getStringForKey(global->UserData.UserCaveFileNameKey[i].c_str()).size())
		{
			global->UserData.UserCaveFileName[i] = "未命名存档";
		}
		else
		{
			global->UserData.UserCaveFileName[i] = userdefault->getStringForKey(global->UserData.UserCaveFileNameKey[i].c_str());
		}
	}

	/* 用户名称 */
	global->UserData.UserName = global->UserData.UserCaveFileName[global->UserGameData.UserDataNumber];
	global->UserData.IsUpdate = true;

	/* 显示信息 */
	switch (userdefault->getBoolForKey("SHOWINFORMATION"))
	{
	case true:
		director->setDisplayStats(true);
		global->GlobalParameter.Global_Information = cocos2d::ui::CheckBox::EventType::SELECTED;
		break;
	case false:
		director->setDisplayStats(false);
		global->GlobalParameter.Global_Information = cocos2d::ui::CheckBox::EventType::UNSELECTED;
		break;
	}

	/* 是否全屏 */
	switch (userdefault->getBoolForKey("SHOWFULLSCREEN"))
	{
	case true:
		((GLViewImpl*)director->getOpenGLView())->setFullscreen();
		global->GlobalParameter.Global_FullScreen = cocos2d::ui::CheckBox::EventType::SELECTED;
		break;
	case false:
		((GLViewImpl*)director->getOpenGLView())->setWindowed(1280, 720);
		global->GlobalParameter.Global_FullScreen = cocos2d::ui::CheckBox::EventType::UNSELECTED;
		break;
	}
	
	/* 是否高帧率 */
	switch (userdefault->getBoolForKey("SHOWHIGHFPS"))
	{
	case true:
		director->setAnimationInterval(1.0f / 60);
		global->GlobalParameter.Global_HighFPS = cocos2d::ui::CheckBox::EventType::SELECTED;
		global->NowFps = 60;
		break;
	case false:
		director->setAnimationInterval(1.0f / 45);
		global->GlobalParameter.Global_HighFPS = cocos2d::ui::CheckBox::EventType::UNSELECTED;
		global->NowFps = 45;
		break;
	}

	/* 是否拉伸显示 */
	switch (userdefault->getBoolForKey("STRETCHINGSHOW"))
	{
	case true:
		global->GlobalParameter.Global_StretchingShow = cocos2d::ui::CheckBox::EventType::SELECTED;
		director->getOpenGLView()->setDesignResolutionSize(director->getWinSize().width, director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
		break;
	case false:
		global->GlobalParameter.Global_StretchingShow = cocos2d::ui::CheckBox::EventType::UNSELECTED;
		director->getOpenGLView()->setDesignResolutionSize(director->getWinSize().width, director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
		break;
	}

	/* 是否隐藏鼠标 */
	switch (userdefault->getBoolForKey("CURSORHIDE"))
	{
	case true:
		global->GlobalParameter.Global_CursorNotHide = cocos2d::ui::CheckBox::EventType::SELECTED;
		break;
	case false:
		global->GlobalParameter.Global_CursorNotHide = cocos2d::ui::CheckBox::EventType::UNSELECTED;
		break;
	}
}

void LoadingScene::ShowLoadingBackGround()
{
	/* 播放音乐 */
	global->changeBgMusic("mainmusic", true);
	
	/* 获取窗口大小 */
	auto const size = Director::getInstance()->getWinSize();

	/* 创建精灵 */
	sprite[0] = Sprite::create(global->ImagesPath.find("PopCap_Logo1")->second);
	sprite[1] = Sprite::create(global->ImagesPath.find("PopCap_Logo")->second);
	sprite[7] = Sprite::create("resources/Text/txt/About.txt");
	sprite[2] = Sprite::create(global->ImagesPath.find("titlescreen")->second);

	/* 为精灵设置名字 */
	sprite[0]->setName("0");
	sprite[1]->setName("1");
	sprite[7]->setName("7");
	
	/* 设置精灵大小 */
	sprite[0]->setScale(1.5f);
	sprite[1]->setScale(1.5f);
	sprite[2]->setContentSize(size);
	sprite[7]->setScale(1.5f);

	/* 设置精灵的位置 */
	sprite[0]->setPosition(size / 2);
	sprite[1]->setPosition(size / 2);
	sprite[7]->setPosition(size / 2);
	sprite[2]->setPosition(size / 2);

	/* 设置精灵透明度 */
	sprite[0]->setOpacity(0);
	sprite[1]->setOpacity(0);
	sprite[7]->setOpacity(0);
	sprite[2]->setOpacity(0);

	/* 把精灵加载到场景中 */
	this->addChild(sprite[0]);
	this->addChild(sprite[1]);
	this->addChild(sprite[7]);
	this->addChild(sprite[2]);

	/* 设置精灵不可见 */
	sprite[1]->setVisible(false);
	sprite[7]->setVisible(false);
	sprite[2]->setVisible(false);

	/* 设置精灵动作 */
	sprite[0]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::RunLoGoCallBack, this, 1)), NULL));
}

void LoadingScene::RunLoGoCallBack(Node* node, const int &ID)
{
	switch (ID)
	{
	case 1:
		this->removeChildByName("0"); /* 从场景中移除名字为0的孩子 */
		sprite[1]->setVisible(true);  /* 设置精灵1可见 */
		sprite[1]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::RunLoGoCallBack, this, 5)), NULL));
		break;
	case 2:
		this->removeChildByName("7"); /* 从场景中移除名字为7的孩子 */
		sprite[2]->setVisible(true);  /* 设置精灵2可见 */
		sprite[2]->runAction(Sequence::create(FadeIn::create(2.0f),CallFuncN::create(CC_CALLBACK_1(LoadingScene::RunLoGoCallBack, this, 3)), NULL));
		break;
	case 3:
		this->ShowTileAndLoadingBar(); /* 展示标题和进度条 */
		break;
	case 4:
		this->BeginLoadingImageAndMusic(); /* 开始加载图片和音乐 */
		break;
	case 5:
		this->removeChildByName("1"); /* 从场景中移除名字为1的孩子 */
		sprite[7]->setVisible(true);  /* 设置精灵7可见 */
		sprite[7]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(4.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::RunLoGoCallBack, this, 2)), NULL));
		break;
	}
}

void LoadingScene::ShowTileAndLoadingBar()
{
	/* 获取窗口大小 */
    auto const size = director->getWinSize();

	/* 创建精灵 */
	sprite[3] = Sprite::create(global->ImagesPath.find("PvZ_Logo")->second);
	sprite[4] = Sprite::create(global->ImagesPath.find("LoadBar_dirt")->second);
	sprite[5] = Sprite::create(global->ImagesPath.find("SodRollCap")->second);
	sprite[6] = Sprite::create(global->ImagesPath.find("spark")->second);

	/* 创建裁剪节点 */
	auto clippingNode = ClippingNode::create();

	/* 获取第三个精灵的大小 */
	Size const SpriteSize = sprite[3]->getContentSize();

	/* 为精灵设置初始位置 */
	sprite[4]->setPosition(Vec2(size.width / 2, -100));
	sprite[5]->setPosition(Vec2(10, 95));
	sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
	clippingNode->setPosition(Vec2(size.width / 2 + 10, 1100));

	/* 为精灵设置大小 */
	sprite[3]->setScale(2.0f);
	sprite[3]->setScaleX(2.5f);
	sprite[4]->setScale(2.0f);
	sprite[6]->setScale(2.0f);

	/* 让精灵运动起来 */
	sprite[4]->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(size.width / 2, 150)),CallFuncN::create(CC_CALLBACK_1(LoadingScene::RunLoGoCallBack, this, 4)), NULL));
	sprite[6]->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, Vec2(SpriteSize.width, 0)), DelayTime::create(2.0f), CallFunc::create([=]() {sprite[6]->setPosition(Vec2(-SpriteSize.width, 0)); }), NULL)));
    clippingNode->runAction(MoveTo::create(0.5f, Vec2(size.width / 2, 900)));

	/* 加到场景中 */
	this->addChild(sprite[4]);
	this->addChild(clippingNode);
	sprite[4]->addChild(sprite[5],1);
	
	/* 设置裁剪节点的基本属性 */
	clippingNode->setAlphaThreshold(0.01f);   //设置alpha闸值
	clippingNode->setContentSize(SpriteSize); //设置尺寸大小
	clippingNode->setStencil(sprite[3]);      //设置模板stencil
	clippingNode->addChild(sprite[3]);        //先添加标题,会完全显示出来,因为跟模板一样大小
	clippingNode->addChild(sprite[6]);        //会被裁减

	/* 创建菜单标签 */
	label = MenuItemLabel::create(Label::createWithTTF("加载中......", "resources/fonts/fzse_gbk.ttf", 20), CC_CALLBACK_1(LoadingScene::BeginGameCallBack, this));
	label->setColor(Color3B::YELLOW);
	label->setEnabled(false);
	
	/* 创建菜单 */
	auto menu = Menu::create(label, NULL);
	menu->setPosition(150, 55);
	sprite[4]->addChild(menu);
}

void LoadingScene::BeginLoadingImageAndMusic()
{
	/* 创建进度条 */
	loadingBar = LoadingBar::create(global->ImagesPath.find("LoadBar_grass")->second);
	loadingBar->setDirection(LoadingBar::Direction::LEFT); /* 设置加载方向 */
	loadingBar->setPosition(Vec2(150, 70));
	sprite[4]->addChild(loadingBar);

	/* 定时器 */
	scheduleUpdate();

	LoadingText(); /* 加载文本 */
	
	LoadingMusic();/* 加载音乐 */
	
	LoadingAnimation(); /* 加载动画 */

	LoadingImage();/* 加载图片 */	
}

void LoadingScene::update(float Time)
{
	if (LoadingPrecent <= 100) 
	{
		loadingBar->setPercent(LoadingPrecent);              /* 设置加载进度 */
		sprite[5]->setScale(1 - LoadingPrecent / 170);       /* 设置精灵大小 */
		sprite[5]->setRotation(9 * LoadingPrecent);          /* 设置精旋转度数 */
		sprite[5]->setPosition(Vec2(10 + 290 / 100.0*LoadingPrecent, 100 - sprite[5]->getContentSize().height / 400 * LoadingPrecent));

		if (LoadingPrecent >= 20) ShowLoadingBarFlower(0);
		if (LoadingPrecent >= 40) ShowLoadingBarFlower(1);
		if (LoadingPrecent >= 60) ShowLoadingBarFlower(2);
		if (LoadingPrecent >= 80) ShowLoadingBarFlower(3);
		if (LoadingPrecent >= 100) ShowLoadingBarFlower(4);

	}
	if(LoadingPrecent >= 100) /* 如果加载完成 */
	{
		label->setString(global->GameText.find("点击开始")->second);  /* 重新设置标签文字内容 */
		auto action = TintBy::create(0.5f, 0, 255, 255);
		label->runAction(RepeatForever::create(Sequence::create(action,action->reverse(), nullptr)));
		sprite[5]->setVisible(false);  /* 设置精灵5为不可见 */
		label->setEnabled(true);       /* 设置触摸可行 */
	}
}

void LoadingScene::ShowLoadingBarFlower(const int &ID)
{
	srand(unsigned(time(nullptr)));
	if (!FlowerVisible[ID]) /* 如果花的ID是不可见 */
	{
		/* 创建花 */
		auto flower = Sprite::create(global->ImagesPath.find("sprout_petal")->second);
		flower->setScale(0.1f);
		flower->setPosition(Vec2(20 + (55 + rand() % 10)* ID, 85 + rand() % 10));
		flower->runAction(ScaleTo::create(0.2f, 0.5f));
		sprite[4]->addChild(flower);
		if (ID == 4) /* 如果ID==4 创建僵尸头*/
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("loadingbar_zombie")->second, false), global->GlobalMusic.SoundEffect);
			flower->setTexture(global->ImagesPath.find("ZombieHead")->second);
		}
		else  /* 否则创建花 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("loadingbar_flower")->second, false), global->GlobalMusic.SoundEffect);
			flower->setRotation(rand() % 180);
		}
		
		FlowerVisible[ID] = true; /* 花ID设置为可见 */
	}
}

void LoadingScene::CalculateFileNumbers()
{
#if MYDEBUG
	/* 文件总数 = 文本数 + 图片数 + 音乐数 + 动画数 */
	AllFileNumbers =
		OpenResourcesPath(global->TextPath, "resources/Text/TextPath.reanim.compiled", true) +
		OpenResourcesPath(global->ImagesPath, "resources/Text/ImagePath.reanim.compiled", true) +
		OpenResourcesPath(global->MusicsPath, "resources/Text/MusicPath.reanim.compiled", true) +
		OpenResourcesPath(global->AnimationPath, "resources/Text/AnimationPath.reanim.compiled", true);
#else
	AllFileNumbers =
		OpenResourcesPath(global->TextPath, "resources/Text/TextPath.xml", false) +
		OpenResourcesPath(global->ImagesPath, "resources/Text/ImagePath.xml", false) +
		OpenResourcesPath(global->MusicsPath, "resources/Text/MusicPath.xml", false) +
		OpenResourcesPath(global->AnimationPath, "resources/Text/AnimationPath.xml", false);
#endif
}

int LoadingScene::OpenResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	if (IsEncryption)
	{
		std::string textpath = Files->getStringFromFile(xml);

		char Encryption[200000] = { 0 }, Password[200000] = { 0 };
		strcpy(Encryption, textpath.c_str());

		OpenLevelData::getInstance()->Decrypt(Encryption, Password);

		/* 打开资源路径 */
		doc->Parse(Password);
	}
	else
	{
		/* 打开资源路径 */
		doc->Parse(Files->getStringFromFile(xml).c_str());
	}

	auto root = doc->RootElement();
	for (auto e = root->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		for (auto at = e->FirstAttribute(); at; at = at->Next())
		{
			Path.insert(pair<string, string>(at->Name(), at->Value()));
		}
	}
	return Path.size();
}

void LoadingScene::LoadingText()
{
	/* 循环加载文本 */
	for (auto i : global->TextPath)
	{
		ValueVector Text = Files->getValueVectorFromFile(i.second);
		for (auto& file : Text)
		{
			auto file_map = file.asValueMap();
			global->GameText.insert(pair<string, string>(file_map.at("id").asString(), file_map.at("info").asString()));
		}
		/* 加载回调 */
		this->LoadingTextCallBack();
	}
}

void LoadingScene::LoadingImage()
{
    /* 循环加载图片 */
	for (auto i : global->ImagesPath)
	{
		director->getTextureCache()->addImageAsync(i.second, CC_CALLBACK_1(LoadingScene::LoadingImageCallBack, this));
	}
}

void LoadingScene::LoadingMusic()
{
	/* 循环加载音乐 */
	for (auto i : global->MusicsPath)
	{
		AudioEngine::preload(i.second, CC_CALLBACK_1(LoadingScene::LoadingMusicCallBack, this));
	}
}

void LoadingScene::LoadingAnimation()
{
	/* 循环加载动画 */
	for (auto i : global->AnimationPath)
	{
		/* 临时存储文件名字 */
		char JsonName[100], AtlasName[100];

		/* 转换 */
		sprintf(JsonName, "resources/Animations/Json/%s.json", (i.second).c_str());
		sprintf(AtlasName, "resources/Animations/Atlas/%s.atlas", (i.second).c_str());

		/* 加载 */
		spSkeletonJson* json = spSkeletonJson_createWithLoader((spAttachmentLoader*)Cocos2dAttachmentLoader_create(spAtlas_createFromFile(AtlasName, nullptr)));
		auto skeletonData = spSkeletonJson_readSkeletonDataFile(json, JsonName);
		spSkeletonJson_dispose(json);

		/* 把加载到的动画放入map中*/
		global->GlobalAnimationData.insert(pair<string, spSkeletonData*>(i.second, skeletonData));

		/* 进行回调 */
		this->LoadingAnimationCallBack();
	}
}

void LoadingScene::LoadingTextCallBack()
{
	LoadFileNumbers++;     /* 文件数加一 */
	LoadingPrecent = ((LoadFileNumbers * 1.0f) / AllFileNumbers) * 100;  /* 计算加载的百分比 */
}

void LoadingScene::LoadingImageCallBack(Ref* pSender)
{
	LoadFileNumbers++;     /* 文件数加一 */
	LoadingPrecent = ((LoadFileNumbers * 1.0f) / AllFileNumbers) * 100;  /* 计算加载的百分比 */
}

void LoadingScene::LoadingMusicCallBack(bool isSucceed)
{
	if (isSucceed)/* 如果加载成功 */
	{
		LoadFileNumbers++;     /* 文件数加一 */
		LoadingPrecent = ((LoadFileNumbers * 1.0f) / AllFileNumbers) * 100;  /* 计算加载的百分比 */
	}
}

void LoadingScene::LoadingAnimationCallBack()
{
	LoadFileNumbers++;     /* 文件数加一 */
	LoadingPrecent = ((LoadFileNumbers * 1.0f) / AllFileNumbers) * 100;  /* 计算加载的百分比 */
}

void LoadingScene::BeginGameCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(global->MusicsPath.find("buttonclick")->second), global->GlobalMusic.SoundEffect);
	director->replaceScene(MainMenu::createScene()); /* 切换场景 */
}