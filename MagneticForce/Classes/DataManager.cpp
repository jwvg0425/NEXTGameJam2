#include "DataManager.h"
#include "Player.h"
#include "box.h"
#include <fstream>
#include "SimpleAudioEngine.h" 
#include "Portal.h"
#include "GameScene.h"
#include "Thorn.h"
#include "Slime.h"

using namespace CocosDenshion;

USING_NS_CC;

DataManager* DataManager::getInstance()
{
	static DataManager manager;

	return &manager;
}

DataManager::DataManager()
{
	m_Status = new PlayerStatus();
	initTile();
	initPortal();
}

DataManager::~DataManager()
{
	delete m_Status;
}

cocos2d::Animation* DataManager::getAnimation(const std::string& fileName)
{
	//이미 존재할 경우 존재하는 데이터 돌려줌
	auto it = m_Animations.find(fileName);
	if (it != m_Animations.end())
		return it->second;

	//그 외의 경우 생성후 저장, 리턴
	std::ifstream file("data/" + fileName + ".txt");

	if (!file.is_open())
	{
		CCLOG("%s.txt file is not existed.", fileName.c_str());
		return nullptr;
	}

	float delay;

	file >> delay;

	auto animation = Animation::create();
	animation->setDelayPerUnit(delay);

	while(!file.eof())
	{
		std::string name;
		int frameNum;
		file >> name >> frameNum;
		name = "graphics/" + name;

		for (int i = 0; i < frameNum; i++)
		{
			animation->addSpriteFrameWithFile(name);
			animation->getFrames().at(i)->getSpriteFrame()->getTexture()->setAliasTexParameters();
		}
	}

	m_Animations.insert(fileName, animation);

	return animation;
}

cocos2d::Sprite* DataManager::getTile(const std::string& fileName, TileType type)
{
	return getSprite(fileName + std::to_string(static_cast<int>(type)));
}

cocos2d::Sprite* DataManager::getSprite(const std::string& fileName)
{
	auto sprite = Sprite::create("graphics/" + fileName + ".png");
	sprite->getTexture()->setAliasTexParameters();

	return sprite;
}

void DataManager::initTile()
{
	std::ifstream file("data/tile.txt");

	if (!file.is_open())
	{
		CCLOG("tile.txt file is not existed.");
		return;
	}

	while (!file.eof())
	{
		std::string tile;
		int property;

		file >> tile >> property;

		m_TileProperties[tile] = property;
	}
}

int DataManager::getTileProperty(const std::string& tileName)
{
	return m_TileProperties[tileName];
}

Unit* DataManager::getObject(const std::string& objName)
{
	if (objName == "player")
	{
		return Player::create();
	}
	else if (objName == "box")
	{
		return Box::create();
	}
	else if (objName == "portal")
	{
		return Portal::create(m_NowScene->getMapName());
	}
	else if (objName == "thorn")
	{
		return Thorn::create();
	}
	else if (objName == "slime")
	{
		return Slime::create();
	}

	return nullptr;
}

void DataManager::initSound()
{
	std::ifstream file("data/sound.txt");

	if (!file.is_open())
	{
		CCLOG("sound.txt file is not existed.");
		return;
	}

	while (!file.eof())
	{
		std::string token;
		std::string name;

		file >> token >> name;

		if (token == "effect")
		{
			SimpleAudioEngine::getInstance()->preloadEffect(("sound/" + name).c_str());
		}
		else if (token == "background")
		{
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic(("sound/" + name).c_str());
		}
	}
}

int DataManager::playEffect(const std::string& name, bool loop)
{
	return SimpleAudioEngine::getInstance()->playEffect(("sound/" + name + ".wav").c_str(), loop);
}

void DataManager::playBackgroundMusic(const std::string& name, bool loop)
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic(("sound/" + name + ".wav").c_str(), loop);
}

void DataManager::resumeEffect(int id)
{
	SimpleAudioEngine::getInstance()->resumeEffect(id);
}

void DataManager::stopEffect(int id)
{
	SimpleAudioEngine::getInstance()->stopEffect(id);
}

void DataManager::pauseEffect(int id)
{
	SimpleAudioEngine::getInstance()->pauseEffect(id);
}

void DataManager::initPortal()
{
	std::ifstream file("data/portal.txt");

	if (!file.is_open())
	{
		CCLOG("tile.txt file is not existed.");
		return;
	}

	while (!file.eof())
	{
		std::string portal;
		std::string next;

		file >> portal >> next;

		m_Portals[portal] = next;
	}
}

std::string DataManager::getNextStage(const std::string& portal)
{
	return m_Portals[portal];
}
