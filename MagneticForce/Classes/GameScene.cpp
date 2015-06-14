#include "GameScene.h"
#include "TileMap.h"
#include "AppDelegate.h"
#include "UILayer.h"
#include "DataManager.h"

USING_NS_CC;

Scene* GameScene::createScene(const std::string& fileName)
{
    auto scene = Scene::create();
    auto layer = GameScene::create(fileName);

    scene->addChild(layer);
	scene->addChild(UILayer::create());

    return scene;
}

bool GameScene::init(const std::string& fileName)
{
    if ( !Layer::init() )
    {
        return false;
    }

	DataManager::getInstance()->setNowScene(this);
	m_Name = fileName;
	
	m_Map = TileMap::createWithFile(fileName, this);
	m_Map->setAnchorPoint({ 0.0f, 0.0f });

	addChild(m_Map, -100000);

	scheduleUpdate();
	

	DataManager::getInstance()->playBackgroundMusic("cave", true);

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

		if (!u->isSolid())
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

		if (!u->isSolid())
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

void GameScene::conditionCheck(std::function<bool(Unit*)> func, OUT cocos2d::Vector<Unit*>& units)
{
	for (auto u : m_Units)
	{
		if (func(u))
		{
			units.pushBack(u);
		}
	}
}

void GameScene::physics(float dTime)
{
	//물리 계산
	for (auto u : m_Units)
	{
		Point pos(u->getPositionX() + u->getForce().x*dTime, u->getPositionY() + u->getForce().y*dTime);
		cocos2d::Vector<Unit*> units;

		if (m_Map->isSolidTile(pos.x, pos.y, u->getMoveBox()))
		{
			//부딪히면 속도 0으로. 부딪혔음을 통보.
			u->collision(u->getForce().getLength());
			u->setForce({ 0.0f, 0.0f });
		}
		else
		{
			conditionCheck([pos, u](Unit* unit) -> bool
			{

				if (u == unit)
					return false;

				if (!unit->isSolid())
					return false;

				cocos2d::Rect moveBox = u->getMoveBox();
				cocos2d::Rect realMoveA(pos.x + moveBox.origin.x, pos.y + moveBox.origin.y,
					moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);

				moveBox = unit->getMoveBox();
				cocos2d::Rect realMoveB(unit->getPositionX() + moveBox.origin.x, unit->getPositionY() + moveBox.origin.y,
					moveBox.size.width - moveBox.origin.x, moveBox.size.height - moveBox.origin.y);

				if (realMoveA.intersectsRect(realMoveB))
				{
					return true;
				}

				return false;
			}, units);

			if (units.size() > 0)
			{
				std::set<std::pair<Unit*, Unit*>> pair;

				for (auto unit : units)
				{
					float power = u->getForce().getDistance(unit->getForce());
					if (pair.find(std::make_pair(u, unit)) == pair.end())
					{
						u->collision(unit, power);
						pair.insert(std::make_pair(u, unit));
					}
					if (pair.find(std::make_pair(unit, u)) == pair.end())
					{
						unit->collision(u, power);
						pair.insert(std::make_pair(unit, u));
					}
				}

				u->setForce({ 0.0f, 0.0f });
			}
			else
			{
				u->setPosition(pos);
			}
		}
		
		//마찰력보다 작아지면 그냥 0
		if (u->getForce().x > u->getFriction()*dTime)
		{
			u->setForce({ u->getForce().x - u->getFriction() *dTime, u->getForce().y });
		}
		else if (u->getForce().x < -u->getFriction()*dTime)
		{
			u->setForce({ u->getForce().x + u->getFriction()*dTime, u->getForce().y });
		}
		else
		{
			u->setForce({ 0.0f, u->getForce().y });
		}

		if (u->getForce().y > u->getFriction()*dTime)
		{
			u->setForce({u->getForce().x, u->getForce().y - u->getFriction() *dTime });
		}
		else if (u->getForce().y < -u->getFriction()*dTime)
		{
			u->setForce({ u->getForce().x, u->getForce().y + u->getFriction()*dTime });
		}
		else
		{
			u->setForce({ u->getForce().x, 0.0f });
		}
	}
}

void GameScene::update(float dTime)
{
	physics(dTime);
}

GameScene* GameScene::create(const std::string& fileName)
{
	GameScene* ret = new GameScene();

	if (ret != nullptr && ret->init(fileName))
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
