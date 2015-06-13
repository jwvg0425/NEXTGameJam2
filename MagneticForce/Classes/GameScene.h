#pragma once
#include "cocos2d.h"
#include "Unit.h"

class TileMap;
class UILayer;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    bool init() override;
    
	CREATE_FUNC(GameScene);

	// x, y ��ǥ�� �߽ɿ� ������ ȭ�� ��ǥ ����
	void setFocus(float x, float y);

	TileMap* getMap() const { return m_Map; }

	void addUnit(Unit* unit);

	bool moveCheck(Unit* unit, cocos2d::Point pos);
	bool hitCheck(Unit* unit, cocos2d::Point pos);
	void hitCheck(std::function<bool(Unit*)> func, OUT cocos2d::Vector<Unit*>& units);

private:
	TileMap* m_Map;
	cocos2d::Vector<Unit*> m_Units;
	UILayer* m_UI;
};