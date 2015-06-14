#include "Thorn.h"
#include "DataManager.h"
#include "GameScene.h"

USING_NS_CC;

void Thorn::update(float dTime)
{
	Unit::update(dTime);

	setZOrder(-9999);

	//가시 켜져있을 때만 계산
	if (!m_IsOn)
		return;

	auto scene = DataManager::getInstance()->getNowScene();
	cocos2d::Vector<Unit*> units;

	scene->conditionCheck([this](Unit* unit) -> bool
	{
		cocos2d::Rect rect = { getPositionX() - 24.0f, getPositionY() - 24.0f, 48.0f, 48.0f };

		cocos2d::Rect moveBox = unit->getMoveBox();
		cocos2d::Rect realMove(unit->getPositionX() + moveBox.origin.x, unit->getPositionY() + moveBox.origin.y,
			moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);

		if (rect.intersectsRect(realMove))
		{
			return true;
		}

		return false;
	}, units);


	//가시에 닿은 유닛들은 데미지 입음
	for (auto u : units)
	{
		u->hurt(5.0f);
	}
}

void Thorn::changeOnOff()
{
	m_IsOn = !m_IsOn;

	if (m_IsOn)
	{
		changeSprite("thorn_on", false);
		DataManager::getInstance()->playEffect("thorn");
	}
	else
	{
		changeSprite("thorn_off", false);
		DataManager::getInstance()->playEffect("thorn");
	}
}

bool Thorn::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("thorn_off", false);
	m_IsStatic = true;
	m_IsSolid = false;

	//1초마다 가시 on off 자동으로 바뀌도록

	auto delay = DelayTime::create(1.0f);
	auto action = RepeatForever::create(Sequence::create(delay, CallFunc::create(CC_CALLBACK_0(Thorn::changeOnOff, this)), nullptr));

	runAction(action);

	scheduleUpdate();

	return true;
}
