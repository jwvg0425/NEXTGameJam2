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
	auto map = TileMap::createWithFile("map");

	addChild(map);

	auto sprite = Sprite::create();
	auto ani = DataManager::getInstance()->getAnimation("player_down");
	auto animate = Animate::create(ani);
	auto repeat = RepeatForever::create(animate);

	sprite->runAction(repeat);
	sprite->setPosition(100, 100);

	addChild(sprite);

    return true;
}