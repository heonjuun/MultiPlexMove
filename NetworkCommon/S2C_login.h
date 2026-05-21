#pragma once
#include"Packet.h"
class S2C_login:public Packet
{
public:
	std::string Message;
	void Parse(std::string InString) override;
	std::string ToString() override;
};

