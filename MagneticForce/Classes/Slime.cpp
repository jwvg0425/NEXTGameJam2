#include "slime.h"
#include "DataManager.h"
#include "GameScene.h"
#include "Player.h"
#include "Effect.h"

USING_NS_CC;

bool Slime::init()
{
	if (!Unit::init())
	{
		return false;
	}

	changeSprite("slime", true);

	m_MoveBox = { -10, -7, 11, -1 };
	m_HitBox = { -10, -7, 11, 4 };
	m_Friction = 100.0f;

	scheduleUpdate();

	return true;
}

void Slime::update(float dTime)
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
		auto vect = DataManager::getInstance()->getPlayer()->getPosition() - getPosition();
		vect.normalize();

		//속도만큼의 크기를 가지도록.
		vect *= 80.0f;

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
}

void Slime::collision(float power)
{
	hurt(power / 10.0f);
}

void Slime::collision(Unit* unit, float power)
{
	//고체인 물체와 충돌시 타격

	if (unit->isSolid())
		hurt(power / 10.0f);
}

void Slime::hurt(float hp)
{
	//무적이면 뎀 무시
	if (m_Invincible)
		return;

	auto e = Effect::create("collision_effect", "slime", getPositionX(), getPositionY(),
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

void Slime::invincible(float time)
{
	m_Invincible = true;

	auto action = Blink::create(time, 3 * time);
	auto sequence = Sequence::create(action, CallFunc::create([this](){ m_Invincible = false; }), nullptr);

	runAction(sequence);
}

void Slime::die()
{
	//스탯 상승
	DataManager::getInstance()->getStatus()->m_MaxHp += 10.0f;
	DataManager::getInstance()->getStatus()->m_Hp += 10.0f;
	DataManager::getInstance()->getStatus()->m_MaxMp += 3.0f;
	DataManager::getInstance()->getStatus()->m_Mp += 3.0f;
	DataManager::getInstance()->getStatus()->m_PullPower += 500.0f;
	DataManager::getInstance()->getStatus()->m_PushPower += 300.0f;
	DataManager::getInstance()->getNowScene()->removeUnit(this);
	removeFromParent();
}
