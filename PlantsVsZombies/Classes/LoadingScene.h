#pragma once
#include <map>
#include "cocos2d.h"
#include "MainMenu.h"
#include "LevelData.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class LoadingScene :public Scene
{
public:
	static Scene* CreateLaodingScene();          /* 创建场景 */
	virtual bool init() override;                /* 初始化 */
	CREATE_FUNC(LoadingScene); 

private:

	int TextNumbers;                               /* 文本数 */
	int LoadFileNumbers = 0;                       /* 文件加载数 */
	int AllFileNumbers;                            /* 文件总数(图片，音乐，动画，文本) */
	bool FlowerVisible[5] = { false };             /* 加载花朵是否可见 */
	float LoadingPrecent = 0;                      /* 加载进度 */
	Sprite* sprite[8];                             /* 精灵图片 */
	MenuItemLabel* label;                          /* 文字标签 */
	ui::LoadingBar* loadingBar;                    /* 进度条 */
	EventListenerTouchOneByOne* Listener;          /* 加载监听 */

	Global* global = Global::getInstance();        /* 全局变量单例 */
	Director* director = Director::getInstance();  /* 导演单例 */
	FileUtils* Files = FileUtils::getInstance();   /* 文件单例 */

private:

	void update(float Time) override;                 /* 定时器 */

	void SetSystem();                                 /* 设置系统参数 */
	void CalculateFileNumbers();                      /* 计算文件总数 */

	void LoadUserData();                              /* 加载用户数据 */
	void LoadingText();                               /* 加载文字 */
	void LoadingImage();                              /* 加载图片 */
	void LoadingMusic();                              /* 加载音乐 */
	void LoadingAnimation();                          /* 加载动画 */

	void ShowLoadingBackGround();                     /* 展示背景 */
	void ShowTileAndLoadingBar();                     /* 展示进度条 */
	void ShowLoadingBarFlower(const int &ID);         /* 展示进度条上的花 */
	void BeginLoadingImageAndMusic();                 /* 开始加载图片与音乐 */

	void RunLoGoCallBack(Node* node,const int &ID);   /* 展示logo回调 */
	void LoadingTextCallBack();                       /* 加载文字回调 */
	void LoadingImageCallBack(Ref* pSender);          /* 加载图片回调 */
	void LoadingMusicCallBack(bool isSucceed);        /* 加载音乐回调 */
	void LoadingAnimationCallBack();                  /* 加载动画回调 */
	void BeginGameCallBack(Ref* pSender);             /* 开始游戏回调 */

	int OpenResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption = false);  /* 打开资源路径 */
};