#pragma once
#include "cocos2d.h"

class TileMap;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    bool init() override;
    
	CREATE_FUNC(GameScene);

private:
	TileMap* m_Map;
};