#include "GameScene.h"
#include "TileMap.h"

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

	addChild(m_Map, -1);

	/*
	auto sprite = Sprite::create();
	auto ani = DataManager::getInstance()->getAnimation("player_down");
	auto animate = Animate::create(ani);
	auto repeat = RepeatForever::create(animate);

	sprite->runAction(repeat);
	sprite->setPosition(100, 100);

	addChild(sprite);
	*/

    return true;
}