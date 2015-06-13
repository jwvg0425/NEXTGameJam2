#include "UILayer.h"
#include "DataManager.h"
#include "AppDelegate.h"
#include "Player.h"

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

	auto skill_ui = DataManager::getInstance()->getSprite("skill_ui");

	skill_ui->setAnchorPoint({ 0.0f, 0.0f });
	skill_ui->setPosition(WIDTH / 2 + 140, 5);
	addChild(skill_ui);

	m_Pull = DataManager::getInstance()->getSprite("skill_a");
	m_Pull->setAnchorPoint({ 0.0f, 0.0f });
	m_Pull->setPosition(WIDTH / 2 + 140, 5 + 48);
	addChild(m_Pull);

	m_Push = DataManager::getInstance()->getSprite("skill_s");
	m_Push->setAnchorPoint({ 0.0f, 0.0f });
	m_Push->setPosition(WIDTH / 2 + 140 + 48, 5 + 48);
	addChild(m_Push);

	scheduleUpdate();

	return true;
}

void UILayer::update(float dTime)
{
	auto player = DataManager::getInstance()->getPlayer();

	m_HP->setTextureRect(cocos2d::Rect(0.0f, 0.0f, 20.0f + player->getHp() / player->getMaxHp()*201.0f, 64.0f));
	m_MP->setTextureRect(cocos2d::Rect(0.0f, 0.0f, 20.0f + player->getMp() / player->getMaxMp()*201.0f, 64.0f));
}
