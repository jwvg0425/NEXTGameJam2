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
class GameScene;
class Tiger;

struct PlayerStatus;

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

	int					playEffect(const std::string& name, bool loop = false);
	void				resumeEffect(int id);
	void				pauseEffect(int id);
	void				stopEffect(int id);
	void				playBackgroundMusic(const std::string& name, bool loop = false);
	GameScene*			getNowScene() const { return m_NowScene; }
	void				setNowScene(GameScene* scene) { m_NowScene = scene; }
	PlayerStatus*		getStatus() { return m_Status; }

	void				initPortal();
	std::string			getNextStage(const std::string& portal);
	Tiger*				getBoss()const { return m_Boss; }
	void				setBoss(Tiger* boss){ m_Boss = boss; }

private:
	DataManager();
	~DataManager();

	cocos2d::Map<std::string, cocos2d::Animation*> m_Animations;
	std::map<std::string, int> m_TileProperties;
	std::map<std::string, std::string> m_Portals;
	Player* m_Player = nullptr;
	Tiger*	m_Boss = nullptr;
	GameScene* m_NowScene;
	PlayerStatus* m_Status;
};