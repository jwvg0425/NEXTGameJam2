#include "Tiger.h"
#include "DataManager.h"
#include "Player.h"
#include "GameScene.h"
#include "Effect.h"
#include "Slime.h"
#include "Skeleton.h"

USING_NS_CC;

bool Tiger::init()
{
	if (!Unit::init())
	{
		return false;
	}

	DataManager::getInstance()->playBackgroundMusic("boss", true);

	changeSprite("tiger_down2", false);

	m_MoveBox = { -37, -37, 37, -22 };
	m_HitBox = { -37, -37, 37, 37 };
	m_Friction = 100.0f;

	scheduleUpdate();

	return true;
}

void Tiger::update(float dTime)
{
	Unit::update(dTime);

	m_Time += dTime;
	
	//5초마다 상태 변경
	if (m_Time > 5.0f)
	{
		m_Time = 0.0f;
		switch (m_State)
		{
		case STOP:
			m_Dest = DataManager::getInstance()->getPlayer()->getPosition();
			DataManager::getInstance()->playEffect("scream");
			m_State = MOVE;
			//sprite 변경용
			changeDirection(m_Dir);
			break;
		case MOVE: //move는 time과 무관
			m_Time = 0.0f;
			break;
		case SUMMON:
			m_State = STOP;
			//sprite 변경용
			changeDirection(m_Dir);
			break;
		}
	}

	switch (m_State)
	{
	case STOP:
		stop(dTime);
		break;
	case MOVE:
		move(dTime);
		break;
	case SUMMON:
		summon(dTime);
		break;
	}
}

void Tiger::collision(float power)
{
	//지형 충돌에는 데미지를 받지 않는다.
}

void Tiger::collision(Unit* unit, float power)
{
	//고체인 물체와 충돌시 타격

	if (unit->isSolid())
		hurt(power / 10.0f);
}

void Tiger::hurt(float hp)
{
	auto e = Effect::create("collision_effect", "", getPositionX() - 20+ rand() % 40, getPositionY() - 20 + rand() % 40,
		[](Effect* e)
	{
		e->removeFromParent();
	});

	m_Hp -= hp;

	if (m_Hp <= 0.0f)
	{
		m_IsDie = true;
	}
}

void Tiger::invincible(float time)
{
}

void Tiger::die()
{
	DataManager::getInstance()->getNowScene()->removeUnit(this);
	removeFromParent();
}

void Tiger::stop(float dTime)
{
	auto vect = DataManager::getInstance()->getPlayer()->getPosition() - getPosition();
	auto angle = vect.getAngle() * 180.0f / 3.1415f;

	
	if (angle <45.0f && angle > -45.0f) //right
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

void Tiger::move(float dTime)
{
	auto vect = m_Dest - getPosition();
	vect.normalize();

	//속도 200
	vect *= 200.0f;

	auto nextPos = getPosition() + vect*dTime;
	auto scene = DataManager::getInstance()->getNowScene();

	//가다가 뭔가에 부딪혀도 move 중지.
	if (scene->moveCheck(this, nextPos))
	{
		if (getPosition().getDistance(DataManager::getInstance()->getPlayer()->getPosition()) < 60.0f)
		{
			DataManager::getInstance()->getPlayer()->hurt(20.0f);
		}
		m_State = SUMMON;
		//sprite 변경용
		changeDirection(m_Dir);
		m_IsSummon = false;

		return;
	}

	setPosition(nextPos);

	//목적지로 추정되는 위치에 도착하면 중지. 중지한 시점에서 플레이어가 보스 근처라면 딜을 입음
	if (nextPos.getDistance(m_Dest) < 10.0f)
	{
		if (nextPos.getDistance(DataManager::getInstance()->getPlayer()->getPosition()) < 60.0f)
		{
			DataManager::getInstance()->getPlayer()->hurt(20.0f);
		}
		m_State = SUMMON;
		//sprite 변경용
		changeDirection(m_Dir);
		m_IsSummon = false;
	}
}

void Tiger::summon(float dTime)
{
	auto scene = DataManager::getInstance()->getNowScene();

	//필드에 몹 10마리 이하일 때만 소환
	if (!m_IsSummon && m_Time > 3.0f && scene->getUnitNum() < 20)
	{
		DataManager::getInstance()->playEffect("summon");

		//소환. 슬라임 3 or 스켈레톤 1.	
		if (rand() % 2)
		{
			//슬라임
			for (int i = 0; i < 3; i++)
			{
				auto slime = Slime::create();
				cocos2d::Point pos;
				cocos2d::Vector<Unit*> units;

				do
				{
					units.clear();
					pos.x = 192 + rand() % (768 - 192);
					pos.y = 144 + rand() % (576 - 144);
					scene->conditionCheck([pos](Unit* unit) -> bool
					{
						auto unitPos = unit->getPosition();
						auto hitBox = unit->getHitBox();
						cocos2d::Rect box = { pos.x - 24.0f, pos.y - 24.0f, 48.0f, 48.0f };

						cocos2d::Rect realHit(unitPos.x + hitBox.origin.x, unitPos.y + hitBox.origin.y,
							hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

						if (realHit.intersectsRect(box))
						{
							return true;
						}

						return false;

					}, units);
				} while (units.size() != 0);

				slime->setPosition(pos);
				slime->setFocus();
				scene->addUnit(slime);
			}
		}
		else
		{
			auto skeleton = Skeleton::create();
			cocos2d::Point pos;
			cocos2d::Vector<Unit*> units;

			do
			{
				units.clear();
				pos.x = 192 + rand() % (768 - 192);
				pos.y = 144 + rand() % (576 - 144);
				scene->conditionCheck([pos](Unit* unit) -> bool
				{
					auto unitPos = unit->getPosition();
					auto hitBox = unit->getHitBox();
					cocos2d::Rect box = { pos.x - 24.0f, pos.y - 24.0f, 48.0f, 48.0f };

					cocos2d::Rect realHit(unitPos.x + hitBox.origin.x, unitPos.y + hitBox.origin.y,
						hitBox.size.width - hitBox.origin.x, hitBox.size.height - hitBox.origin.y);

					if (realHit.intersectsRect(box))
					{
						return true;
					}

					return false;

				}, units);
			} while (units.size() != 0);

			skeleton->setPosition(pos);
			skeleton->setFocus();
			scene->addUnit(skeleton);
		}

		m_IsSummon = true;
	}
}

void Tiger::changeDirection(Direction dir)
{
	if (dir != m_Dir)
	{
		switch (dir)
		{
		case Direction::LEFT:
			if (m_State == MOVE)
			{
				changeSprite("tiger_left", true);
			}
			else
			{
				changeSprite("tiger_left2", false);
			}
			break;
		case Direction::UP:
			if (m_State == MOVE)
			{
				changeSprite("tiger_up", true);
			}
			else
			{
				changeSprite("tiger_up2", false);
			}
			break;
		case Direction::RIGHT:
			if (m_State == MOVE)
			{
				changeSprite("tiger_right", true);
			}
			else
			{
				changeSprite("tiger_right2", false);
			}
			break;
		case Direction::DOWN:
			if (m_State == MOVE)
			{
				changeSprite("tiger_down", true);
			}
			else
			{
				changeSprite("tiger_down2", false);
			}
			break;
		}
	}

	m_Dir = dir;
}

void Tiger::force(float fx, float fy)
{
	//얘는 자력 영향을 받지 않는다.
}
