#include "Unit.h"
#include "DataManager.h"
#include "GameScene.h"
#include "TileMap.h"


USING_NS_CC;

bool Unit::init()
{
	return cocos2d::Node::init();
}

void Unit::changeSprite(const std::string& name, bool isAni)
{
	if (m_Sprite != nullptr)
	{
		removeChild(m_Sprite);
	}

	if (!isAni)
	{
		m_Sprite = DataManager::getInstance()->getSprite(name);
	}
	else
	{
		m_Sprite = Sprite::create();
		auto animation = DataManager::getInstance()->getAnimation(name);
		auto animate = Animate::create(animation);
		auto repeat = RepeatForever::create(animate);

		m_Sprite->runAction(repeat);
	}

	addChild(m_Sprite);
}

void Unit::changeDirection(Direction dir)
{
	m_Dir = dir;
}

bool Unit::solidCheck(cocos2d::Point pos)
{
	GameScene* scene = static_cast<GameScene*>(getParent());

	if (scene->getMap()->isSolidTile(pos.x, pos.y, m_MoveBox))
	{
		return true;
	}

	return false;
}

void Unit::force(float fx, float fy)
{
	m_Force.add({ fx, fy });
}

void Unit::update(float dTime)
{
	//0.5f보다 작아지면 그냥 0
	setPosition(getPositionX() + m_Force.x, getPositionY() + m_Force.y);

	if (m_Force.x > 0.5f)
	{
		m_Force.x -= m_Friction *dTime;
	}
	else if (m_Force.x < -0.5f)
	{
		m_Force.x += m_Friction*dTime;
	}
	else
	{
		m_Force.x = 0.0f;
	}

	if (m_Force.y > 0.5f)
	{
		m_Force.y -= m_Friction *dTime;
	}
	else if (m_Force.y < -0.5f)
	{
		m_Force.y += m_Friction*dTime;
	}
	else
	{
		m_Force.y = 0.0f;
	}

	setZOrder(-getPositionY());
}
