#pragma once
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

	void stop(float dTime);
	void move(float dTime);
	void summon(float dTime);
	
	void changeDirection(Direction dir) override;

	void force(float fx, float fy) override;

	float getHp() const { return m_Hp; }
	float getMaxHp() const { return 200.0f; }

	CREATE_FUNC(Tiger);

private:
	enum State
	{
		STOP,
		MOVE,
		SUMMON,
	};

	State m_State = STOP;

	float m_Time = 0.0f;
	float m_Hp = 200.0f;
	bool m_IsSummon = false;
	bool m_Invincible = false;
	bool m_IsDie = false;
	bool m_IsFocus = false;
	cocos2d::Point m_Dest;
};