#pragma once
#include "cocos2d.h"
#include "Dialog.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;

class QuitMenu :public Dialog
{
public:
	static Scene* CreateQuitScene();
	virtual bool init();
	CREATE_FUNC(QuitMenu);

private:
	Sprite* QuitDialog;                                     /* 对话框 */
	
private:

	/* 创建对话框 */
	void CreateDiglog();                      

	/* 创建按钮 */
	void CreateButtons(const std::string &Label, Vec2 &vec2, const int &ID);                     
};