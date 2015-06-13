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
	//dash 0.5초
	m_Friction = m_Dash * 2;

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
	}
	m_State = state;
}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	//힘이 가해지고 있는 중에는 아무 동작 못함. 적당히 0.5f정도로.

	if (m_Force.x > 0.5f || m_Force.x < -0.5f ||
		m_Force.y > 0.5f || m_Force.y < -0.5f)
	{
		return;
	}

	switch(keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		changeDirection(Direction::LEFT);
		m_ArrowPressed |= static_cast<int>(Direction::LEFT);
		setState(MOVE);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		changeDirection(Direction::UP);
		m_ArrowPressed |= static_cast<int>(Direction::UP);
		setState(MOVE);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		changeDirection(Direction::RIGHT);
		m_ArrowPressed |= static_cast<int>(Direction::RIGHT);
		setState(MOVE);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		changeDirection(Direction::DOWN);
		m_ArrowPressed |= static_cast<int>(Direction::DOWN);
		setState(MOVE);
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		if (m_Dir == Direction::DOWN)
		{
			force(0, -m_Dash);
		}
		else if (m_Dir == Direction::UP)
		{
			force(0, m_Dash);
		}
		else if (m_Dir == Direction::LEFT)
		{
			force(-m_Dash, 0);
		}
		else
		{
			force(m_Dash, 0);
		}
		m_ArrowPressed = 0;
		setState(IDLE);
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
	}

	if (m_ArrowPressed == 0)
	{
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
	if (m_State == MOVE || m_Dir != dir)
	{
		setState(MOVE);
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

		if (!moveCheck(nextPos))
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
