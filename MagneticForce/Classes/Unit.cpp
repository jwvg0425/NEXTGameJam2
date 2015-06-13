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
		m_Sprite->getTexture()->setAliasTexParameters();
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

bool Unit::moveCheck(cocos2d::Point pos)
{
	GameScene* scene = static_cast<GameScene*>(getParent());

	if (scene->getMap()->isSolidTile(pos.x, pos.y, m_MoveBox))
	{
		return true;
	}

	if (scene->moveCheck(this, pos))
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
	Point pos(getPositionX() + m_Force.x*dTime, getPositionY() + m_Force.y*dTime);
	
	if (!moveCheck(pos))
	{
		setPosition(pos);
	}
	else
	{
		//부딪히면 데미지 입고 힘 0으로. 속도 10당 데미지 1.
		m_Hp -= m_Force.getLength() / 10.0f;
		m_Force.x = 0.0f;
		m_Force.y = 0.0f;
	}

	//마찰력보다 작아지면 그냥 0
	if (m_Force.x > m_Friction*dTime)
	{
		m_Force.x -= m_Friction *dTime;
	}
	else if (m_Force.x < -m_Friction*dTime)
	{
		m_Force.x += m_Friction*dTime;
	}
	else
	{
		m_Force.x = 0.0f;
	}

	if (m_Force.y > m_Friction*dTime)
	{
		m_Force.y -= m_Friction *dTime;
	}
	else if (m_Force.y < -m_Friction*dTime)
	{
		m_Force.y += m_Friction*dTime;
	}
	else
	{
		m_Force.y = 0.0f;
	}

	setZOrder(-getPositionY());
}
