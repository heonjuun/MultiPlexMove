#pragma once
#include"Packet.h"
class PositionPacket:public Packet
{
public:
	std::string UserID;
	int PositionX;
	int PositionY;
	void Parse(std::string InString) override;
	std::string ToString() override;
	int Length() override;

	void SetPosition(std::string InUserID,int InX,int InY);

	std::string GetUserID() const;
	int GetX() const;
	int GetY() const;

	
	

};

