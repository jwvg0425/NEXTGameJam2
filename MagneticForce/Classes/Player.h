#pragma once
#include "Unit.h"

class Player : public Unit
{
public:
	enum State
	{
		IDLE,
		MOVE,
		ACT
	};

	enum ActType
	{
		NONE,
		PULL,
		PUSH,
	};

	Player() = default;
	~Player() override = default;

	bool init() override;

	CREATE_FUNC(Player);

	void setState(State state);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);

	void changeDirection(Direction dir) override;

	void update(float dTime);

	void setSpeed(float speed);


	float getMaxMp() const { return m_MaxMp; }
	float getMp() const { return m_Mp; }


	void setMaxMp(float maxMp) { m_MaxMp = maxMp; }
	void setMp(float mp) { m_Mp = mp; }

	void pull(float dTime);
	void push(float dTime);

	void collision(float power) override;
	void collision(const cocos2d::Vector<Unit*>& units, float power) override;

	bool isEnablePush() const { return m_EnablePush; }
	bool isEnablePull() const { return m_EnablePull; }

	State getState() const { return m_State; }
	ActType getActType() const { return m_Type; }

private:
	void changeSpriteByType(ActType type);

	State m_State;
	ActType m_Type = NONE;
	int m_ArrowPressed = 0;
	float m_Speed = 200.0f;
	float m_Dash = 400.0f;
	float m_MaxMp = 100.0f;
	float m_Mp = 100.0f;
	float m_PullPower = 20000.0f;
	bool m_EnablePush = true;
	bool m_EnablePull = true;
	unsigned int m_Drone = 0;
};