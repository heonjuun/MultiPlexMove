#pragma once

struct PlayerRenderState
{
	int X;
	int Y;
};
class GameMap
{
public:
	GameMap();
	int width;
	int height;
	void SetPosition(std::string Player,int InX,int InY);
	void Draw();

	std::map<std::string, PlayerRenderState> Players;

	

	
};


