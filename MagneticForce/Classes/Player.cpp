﻿#include "Player.h"
#include "GameScene.h"
#include "TileMap.h"
#include "Effect.h"

USING_NS_CC;

bool Player::init()
{
	if (!Unit::init())
	{
		return false;
	}

	m_Dir = Direction::UP;
	changeSprite("player_up2", false);
	setState(IDLE);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	scheduleUpdate();
	m_Status = DataManager::getInstance()->getStatus();

	m_MoveBox = cocos2d::Rect(-6, -18, 7, -11);
	m_HitBox = cocos2d::Rect(-6, -18, 7, 14);
	m_Friction = 250.0f;

	DataManager::getInstance()->initPlayer(this);

	

	return true;
}

void Player::setState(State state)
{
	switch (state)
	{
	case IDLE:
		switch (m_Dir)
		{
		case Direction::UP:
			changeSprite("player_up2", false);
			break;
		case Direction::RIGHT:
			changeSprite("player_right2", false);
			break;
		case Direction::DOWN:
			changeSprite("player_down2", false);
			break;
		case Direction::LEFT:
			changeSprite("player_left2", false);
			break;
		}
		break;
	case MOVE:
		switch (m_Dir)
		{
		case Direction::UP:
			changeSprite("player_up", true);
			break;
		case Direction::RIGHT:
			changeSprite("player_right", true);
			break;
		case Direction::DOWN:
			changeSprite("player_down", true);
			break;
		case Direction::LEFT:
			changeSprite("player_left", true);
			break;
		}
		break;
	case ACT:
		DataManager::getInstance()->playEffect("pull_push");
		changeSpriteByType(m_Type);
		if (m_Drone == 0)
		{
			m_Drone = DataManager::getInstance()->playEffect("drone", true);
		}
		else
		{
			DataManager::getInstance()->resumeEffect(m_Drone);
		}
		break;
	}

	if (state != ACT && m_Drone != 0)
	{
		DataManager::getInstance()->pauseEffect(m_Drone);
	}
	
	m_State = state;
}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	switch(keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_ArrowPressed |= static_cast<int>(Direction::LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		m_ArrowPressed |= static_cast<int>(Direction::UP);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_ArrowPressed |= static_cast<int>(Direction::RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		m_ArrowPressed |= static_cast<int>(Direction::DOWN);
		break;
	//a키 누르면 인력 발동
	case EventKeyboard::KeyCode::KEY_A:
		if (m_Status->m_EnablePull)
		{
			m_Type = PULL;
			setState(ACT);
		}
		break;
	//s키 누르면 척력 발동
	case EventKeyboard::KeyCode::KEY_S:
		if (m_Status->m_EnablePush)
		{
			m_Type = PUSH;
			setState(ACT);
		}
		break;
	}
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_ArrowPressed &= ~static_cast<int>(Direction::LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		m_ArrowPressed &= ~static_cast<int>(Direction::UP);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_ArrowPressed &= ~static_cast<int>(Direction::RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		m_ArrowPressed &= ~static_cast<int>(Direction::DOWN);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (m_State == ACT && m_Type == PULL)
		{
			setState(IDLE);
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (m_State == ACT && m_Type == PUSH)
		{
			setState(IDLE);
		}
		break;
	}
}

void Player::changeDirection(Direction dir)
{
	auto prevDir = m_Dir;
	Unit::changeDirection(dir);

	if (m_State == IDLE || (m_State == MOVE && prevDir != m_Dir))
	{
		setState(MOVE);
	}
}

void Player::update(float dTime)
{
	//기본 update
	Unit::update(dTime);

	GameScene* scene = static_cast<GameScene*>(getParent());
	scene->setFocus(getPositionX(), getPositionY());


	if (m_State != ACT)
	{
		if (m_Status->m_Mp < m_Status->m_MaxMp - 2.0f*dTime)
		{
			m_Status->m_Mp += 2.0f*dTime;
		}
		else
		{
			m_Status->m_Mp = m_Status->m_MaxMp;
		}

		if (m_ArrowPressed == 0)
		{
			setState(IDLE);
		}
		else if ((m_ArrowPressed & static_cast<int>(Direction::UP)) != 0)
		{
			changeDirection(Direction::UP);
		}
		else if ((m_ArrowPressed & static_cast<int>(Direction::RIGHT)) != 0)
		{
			changeDirection(Direction::RIGHT);
		}
		else if ((m_ArrowPressed & static_cast<int>(Direction::DOWN)) != 0)
		{
			changeDirection(Direction::DOWN);
		}
		else
		{
			changeDirection(Direction::LEFT);
		}
	}

	if (m_State == MOVE)
	{
		//힘이 가해지고 있는 상태면 idle 상태로 변경. 이동 불가.
		if (m_Force.x > 0.5f || m_Force.x < -0.5f ||
			m_Force.y > 0.5f || m_Force.y < -0.5f)
		{
			setState(IDLE);
			return;
		}

		Point nextPos = getPosition();

		switch (m_Dir)
		{
		case Direction::UP:
			nextPos.setPoint(getPositionX(), getPositionY() + m_Status->m_Speed*dTime);
			break;
		case Direction::RIGHT:
			nextPos.setPoint(getPositionX() + m_Status->m_Speed*dTime, getPositionY());
			break;
		case Direction::DOWN:
			nextPos.setPoint(getPositionX(), getPositionY() - m_Status->m_Speed*dTime);
			break;
		case Direction::LEFT:
			nextPos.setPoint(getPositionX() - m_Status->m_Speed*dTime, getPositionY());
			break;
		}

		if (!moveCheck(nextPos))
		{
			setPosition(nextPos);
		}
	}
	else if (m_State == ACT)
	{
		if (m_Status->m_Mp < 5.0f*dTime)
		{
			setState(IDLE);
		}
		switch (m_Type)
		{
		case PULL:
			pull(dTime);
			m_Status->m_Mp -= 5.0f*dTime;
			break;
		case PUSH:
			push(dTime);
			m_Status->m_Mp -= 5.0f*dTime;
			break;
		}
	}
}

void Player::setSpeed(float speed)
{
	m_Status->m_Speed = speed;
}

void Player::pull(float dTime)
{
	cocos2d::Vector<Unit*> units;
	GameScene* scene = static_cast<GameScene*>(getParent());
	auto myPos = getPosition();
	float fx = 0.0f, fy = 0.0f;

	switch (m_Dir)
	{
	case Direction::LEFT:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto xCut = myPos.x - 24.0f;

			if (((realHit.origin.y < myPos.y + 24.0f && realHit.origin.y > myPos.y - 24.0f) ||
				(realHit.origin.y + hitBox.size.height < myPos.y + 24.0f && realHit.origin.y + hitBox.size.height > myPos.y - 24.0f)) &&
				realHit.origin.x + hitBox.size.width < xCut)
				return true;

			return false;

		}, units);
		fx = m_Status->m_PullPower;
		break;
	case Direction::UP:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto yCut = myPos.y + 24.0f;

			if (((realHit.origin.x < myPos.x + 24.0f && realHit.origin.x > myPos.x - 24.0f) ||
				(realHit.origin.x + hitBox.size.width < myPos.x + 24.0f && realHit.origin.x + hitBox.size.width > myPos.x - 24.0f)) &&
				realHit.origin.y > yCut)
				return true;

			return false;

		}, units);
		fy = -m_Status->m_PullPower;
		break;
	case Direction::RIGHT:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto xCut = myPos.x + 24.0f;

			if (((realHit.origin.y < myPos.y + 24.0f && realHit.origin.y > myPos.y - 24.0f) ||
				(realHit.origin.y + hitBox.size.height < myPos.y + 24.0f && realHit.origin.y + hitBox.size.height > myPos.y - 24.0f)) &&
				realHit.origin.x > xCut)
				return true;

			return false;

		}, units);
		fx = -m_Status->m_PullPower;
		break;
	case Direction::DOWN:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto yCut = myPos.y - 24.0f;

			if (((realHit.origin.x < myPos.x + 24.0f && realHit.origin.x > myPos.x - 24.0f) ||
				(realHit.origin.x + hitBox.size.width < myPos.x + 24.0f && realHit.origin.x + hitBox.size.width > myPos.x - 24.0f)) &&
				realHit.origin.y + hitBox.size.height < yCut)
				return true;

			return false;

		}, units);
		fy = m_Status->m_PullPower;
		break;
	}

	for (auto u : units)
	{
		auto dist = myPos.getDistance(u->getPosition());

		u->force(fx*dTime / dist, fy*dTime / dist);
	}
}

void Player::push(float dTime)
{
	cocos2d::Vector<Unit*> units;
	GameScene* scene = static_cast<GameScene*>(getParent());
	auto myPos = getPosition();
	float fx = 0.0f, fy = 0.0f;

	switch (m_Dir)
	{
	case Direction::LEFT:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto xCut = myPos.x - 24.0f;

			if (((realHit.origin.y < myPos.y + 24.0f && realHit.origin.y > myPos.y - 24.0f) ||
				(realHit.origin.y + hitBox.size.height < myPos.y + 24.0f && realHit.origin.y + hitBox.size.height > myPos.y - 24.0f)) &&
				realHit.origin.x + hitBox.size.width < xCut)
				return true;

			return false;

		}, units);
		fx = -m_Status->m_PushPower;
		break;
	case Direction::UP:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto yCut = myPos.y + 24.0f;

			if (((realHit.origin.x < myPos.x + 24.0f && realHit.origin.x > myPos.x - 24.0f) ||
				(realHit.origin.x + hitBox.size.width < myPos.x + 24.0f && realHit.origin.x + hitBox.size.width > myPos.x - 24.0f)) &&
				realHit.origin.y > yCut)
				return true;

			return false;

		}, units);
		fy = m_Status->m_PushPower;
		break;
	case Direction::RIGHT:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto xCut = myPos.x + 24.0f;

			if (((realHit.origin.y < myPos.y + 24.0f && realHit.origin.y > myPos.y - 24.0f) ||
				(realHit.origin.y + hitBox.size.height < myPos.y + 24.0f && realHit.origin.y + hitBox.size.height > myPos.y - 24.0f)) &&
				realHit.origin.x > xCut)
				return true;

			return false;

		}, units);
		fx = m_Status->m_PushPower;
		break;
	case Direction::DOWN:
		scene->conditionCheck([myPos](Unit* unit) -> bool
		{
			auto pos = unit->getPosition();
			auto hitBox = unit->getHitBox();

			cocos2d::Rect realHit(pos.x + hitBox.origin.x, pos.y + hitBox.origin.y,
				hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

			auto yCut = myPos.y - 24.0f;

			if (((realHit.origin.x < myPos.x + 24.0f && realHit.origin.x > myPos.x - 24.0f) ||
				(realHit.origin.x + hitBox.size.width < myPos.x + 24.0f && realHit.origin.x + hitBox.size.width > myPos.x - 24.0f)) &&
				realHit.origin.y + hitBox.size.height < yCut)
				return true;

			return false;

		}, units);
		fy = -m_Status->m_PushPower;
		break;
	}

	for (auto u : units)
	{
		auto dist = myPos.getDistance(u->getPosition());

		u->force(fx*dTime / dist, fy*dTime / dist);
	}
}

void Player::collision(float power)
{
	hurt(power / 10.0f);
}

void Player::collision(Unit* unit, float power)
{
	hurt(power / 10.0f);
}

void Player::changeSpriteByType(ActType type)
{
	switch (type)
	{
	case PULL:
		switch (m_Dir)
		{
		case Direction::UP:
			changeSprite("player_act_pull_up", true);
			break;
		case Direction::RIGHT:
			changeSprite("player_act_pull_right", true);
			break;
		case Direction::DOWN:
			changeSprite("player_act_pull_down", true);
			break;
		case Direction::LEFT:
			changeSprite("player_act_pull_left", true);
			break;
		}
		break;
	case PUSH:
		switch (m_Dir)
		{
		case Direction::UP:
			changeSprite("player_act_push_up", true);
			break;
		case Direction::RIGHT:
			changeSprite("player_act_push_right", true);
			break;
		case Direction::DOWN:
			changeSprite("player_act_push_down", true);
			break;
		case Direction::LEFT:
			changeSprite("player_act_push_left", true);
			break;
		}
		break;
	}
}

void Player::hurt(float hp)
{
	//무적이면 뎀 무시
	if (m_Invincible)
		return;

	auto e = Effect::create("collision_effect", "player_hurt", getPositionX(), getPositionY(),
		[](Effect* e)
	{
		e->removeFromParent();
	});

	m_Status->m_Hp -= hp;

	//1초간 무적
	invincible(1.0f);
}

void Player::invincible(float time)
{
	m_Invincible = true;

	auto action = Blink::create(time, 3 * time);
	auto sequence = Sequence::create(action, CallFunc::create([this](){m_Invincible = false; }), nullptr);

	runAction(sequence);
}


