#pragma once
#include "cocos2d.h"
#include "Unit.h"

class TileMap;
class UILayer;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(const std::string& fileName);
    bool init(const std::string& fileName);
    
	static GameScene* create(const std::string& fileName);

	// x, y 좌표가 중심에 오도록 화면 좌표 변경
	void setFocus(float x, float y);

	TileMap* getMap() const { return m_Map; }

	void addUnit(Unit* unit);

	bool moveCheck(Unit* unit, cocos2d::Point pos);
	bool hitCheck(Unit* unit, cocos2d::Point pos);
	void conditionCheck(std::function<bool(Unit*)> func, OUT cocos2d::Vector<Unit*>& units);

	void update(float dTime);
	void physics(float dTime);

	const std::string& getMapName() const { return m_Name; }

private:
	TileMap* m_Map;
	cocos2d::Vector<Unit*> m_Units;
	std::string m_Name;
};