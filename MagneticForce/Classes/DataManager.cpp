#include "DataManager.h"
#include "Player.h"
#include "box.h"
#include <fstream>

USING_NS_CC;

DataManager* DataManager::getInstance()
{
	static DataManager manager;

	return &manager;
}

DataManager::DataManager()
{
	initTile();
}

DataManager::~DataManager()
{
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
		std::string fileName;
		int frameNum;
		file >> fileName >> frameNum;
		fileName = "graphics/" + fileName;

		for (int i = 0; i < frameNum; i++)
		{
			animation->addSpriteFrameWithFile(fileName);
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
	return Sprite::create("graphics/" + fileName + ".png");

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

	return nullptr;
}
