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
