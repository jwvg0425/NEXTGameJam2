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

protected:
	Direction m_Dir = Direction::DOWN;
	cocos2d::Sprite* m_Sprite = nullptr;
};