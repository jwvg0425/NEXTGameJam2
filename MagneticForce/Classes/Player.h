#pragma once
#include "Unit.h"

struct PlayerStatus
{
	float m_Speed = 200.0f;
	float m_MaxMp = 100.0f;
	float m_Mp = 100.0f;
	float m_PullPower = 20000.0f;
	float m_Hp = 100.0f;
	float m_MaxHp = 100.0f;
	float m_PushPower = 15000.0f;
	bool m_EnablePush = true;
	bool m_EnablePull = true;
	bool m_EnablePullCry = true;
	bool m_EnablePushCry = true;
};

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
		PULL_CRY,
		PUSH_CRY
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


	float getMaxMp() const { return m_Status->m_MaxMp; }
	float getMp() const { return m_Status->m_Mp; }

	void setMaxMp(float maxMp) { m_Status->m_MaxMp = maxMp; }
	void setMp(float mp) { m_Status->m_Mp = mp; }

	float getMaxHp() const { return m_Status->m_MaxHp; }
	float getHp() const { return m_Status->m_Hp; }

	void setMaxHp(float maxHp) { m_Status->m_MaxHp = maxHp; }
	void setHp(float hp) { m_Status->m_Hp = hp; }

	void pull(float dTime);
	void push(float dTime);

	void pullCry(float dTime);
	void pushCry(float dTime);

	void collision(float power) override;
	void collision(Unit* unit, float power) override;

	bool isEnablePush() const { return m_Status->m_EnablePush; }
	bool isEnablePull() const { return m_Status->m_EnablePull; }
	bool isEnablePushCry() const { return m_Status->m_EnablePushCry; }
	bool isEnablePullCry() const { return m_Status->m_EnablePullCry; }

	State getState() const { return m_State; }
	ActType getActType() const { return m_Type; }

	void hurt(float hp) override;
	void invincible(float time);

private:
	void changeSpriteByType(ActType type);

	State m_State;
	ActType m_Type = NONE;
	int m_ArrowPressed = 0;
	PlayerStatus* m_Status = nullptr;
	unsigned int m_Drone = 0;
	bool m_Invincible = false;
	float m_CryTime = 0.0f;
};