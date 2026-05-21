#pragma once
#include"Packet.h"
class S2C_Spawn:public Packet
{
public:
	SOCKET ClientSocket;//¼ıÀÚ
	int X;
	int Y;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

