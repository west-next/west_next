#pragma once
#include "Dialog.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "GameEasterEggs.h"


#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::extension;

class OptionsMenu :public Dialog
{
public:
	static Scene* CreateOptionsScene();
	virtual bool init() override;
	CREATE_FUNC(OptionsMenu);

private:

	UserDefault* userdefault = UserDefault::getInstance();
	Director* director = Director::getInstance();

protected:
	Sprite* Option;                                         /* 对话框 */
	
protected:

	virtual void Create_Dialog();                                                                /* 创建对话框 */
	virtual void CreateButton();                                                                 /* 创建按钮 */
	CheckBox* CreateCheckBox(Vec2& vec2, Vec2& vec2_, const std::string& Label, const int& ID,
		const std::string NotSelect, const std::string Select, const bool IsNew = false);        /* 创建复选框 */
	ControlSlider* CreateSlider(Vec2& vec2, Vec2& vec2_, const std::string& Label, const int& ID,
		const std::string bgFile, const std::string progressFile,
		const std::string thumbFile, const bool IsNew = false);                                  /* 创建滑动条 */
	ControlSlider* CreateSlider(Vec2& vec2, Vec2& vec2_, const std::string& Label, const int& ID,
		Sprite* BgFile, Sprite* progressFile, Sprite* thumbFile, const bool IsNew = false);      /* 创建滑动条 */

	void BackGroundMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);              /* 背景音乐音量调节回调函数*/
	void SoundEffectMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);             /* 音效音乐音量调节回调函数*/
};