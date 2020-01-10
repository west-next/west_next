#include "JudgeProFile.h"

Scene* JudgeProFile::CreateScene()
{
	return JudgeProFile::create();
}

bool JudgeProFile::init()
{
	if (!Scene::init())
	{
		return false;
	}

	this->CreateBackGroundImage();

	UiImageText::getInstance()->ShowIsResume(this);

	return true;
}
