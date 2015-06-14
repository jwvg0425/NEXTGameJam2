#pragma once
#include "cocos2d.h"
#include <array>

enum class TileType
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
	UPLEFT_CORNER = 9,
	UPRIGHT_CORNER = 10,
	DOWNRIGHT_CORNER = 11,
	DOWNLEFT_CORNER = 12
};

enum TileProperty
{
	NONE = 0x0,
	SOLID = 0x1,
	TILE = 0x2, //TILE 속성이 없으면 1칸에 1개 블록 다 들어감
};

class Unit;
class Player;

class DataManager
{
public:
	static DataManager* getInstance();
	cocos2d::Animation* getAnimation(const std::string& fileName);
	cocos2d::Sprite*	getTile(const std::string& fileName, TileType type);
	cocos2d::Sprite*	getSprite(const std::string& fileName);
	void				initTile();
	void				initSound();
	int					getTileProperty(const std::string& tileName);
	Unit*				getObject(const std::string& objName);

	void				initPlayer(Player* player) { m_Player = player; }
	Player*				getPlayer() const { return m_Player; }

	int					playEffect(const std::string& name);

private:
	DataManager();
	~DataManager();

	cocos2d::Map<std::string, cocos2d::Animation*> m_Animations;
	std::map<std::string, int> m_TileProperties;
	Player* m_Player = nullptr;
};