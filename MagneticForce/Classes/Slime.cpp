#include "slime.h"
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
	m_Friction = 200.0f;

	scheduleUpdate();

	return true;
}

void Slime::update(float dTime)
{
	Unit::update(dTime);
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

	auto e = Effect::create("collision_effect", "", getPositionX(), getPositionY(),
		[](Effect* e)
	{
		e->removeFromParent();
	});

	m_Hp -= hp;
	CCLOG("%f", m_Hp);
	if (m_Hp <= 0.0f)
	{
		die();
	}

	//1초간 무적
	invincible(1.0f);
}

void Slime::invincible(float time)
{
	m_Invincible = true;

	auto action = Blink::create(time, 3 * time);
	auto sequence = Sequence::create(action, CallFunc::create([this](){m_Invincible = false; }), nullptr);

	runAction(sequence);
}

void Slime::die()
{
	removeFromParent();
}
