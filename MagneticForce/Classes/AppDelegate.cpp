#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
		glview = GLView::createWithRect("Magnetic Force", cocos2d::Rect(300, 300, WIDTH, HEIGHT));
		director->setOpenGLView(glview);
	}

	srand((unsigned)time(nullptr));

	director->setDisplayStats(false);
	director->setAnimationInterval(1.0 / 60);

	auto scene = GameScene::createScene("stage4");
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() 
{
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() 
{
	Director::getInstance()->startAnimation();
}
