/****************************************
ProjectName:     PlantsVsZombies
Author:          ErLinEryi
LastModifyTime:  2020.01.10 18:30
****************************************/

#include "cocos2d.h"
#include "AppDelegate.h"
#include "PlantsVsZombies.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
