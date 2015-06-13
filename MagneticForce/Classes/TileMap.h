#pragma once
#include "cocos2d.h"

class TileMap : public cocos2d::Node
{
public:
	CREATE_FUNC(TileMap);

	TileMap() = default;
	~TileMap() override = default;

	bool init() override;

private:
};