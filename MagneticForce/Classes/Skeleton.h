﻿#pragma once
#include "Unit.h"

class Skeleton : public Unit
{
public:
	Skeleton() = default;
	~Skeleton() override = default;

	bool init();

	void update(float dTime);

	void collision(float power) override;
	void collision(Unit* unit, float power) override;

	void hurt(float hp) override;
	void invincible(float time);
	void die();

	CREATE_FUNC(Skeleton);

private:
	float m_Hp = 40.0f;
	bool m_Invincible = false;
	bool m_IsDie = false;
	bool m_IsFocus = false;
};