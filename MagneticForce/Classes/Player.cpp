#include "Player.h"
#include "GameScene.h"
#include "TileMap.h"

USING_NS_CC;

bool Player::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("player_down2", false);
	setState(IDLE);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	scheduleUpdate();

	m_MoveBox = cocos2d::Rect(-14, -18, 7, -11);
	m_HitBox = cocos2d::Rect(-14, -18, 7, 14);

	return true;
}

void Player::setState(State state)
{
	m_State = state;
}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	switch(keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		changeDirection(Direction::LEFT);
		m_ArrowPressed |= static_cast<int>(Direction::LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		changeDirection(Direction::UP);
		m_ArrowPressed |= static_cast<int>(Direction::UP);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		changeDirection(Direction::RIGHT);
		m_ArrowPressed |= static_cast<int>(Direction::RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		changeDirection(Direction::DOWN);
		m_ArrowPressed |= static_cast<int>(Direction::DOWN);
		break;
	}

	setState(MOVE);
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
	}

	if (m_ArrowPressed == 0)
	{
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
		setState(IDLE);
	}
	else
	{
		//상,우,하,좌 순 우선순위 있음
		if ((m_ArrowPressed & static_cast<int>(Direction::UP)) != 0)
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
}

void Player::changeDirection(Direction dir)
{
	if (m_State == IDLE || m_Dir != dir)
	{
		switch (dir)
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
	}

	Unit::changeDirection(dir);
}

void Player::update(float dTime)
{
	//기본 update
	Unit::update(dTime);

	if (m_State == MOVE)
	{
		Point nextPos = getPosition();

		switch (m_Dir)
		{
		case Direction::UP:
			nextPos.setPoint(getPositionX(), getPositionY() + m_Speed*dTime);
			break;
		case Direction::RIGHT:
			nextPos.setPoint(getPositionX() + m_Speed*dTime, getPositionY());
			break;
		case Direction::DOWN:
			nextPos.setPoint(getPositionX(), getPositionY() - m_Speed*dTime);
			break;
		case Direction::LEFT:
			nextPos.setPoint(getPositionX() - m_Speed*dTime, getPositionY());
			break;
		}

		if (!solidCheck(nextPos))
		{
			setPosition(nextPos);
		}
	}

	GameScene* scene = static_cast<GameScene*>(getParent());

	scene->setFocus(getPositionX(), getPositionY());
}

void Player::setSpeed(float speed)
{
	m_Speed = speed;
}
