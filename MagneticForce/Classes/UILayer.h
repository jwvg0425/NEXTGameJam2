#pragma once
#include "cocos2d.h"

class UILayer : public cocos2d::Layer
{
public:
	UILayer() = default;
	~UILayer() override = default;

	bool init() override;

	CREATE_FUNC(UILayer);

	void update(float dTime);

private:
	cocos2d::Sprite* m_HP;
	cocos2d::Sprite* m_MP;
	cocos2d::Sprite* m_Pull;
	cocos2d::Sprite* m_Push;

	float m_ToggleEffect = 0.0f;
};