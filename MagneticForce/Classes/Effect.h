﻿#pragma once
#include "cocos2d.h"

class Effect : public cocos2d::Node
{
public:
	Effect() = default;
	~Effect() override = default;

	bool init(const std::string& file, std::function<void(Effect*)> endRoutine);

	static Effect* create(const std::string& file, float x, float y ,std::function<void(Effect*)> endRoutine);

	void endAnimation();

private:
	std::function<void(Effect*)> m_EndRoutine;
	
};