﻿#pragma once
#include "cocos2d.h"

class UILayer : public cocos2d::Layer
{
public:
	UILayer() = default;
	~UILayer() override = default;

	bool init() override;

	CREATE_FUNC(UILayer);

private:
	cocos2d::Sprite* m_HP;
	cocos2d::Sprite* m_MP;
};