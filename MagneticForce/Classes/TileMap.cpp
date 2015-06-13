#include "TileMap.h"
#include "Unit.h"
#include "GameScene.h"
#include <fstream>

USING_NS_CC;

bool TileMap::initWithFile(const std::string& fileName, GameScene* scene)
{
	if (!Node::init())
	{
		return false;
	}

	std::ifstream file("data/" + fileName + ".txt");

	if (!file.is_open())
	{
		return false;
	}

	bool defSection = true;
	bool objSection = false;

	while (!file.eof())
	{
		if (defSection)
		{
			std::string token;

			file >> token;

			if (token == "tile")
			{
				std::string tileName;
				int number;
				file >> tileName >> number;

				m_Tiles[number] = tileName;
			}
			else if (token == "size")
			{
				file >> m_Layer >> m_Width >> m_Height;
				m_SolidMap.resize(m_Width*m_Height);
				m_TileMap.resize(m_Layer);
				for (auto& map : m_TileMap)
				{
					map.resize(m_Width*m_Height);
				}

				defSection = false;
			}
		}
		else if(!objSection)
		{
			for (int l = 0; l < m_Layer; l++)
			{
				for (int i = 0; i < m_Width * m_Height; i++)
				{
					int tileNum;
					file >> tileNum;
					if (tileNum != -1)
					{
						m_SolidMap[i] = (m_SolidMap[i] || ((DataManager::getInstance()->getTileProperty(m_Tiles[tileNum]) & TileProperty::SOLID) != 0));
					}
					m_TileMap[l][i] = tileNum;
				}
			}
			objSection = true;
		}
		else
		{
			std::string object;
			int x, y;

			file >> object >> x >> y;

			auto unit = DataManager::getInstance()->getObject(object);
			scene->addUnit(unit);
			unit->setPosition(x * 48 + 24, (m_Height - y) * 48 - 24);
		}
	}

	//sprite 초기화 작업

	for (int layer = 0; layer < m_Layer; layer++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				int tile = getTile(layer, x, y);

				if (tile == -1)
					continue;

				//tile의 경우 한 칸에 대해 4개의 sub sprite 존재
				if ((DataManager::getInstance()->getTileProperty(m_Tiles[tile]) & TileProperty::TILE) != 0)
				{
					Sprite* upleft;
					Sprite* upright;
					Sprite* downleft;
					Sprite* downright;

					std::array<bool, 8> surround;

					surround[0] = (tile == getTile(layer, x - 1, y - 1, tile));
					surround[1] = (tile == getTile(layer, x, y - 1, tile));
					surround[2] = (tile == getTile(layer, x + 1, y - 1, tile));
					surround[3] = (tile == getTile(layer, x - 1, y, tile));
					surround[4] = (tile == getTile(layer, x + 1, y, tile));
					surround[5] = (tile == getTile(layer, x - 1, y + 1, tile));
					surround[6] = (tile == getTile(layer, x, y + 1, tile));
					surround[7] = (tile == getTile(layer, x + 1, y + 1, tile));

					upleft = DataManager::getInstance()->getTile(m_Tiles[tile], getTileType(TileType::UPLEFT, surround));
					upright = DataManager::getInstance()->getTile(m_Tiles[tile], getTileType(TileType::UPRIGHT, surround));
					downleft = DataManager::getInstance()->getTile(m_Tiles[tile], getTileType(TileType::DOWNLEFT, surround));
					downright = DataManager::getInstance()->getTile(m_Tiles[tile], getTileType(TileType::DOWNRIGHT, surround));

					upleft->setAnchorPoint({ 0.0f, 1.0f });
					upright->setAnchorPoint({ 0.0f, 1.0f });
					downleft->setAnchorPoint({ 0.0f, 1.0f });
					downright->setAnchorPoint({ 0.0f, 1.0f });

					upleft->setPosition(x * 48, (m_Height - y) * 48);
					upright->setPosition(x * 48 + 24, (m_Height - y) * 48);
					downleft->setPosition(x * 48, (m_Height - y) * 48 - 24);
					downright->setPosition(x * 48 + 24, (m_Height - y) * 48 - 24);

					addChild(upleft);
					addChild(upright);
					addChild(downleft);
					addChild(downright);
				}
				else
				{
					auto sprite = DataManager::getInstance()->getSprite(m_Tiles[tile]);

					sprite->setAnchorPoint({ 0.0f, 1.0f });
					sprite->setPosition(x * 48, (m_Height - y) * 48);

					addChild(sprite);
				}
			}
		}
	}

	return true;
}

TileMap* TileMap::createWithFile(const std::string& fileName, GameScene* scene)
{
	TileMap* ret = new TileMap();
	
	if (ret != nullptr && ret->initWithFile(fileName, scene))
	{
		ret->autorelease();

		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;

		return ret;
	}
}

bool TileMap::isSolidTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= m_Width || y >= m_Height)
	{
		return true;
	}

	return m_SolidMap[y*m_Width + x];
}

bool TileMap::isSolidTile(float x, float y, cocos2d::Rect size) const
{
	int cutXleft = (x + size.origin.x) / 48.0f;
	int cutXright = (x + size.size.width) / 48.0f;
	int cutYdown = m_Height - ((y + size.origin.y) / 48.0f);
	int cutYup = m_Height - ((y + size.size.height) / 48.0f);

	if (x + size.origin.x < 0 || x + size.size.width >= m_Width*48.0f ||
		y + size.origin.y < 0 || y + size.size.height >= m_Height*48.0f)
	{
		return true;
	}

	if (isSolidTile(cutXleft, cutYup) || isSolidTile(cutXleft, cutYdown) ||
		isSolidTile(cutXright, cutYup) || isSolidTile(cutXright, cutYdown))
	{
		return true;
	}

	return false;
}

int TileMap::getTile(int layer, int x, int y, int out /*= -1*/) const
{
	if (x < 0 || y < 0 || x >= m_Width || y >= m_Height)
	{
		return out;
	}
	else
	{
		return m_TileMap[layer][y*m_Width + x];
	}
}

TileType TileMap::getTileType(TileType pos, const std::array<bool, 8>& surround)
{
	TileType upleft[8] =
	{
		TileType::UPLEFT,
		TileType::UP,
		TileType::LEFT,
		TileType::UPLEFT_CORNER,
		TileType::UPLEFT,
		TileType::UP,
		TileType::LEFT,
		TileType::CENTER
	};

	TileType upright[8] =
	{
		TileType::UPRIGHT,
		TileType::UP,
		TileType::RIGHT,
		TileType::UPRIGHT_CORNER,
		TileType::UPRIGHT,
		TileType::UP,
		TileType::RIGHT,
		TileType::CENTER
	};

	TileType downleft[8] =
	{
		TileType::DOWNLEFT,
		TileType::DOWN,
		TileType::LEFT,
		TileType::DOWNLEFT_CORNER,
		TileType::DOWNLEFT,
		TileType::DOWN,
		TileType::LEFT,
		TileType::CENTER
	};

	TileType downright[8] =
	{
		TileType::DOWNRIGHT,
		TileType::DOWN,
		TileType::RIGHT,
		TileType::DOWNRIGHT_CORNER,
		TileType::DOWNRIGHT,
		TileType::DOWN,
		TileType::RIGHT,
		TileType::CENTER
	};

	if (pos == TileType::UPLEFT)
	{
		int type = 0;
		if (surround[0])
			type += 4;
		if (surround[1])
			type += 2;
		if (surround[3])
			type += 1;
		
		return upleft[type];
	}
	else if (pos == TileType::UPRIGHT)
	{
		int type = 0;
		if (surround[2])
			type += 4;
		if (surround[1])
			type += 2;
		if (surround[4])
			type += 1;

		return upright[type];
	}
	else if (pos == TileType::DOWNLEFT)
	{
		int type = 0;
		if (surround[5])
			type += 4;
		if (surround[6])
			type += 2;
		if (surround[3])
			type += 1;

		return downleft[type];
	}
	else
	{
		int type = 0;
		if (surround[7])
			type += 4;
		if (surround[6])
			type += 2;
		if (surround[4])
			type += 1;

		return downright[type];
	}
}
