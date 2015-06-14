#include "Portal.h"
#include "DataManager.h"
#include "Player.h"
#include "GameScene.h"

USING_NS_CC;

bool Portal::init(const std::string& nextStage)
{
	if (!Unit::init())
	{
		return false;
	}

	m_NextStage = nextStage;

	scheduleUpdate();

	return true;
}

Portal* Portal::create(const std::string& nextStage)
{
	Portal* ret = new Portal();

	if (ret != nullptr && ret->init(nextStage))
	{
		ret->autorelease();

		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;

		return ret;
	}
}

void Portal::update(float dTime)
{
	auto pos = DataManager::getInstance()->getPlayer()->getPosition();
	auto moveBox = DataManager::getInstance()->getPlayer()->getMoveBox();
	auto myBox = cocos2d::Rect(getPositionX() - 8.0f, getPositionY() - 8.0f, 16.0f, 16.0f);

	cocos2d::Rect realHit(pos.x + moveBox.origin.x, pos.y + moveBox.origin.y,
		moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);

	if (myBox.intersectsRect(realHit))
	{
		Director::getInstance()->replaceScene(GameScene::createScene(m_NextStage));
	}
}
