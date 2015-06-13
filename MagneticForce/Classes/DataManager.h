#pragma once
#include "cocos2d.h"
#include <array>

enum class Tile
{
	CENTER = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
	LEFT = 4,
	UPLEFT = 5,
	UPRIGHT = 6,
	DOWNRIGHT = 7,
	DOWNLEFT = 8,
	UPLEFT_CONRNER = 9,
	UPRIGHT_CORNER = 10,
	DOWNRIGHT_CORNER = 11,
	DOWNLEFT_CORNER = 12
};

class DataManager
{
public:
	static DataManager* getInstance();
	cocos2d::Animation* getAnimation(const std::string& fileName);

private:
	DataManager();
	~DataManager();

	cocos2d::Map<std::string, cocos2d::Animation*> m_Animations;
	cocos2d::Map<std::string, cocos2d::Sprite*> m_Tiles;
};