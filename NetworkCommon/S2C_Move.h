#pragma once
#include "Packet.h"
class S2C_Move :public Packet
{
public:
	SOCKET ClientSocket;
	int X;
	int Y;
	void Parse(std::string InString) override;
	std::string ToString() override;
};

