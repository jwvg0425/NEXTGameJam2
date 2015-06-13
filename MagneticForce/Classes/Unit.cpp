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
		m_Sprite->stopAllActions();

		if (!isAni)
		{
			m_Sprite->setTexture(DataManager::getInstance()->getSprite(name)->getTexture());
		}
		else
		{
			auto animation = DataManager::getInstance()->getAnimation(name);
			auto animate = Animate::create(animation);
			auto repeat = RepeatForever::create(animate);

			m_Sprite->runAction(repeat);
		}
	}
	else
	{

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
	setZOrder(-getPositionY());
}

void Unit::collision(const cocos2d::Vector<Unit*>& units, float power)
{

}

void Unit::collision(float power)
{

}
