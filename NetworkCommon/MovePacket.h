#pragma once
#include"Packet.h"
class MovePacket :public Packet
{
public:
	char dir;
	void Parse(std::string InString) override;
	std::string ToString() override;
	int Length() override;

	char GetInput() const;

	
};

