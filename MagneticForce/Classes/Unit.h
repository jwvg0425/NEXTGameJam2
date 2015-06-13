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

	bool solidCheck(cocos2d::Point pos);

protected:
	Direction m_Dir = Direction::DOWN;
	cocos2d::Sprite* m_Sprite = nullptr;

	//중점을 기준으로 왼쪽,위까지 거리, 오른쪽,아래까지 거리 저장.(x,y,width,height 의미로 사용하지 않음)
	cocos2d::Rect m_MoveBox;
	cocos2d::Rect m_HitBox;
};