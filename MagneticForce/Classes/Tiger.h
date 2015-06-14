﻿#pragma once
#include "Unit.h"

class Tiger : public Unit
{
public:
	Tiger() = default;
	~Tiger() override = default;

	bool init();

	void update(float dTime);

	void collision(float power) override;
	void collision(Unit* unit, float power) override;

	void hurt(float hp) override;
	void invincible(float time);
	void die();

	CREATE_FUNC(Tiger);

private:
	float m_Hp = 20.0f;
	bool m_Invincible = false;
	bool m_IsDie = false;
	bool m_IsFocus = false;
};