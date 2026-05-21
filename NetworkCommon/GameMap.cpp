#include "pch.h"
#include "GameMap.h"



GameMap::GameMap()
{
	width = 10;
	height = 10;
}


void GameMap::SetPosition(std::string Player, int InX, int InY)
{
	Players[Player] = { InX, InY };
}
void GameMap::Draw()
{
	system("cls");

	std::vector<std::string> Map;

	for (int y = 0; y < height; y++)
	{
		std::string Line;

		for (int x = 0; x < width; x++)
		{
			Line += '.';
		}

		Map.push_back(Line);
	}

	for (const auto& Pair : Players)
	{
		const std::string& PlayerID = Pair.first;
		const PlayerRenderState& State = Pair.second;

		if (State.X >= 0 && State.X < width &&
			State.Y >= 0 && State.Y < height)
		{
			char Symbol = 'P';

			if (!PlayerID.empty())
			{
				char LastChar = PlayerID[PlayerID.length() - 1];

				if (LastChar >= '0' && LastChar <= '9')
				{
					Symbol = LastChar;
				}
			}

			Map[State.Y][State.X] = Symbol;
		}
	}

	for (int y = 0; y < height; ++y)
	{
		std::cout << Map[y] << std::endl;
	}

	std::cout << std::endl;
	std::cout << "===== Player Position =====" << std::endl;

	for (const auto& Pair : Players)
	{
		const std::string& PlayerID = Pair.first;
		const PlayerRenderState& State = Pair.second;

		char Symbol = 'P';

		if (!PlayerID.empty())
		{
			char LastChar = PlayerID[PlayerID.length() - 1];

			if (LastChar >= '0' && LastChar <= '9')
			{
				Symbol = LastChar;
			}
		}

		std::cout << Symbol
			<< " / " << PlayerID
			<< " -> X: " << State.X
			<< ", Y: " << State.Y
			<< std::endl;
	}
}

