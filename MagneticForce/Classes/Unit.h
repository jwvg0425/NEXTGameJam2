#pragma once
#include "cocos2d.h"

enum class Direction
{
	LEFT = 0x1,
	UP = 0x2,
	RIGHT = 0x4,
	DOWN = 0x8
};

class Unit : public cocos2d::Node
{
public:
	Unit() = default;
	~Unit() override = default;
	bool init() override;

	void changeSprite(const std::string& name, bool isAni);

	virtual void changeDirection(Direction dir);

	bool moveCheck(cocos2d::Point pos);

	//힘을 가함
	void force(float fx, float fy);

	//기본적으로 해주어야하는 unit update 연산.
	void update(float dTime);

	cocos2d::Rect getMoveBox() const { return m_MoveBox; }
	cocos2d::Rect getHitBox() const { return m_HitBox; }

	virtual void collision(float power);
	virtual void collision(Unit* unit, float power);

	cocos2d::Vec2 getForce() const { return m_Force; }
	void setForce(cocos2d::Vec2 force) { m_Force = force; }

	float getFriction() const { return m_Friction; }

protected:

	Direction m_Dir = Direction::DOWN;
	cocos2d::Sprite* m_Sprite = nullptr;

	//중점을 기준으로 왼쪽,위까지 거리, 오른쪽,아래까지 거리 저장.(x,y,width,height 의미로 사용하지 않음)
	cocos2d::Rect m_MoveBox;
	cocos2d::Rect m_HitBox;

	//마찰 정도. 클 수록 힘이 빠르게 감소함.
	float m_Friction = 0.0f;
	cocos2d::Vec2 m_Force;
};