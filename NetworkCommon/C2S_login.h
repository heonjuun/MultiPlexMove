#pragma once
#include"Packet.h"
class C2S_login :public Packet
{
public:

	std::string UserID;
	std::string HashKey;

	//inherited via packet
	void Parse(std::string InString) override;
	std::string ToString() override;
};

