#include "pch.h"
#include "PositionPacket.h"

void PositionPacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());
	UserID = JSONDocument["UserID"].GetString();
	PositionX = JSONDocument["PositionX"].GetInt();
	PositionY = JSONDocument["PositionY"].GetInt();
}

std::string PositionPacket::ToString()
{
	JSONDocument.SetObject();
	JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
	JSONDocument.AddMember("PositionX",PositionX,JSONDocument.GetAllocator());
	JSONDocument.AddMember("PositionY", PositionY, JSONDocument.GetAllocator());


	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();
}

int PositionPacket::Length()
{
	return (int)ToString().length();
}

void PositionPacket::SetPosition(std::string InUserID, int InX, int InY)
{
	UserID = InUserID;
	PositionX =InX;
	PositionY = InY;
}


std::string PositionPacket::GetUserID() const
{
	return std::string(UserID);
}


int PositionPacket::GetX() const
{
	return PositionX;
}

int PositionPacket::GetY() const
{
	return PositionY;
}



