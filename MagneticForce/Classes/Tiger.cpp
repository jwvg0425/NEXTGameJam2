#include "Tiger.h"
#include "DataManager.h"
#include "Player.h"
#include "GameScene.h"
#include "Effect.h"

USING_NS_CC;

bool Tiger::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("tiger_down2", false);

	m_MoveBox = { -13, -13, 13, -7 };
	m_HitBox = { -13, -13, 13, 13 };
	m_Friction = 100.0f;

	scheduleUpdate();

	return true;
}

void Tiger::update(float dTime)
{

}

void Tiger::collision(float power)
{

}

void Tiger::collision(Unit* unit, float power)
{

}

void Tiger::hurt(float hp)
{

}

void Tiger::invincible(float time)
{

}

void Tiger::die()
{

}
