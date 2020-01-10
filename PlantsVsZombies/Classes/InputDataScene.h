#pragma once
#include "cocos2d.h"
#include "Dialog.h"
#include "cocos-ext.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::extension;

class InputDataMenu :public Dialog
{
public:
	static Scene* CreateInputDataScene();
	virtual bool init();
	CREATE_FUNC(InputDataMenu);


private:
 
	Sprite* CaveFileDialog;
	Sprite* InputDataDialog;                                /* 对话框 */
	EditBox* editbox;                                       /* 输入框 */
	TextField* _textField;
	Sprite* InputCursor;
	cocos2d::Vec2  _beginPos;
	string InputString, newInputstring, LastString;
	int CaveFileNumber = -1;

	map<int, Button*> DataButton;

private:

	/* 创建对话框 */
	void CreateDialogs();
	void CreateInputDialog();

	/* 创建按钮 */
	void CreateButtons(Sprite* sprite, const std::string &Label, Vec2 &vec2, const float& scale, const int& ID);
	void CreateDataButton(Sprite* sprite, const std::string& Label, Vec2& vec2, const float& scale, const int& ID);
   
	void  SetButtonColor(Button* button);
	Label* onTextFieldInsertText();
	Label* onTextFieldDeleteBackward();
};
