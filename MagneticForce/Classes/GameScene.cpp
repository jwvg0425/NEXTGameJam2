#include "GameScene.h"
#include "TileMap.h"
#include "AppDelegate.h"
#include "UILayer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	m_Map = TileMap::createWithFile("map", this);
	m_Map->setAnchorPoint({ 0.0f, 0.0f });

	addChild(m_Map, -100000);

	m_UI = UILayer::create();

	addChild(m_UI, 1000);

    return true;
}

void GameScene::setFocus(float x, float y)
{
	float mapWidth = 48.0f * m_Map->getWidth();
	float mapHeight = 48.0f * m_Map->getHeight();

	float focusX, focusY;

	if (x < WIDTH / 2)
	{
		focusX = 0;
	}
	else if (x > mapWidth - WIDTH / 2)
	{
		focusX = - mapWidth + WIDTH;
	}
	else
	{
		focusX = -x + WIDTH / 2;
	}

	if (y < HEIGHT / 2)
	{
		focusY = 0;
	}
	else if (y > mapHeight - HEIGHT / 2)
	{
		focusY = - mapHeight + HEIGHT;
	}
	else
	{
		focusY = -y + HEIGHT / 2;
	}

	setPosition(focusX, focusY);

	m_UI->setPosition(-focusX, -focusY);
}

void GameScene::addUnit(Unit* unit)
{
	addChild(unit);
	m_Units.pushBack(unit);
}

bool GameScene::moveCheck(Unit* unit, cocos2d::Point pos)
{
	cocos2d::Rect moveBox = unit->getMoveBox();
	cocos2d::Rect realMoveA(pos.x + moveBox.origin.x,pos.y + moveBox.origin.y,
		moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);


	for (auto u : m_Units)
	{
		if (u == unit)
			continue;

		moveBox = u->getMoveBox();
		cocos2d::Rect realMoveB(u->getPositionX() + moveBox.origin.x, u->getPositionY() + moveBox.origin.y,
			moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);

		if (realMoveA.intersectsRect(realMoveB))
		{
			return true;
		}
	}

	return false;
}

bool GameScene::hitCheck(Unit* unit, cocos2d::Point pos)
{
	cocos2d::Rect hitBox = unit->getHitBox();
	cocos2d::Rect realHitA(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
		hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);


	for (auto u : m_Units)
	{
		if (u == unit)
			continue;

		hitBox = u->getHitBox();
		cocos2d::Rect realHitB(u->getPositionX() + hitBox.origin.x, u->getPositionY() + hitBox.origin.y,
			hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

		if (realHitA.intersectsRect(realHitB))
		{
			return true;
		}
	}

	return false;
}

void GameScene::hitCheck(std::function<bool(Unit*)> func, OUT cocos2d::Vector<Unit*>& units)
{
	for (auto u : m_Units)
	{
		if (func(u))
		{
			units.pushBack(u);
		}
	}
}
