#include "GameScene.h"
#include "TileMap.h"
#include "AppDelegate.h"

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
