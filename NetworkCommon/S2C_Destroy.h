#pragma once
#include"Packet.h"
class S2C_Destroy:public Packet
{
public:
	SOCKET ClientSocket;
	void Parse(std::string InString) override;
	std::string ToString() override;
};

