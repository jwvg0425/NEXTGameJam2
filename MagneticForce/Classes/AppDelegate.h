#pragma once
#include "cocos2d.h"
#define WIDTH 800
#define HEIGHT 600

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    ~AppDelegate() override;
    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};