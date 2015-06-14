#include "Skeleton.h"
#include "DataManager.h"
#include "Player.h"
#include "GameScene.h"
#include "Effect.h"

USING_NS_CC;

bool Skeleton::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("skeleton_down2", false);

	m_MoveBox = { -13, -13, 13, -7 };
	m_HitBox = { -13, -13, 13, 13 };
	m_Friction = 100.0f;
	m_Dir = Direction::NONE;

	scheduleUpdate();

	return true;
}

void Skeleton::update(float dTime)
{
	if (m_IsDie)
	{
		die();
		return;
	}

	Unit::update(dTime);

	//cry 중에는 안 움직임
	if (DataManager::getInstance()->getPlayer()->getActType() == Player::PULL_CRY ||
		DataManager::getInstance()->getPlayer()->getActType() == Player::PUSH_CRY)
	{
		return;
	}

	//플레이어가 일정 거리 안에 있으면 무조건 플레이어랑 일자 되는 방향으로 개돌함. 한 번 인식하면 계속 쫓아옴.
	if (m_IsFocus || DataManager::getInstance()->getPlayer()->getPosition().getDistance(getPosition()) < 200.0f)
	{
		m_IsMove = true;

		auto vect = DataManager::getInstance()->getPlayer()->getPosition() - getPosition();
		vect.normalize();

		auto angle = vect.getAngle() * 180.0f / 3.1415f;

		//right
		if (angle <45.0f && angle > -45.0f)
		{
			changeDirection(Direction::RIGHT);
		}
		else if (angle <135.0f && angle > 45.0f) //up
		{
			changeDirection(Direction::UP);
		}
		else if (angle > 135.0f || angle < -135.0f) //left
		{
			changeDirection(Direction::LEFT);
		}
		else if (angle < -45.0f && angle > -135.0f) //down
		{
			changeDirection(Direction::DOWN);
		}

		//속도만큼의 크기를 가지도록.
		vect *= 40.0f;

		auto nextPos = getPosition() + vect*dTime;

		GameScene* scene = static_cast<GameScene*>(getParent());
		if (!scene->moveCheck(this, nextPos))
		{
			setPosition(nextPos);
		}

		//공격 범위 안쪽이면 딜 입힘
		if (DataManager::getInstance()->getPlayer()->getPosition().getDistance(getPosition()) < 30.0f)
		{
			DataManager::getInstance()->getPlayer()->hurt(3.0f);
		}
		m_IsFocus = true;
	}
	else
	{
		m_IsMove = false;

		auto vect = DataManager::getInstance()->getPlayer()->getPosition() - getPosition();
		auto angle = vect.getAngle() * 180.0f / 3.1415f;

		//right
		if (angle <45.0f && angle > -45.0f)
		{
			changeDirection(Direction::RIGHT);
		}
		else if (angle <135.0f && angle > 45.0f) //up
		{
			changeDirection(Direction::UP);
		}
		else if (angle > 135.0f || angle < -135.0f) //left
		{
			changeDirection(Direction::LEFT);
		}
		else if (angle < -45.0f && angle > -135.0f) //down
		{
			changeDirection(Direction::DOWN);
		}
	}
}

void Skeleton::collision(float power)
{
	//지형과 부딪힐 땐 데미지 받지 않음.
}

void Skeleton::collision(Unit* unit, float power)
{
	//고체인 물체와 충돌시 타격

	if (unit->isSolid())
		hurt(power / 10.0f);
}

void Skeleton::hurt(float hp)
{
	//무적이면 뎀 무시
	if (m_Invincible)
		return;

	auto e = Effect::create("collision_effect", "skeleton", getPositionX(), getPositionY(),
		[](Effect* e)
	{
		e->removeFromParent();
	});

	m_Hp -= hp;
	if (m_Hp <= 0.0f)
	{
		m_IsDie = true;
	}
	else
	{
		//1초간 무적
		invincible(1.0f);
	}
}

void Skeleton::invincible(float time)
{
	m_Invincible = true;

	auto action = Blink::create(time, 3 * time);
	auto sequence = Sequence::create(action, CallFunc::create([this](){ m_Invincible = false; }), nullptr);

	runAction(sequence);
}

void Skeleton::die()
{
	//스탯 상승
	DataManager::getInstance()->getStatus()->m_MaxHp += 5.0f;
	DataManager::getInstance()->getStatus()->m_Hp += 5.0f;
	DataManager::getInstance()->getStatus()->m_MaxMp += 5.0f;
	DataManager::getInstance()->getStatus()->m_Mp += 5.0f;
	DataManager::getInstance()->getStatus()->m_MpRegen += 1.0f;
	DataManager::getInstance()->getStatus()->m_Speed += 5.0f;
	DataManager::getInstance()->getStatus()->m_PullPower += 1000.0f;
	DataManager::getInstance()->getStatus()->m_PushPower += 800.0f;
	DataManager::getInstance()->getNowScene()->removeUnit(this);
}

void Skeleton::force(float fx, float fy)
{
	//얘는 자력 영향을 받지 않는다.
}

void Skeleton::changeDirection(Direction dir)
{
	if (dir != m_Dir)
	{
		switch (dir)
		{
		case Direction::LEFT:
			if (m_IsMove)
			{
				changeSprite("skeleton_left", true);
			}
			else
			{
				changeSprite("skeleton_left2", false);
			}
			break;
		case Direction::UP:
			if (m_IsMove)
			{
				changeSprite("skeleton_up", true);
			}
			else
			{
				changeSprite("skeleton_up2", false);
			}
			break;
		case Direction::RIGHT:
			if (m_IsMove)
			{
				changeSprite("skeleton_right", true);
			}
			else
			{
				changeSprite("skeleton_right2", false);
			}
			break;
		case Direction::DOWN:
			if (m_IsMove)
			{
				changeSprite("skeleton_down", true);
			}
			else
			{
				changeSprite("skeleton_down2", false);
			}
			break;
		}
	}

	m_Dir = dir;
}
