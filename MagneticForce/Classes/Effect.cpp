#include "Effect.h"
#include "DataManager.h"
#include "GameScene.h"

USING_NS_CC;

bool Effect::init(const std::string& file, std::function<void(Effect*)> endRoutine)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	m_EndRoutine = endRoutine;

	auto sprite = Sprite::create();
	sprite->getTexture()->setAliasTexParameters();
	
	auto animation = DataManager::getInstance()->getAnimation(file);
	auto animate = Animate::create(animation);
	auto action = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(Effect::endAnimation, this)), nullptr);

	sprite->runAction(action);

	addChild(sprite);

	return true;
}

Effect* Effect::create(const std::string& file, float x, float y, std::function<void(Effect*)> endRoutine)
{
	Effect* ret = new Effect();

	if (ret != nullptr && ret->init(file, endRoutine))
	{
		ret->autorelease();

		//현재 동작하고 있는 씬에 때려박음
		DataManager::getInstance()->getNowScene()->addChild(ret);
		ret->setPosition(x , y );
		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;

		return ret;
	}
}

void Effect::endAnimation()
{
	m_EndRoutine(this);
}
