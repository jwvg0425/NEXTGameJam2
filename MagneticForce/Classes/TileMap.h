#pragma once
#include "cocos2d.h"
#include "DataManager.h"

class TileMap : public cocos2d::Node
{
public:

	TileMap() = default;
	~TileMap() override = default;

	bool initWithFile(const std::string& fileName, cocos2d::Layer* layer);

	static TileMap* createWithFile(const std::string& fileName, cocos2d::Layer* layer);

	bool isSolidTile(int x, int y) const; // 칸 기반
	bool isSolidTile(float x, float y, cocos2d::Rect size) const; // 실 좌표 기반
	int getTile(int layer, int x, int y, int out = -1) const;

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }

private:
	TileType getTileType(TileType pos, const std::array<bool, 8>& surrond);
	int m_Width;
	int m_Height;
	int m_Layer;
	std::vector<bool> m_SolidMap;
	std::vector<std::vector<int> > m_TileMap;
	std::map<int, std::string> m_Tiles;
};