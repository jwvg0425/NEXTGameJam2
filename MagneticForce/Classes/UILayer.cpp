#include "UILayer.h"
#include "DataManager.h"
#include "AppDelegate.h"

bool UILayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	auto ui_back = DataManager::getInstance()->getSprite("ui_back");

	ui_back->setAnchorPoint({ 0.0f, 0.0f });
	ui_back->setPosition(WIDTH / 2 - 120, 5);

	addChild(ui_back);

	m_HP = DataManager::getInstance()->getSprite("hpbar");

	m_HP->setAnchorPoint({ 0.0f, 0.0f });
	m_HP->setPosition(WIDTH / 2 - 120, 5);
	addChild(m_HP);

	m_MP = DataManager::getInstance()->getSprite("magneticbar");

	m_MP->setAnchorPoint({ 0.0f, 0.0f });
	m_MP->setPosition(WIDTH / 2 - 120, 5);
	addChild(m_MP);

	return true;
}
