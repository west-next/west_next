#pragma once

#include "cocos2d.h"
#include "HelpScene.h"
#include "QuitScene.h"
#include "AudioEngine.h"
#include "spine/spine.h"
#include "OptionsSence.h"
#include "InputDataScene.h"
#include "SelectWorldScene.h"
#include "spine\spine-cocos2dx.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace spine;
using namespace cocos2d::experimental;

class MainMenu :public Scene
{
public:
	
	static Scene* createScene();            /* 创建场景 */
	virtual bool init();                    /* 初始化 */
	CREATE_FUNC(MainMenu);

private:
	
	bool PlayMusic[8] = { false };          /* 音乐是否播放 */
	Vec2 cur;                               /* 鼠标光标位置 */
	Text* UserText;                         /* 储存用户名字 */
	Node* UserNameActionParent;             /* 用户名称动作parent */
	Menu* menu[2];                          /* 菜单 */
	Sprite* sprite[17],*MainButton[9];      /* 精灵 */
	MenuItemImage* MenuItem[5];             /* 菜单项 */
	EventListenerMouse* Mouse;              /* 鼠标监听 */

	Global* global = Global::getInstance();
	Director* director = Director::getInstance();

private:
	
	short CheckCurInButtons();                                   /* 判断鼠标光标位置*/
	void CurUpdate(float time);                                  /* 鼠标光标定时器 */
	void UpdateUserNameOnce(float time);
	void SetCloudPosition(Node *node,int ID, const Vec2 &vec2);  /* 设置云的位置 */
	void PlayMusicBleepInGameButtons(int ID);                    /* 播放音乐 */
	void PlayMusicBleepInMainBuutons(int ID, const Vec2 &vec2);
private:

	/*游戏选择函数*/
	void BeginAdventureGame();     /* 冒险模式 */
	void BeginSurvivalGame();      /* 玩玩小游戏 */
	void BeginChallengesGame();    /* 解谜模式 */
	void BeginVasebreakerGame();   /* 生存模式 */

private:

	/*回调函数*/
	void MenuHelpCallBack(Ref* pSender);   /* 帮助 */
	void MenuOptionCallBack(Ref* pSender); /* 选项 */
	void MenuQuitCallBack(Ref* pSender);   /* 退出 */
	void MenuDataCallBack(Ref* pSender);   /* 数据 */

private:

	/*创建函数*/
	void CreateMainSprite();                                  /* 创建煮主菜单精灵 */
	void CreateParticle();                                    /* 创建粒子特效 */
	void CreateSmoke(const float &Scale, const Vec2 &vec2);   /* 创建烟雾粒子特效 */
	void CreateMouseListener();                               /* 创建鼠标监听器 */
	void CreateMainButton();                                  /* 创建按钮 */
	void CreateClouds();                                      /* 创建云 */
	void CreateAnimation();                                   /* 创建动画 */
	void CreateFlowers();                                     /* 创建花朵 */
	void CreateFlowers(const float &Scale, const Vec2 &vec2, const std::string &fileName, const int &ID);
};
