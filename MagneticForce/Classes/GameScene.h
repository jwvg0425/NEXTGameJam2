#pragma once
#include "cocos2d.h"

class TileMap;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    bool init() override;
    
	CREATE_FUNC(GameScene);

	// x, y 좌표가 중심에 오도록 화면 좌표 변경
	void setFocus(float x, float y);

	TileMap* getMap() const { return m_Map; }

private:
	TileMap* m_Map;
};