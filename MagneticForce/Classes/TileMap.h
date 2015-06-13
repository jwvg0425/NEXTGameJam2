#pragma once
#include "cocos2d.h"
#include "DataManager.h"

class TileMap : public cocos2d::Node
{
public:

	TileMap() = default;
	~TileMap() override = default;

	bool initWithFile(const std::string& fileName);

	static TileMap* createWithFile(const std::string& fileName);

	bool isSolidTile(int x, int y);
	int getTile(int layer, int x, int y, int out = -1);

private:
	TileType getTileType(TileType pos, const std::array<bool, 8>& surrond);
	int m_Width;
	int m_Height;
	int m_Layer;
	std::vector<bool> m_SolidMap;
	std::vector<std::vector<int> > m_TileMap;
	std::map<int, std::string> m_Tiles;
};