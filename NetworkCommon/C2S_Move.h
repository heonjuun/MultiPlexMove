#pragma once
#include"Packet.h"
class C2S_Move:public Packet
{
public:
	SOCKET ClientSocket;
	std::string Direction;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

