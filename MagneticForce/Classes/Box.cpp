#include "Box.h"

USING_NS_CC;


bool Box::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("box", false);
	m_MoveBox = Rect(-13, -21, 14, -3);
	m_HitBox = Rect(-13, -21, 14, 18);

	m_Friction = 100.0f;

	scheduleUpdate();
	
	return true;
}

void Box::update(float dTime)
{
	Unit::update(dTime);
}

