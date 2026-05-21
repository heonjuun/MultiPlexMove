#include "pch.h"
#include "MovePacket.h"
#include<sstream>

void MovePacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());
	const char *DirString = JSONDocument["dir"].GetString();
	if (DirString[0] != '\0')
	{
		dir = DirString[0];
	}
	
}

std::string MovePacket::ToString()
{
	JSONDocument.SetObject();

	rapidjson::Document::AllocatorType& Allocator = JSONDocument.GetAllocator();
	char DirString[2] = { dir, '\0' };

	rapidjson::Value DirValue;
	DirValue.SetString(DirString, Allocator);

	JSONDocument.AddMember("dir", DirValue,JSONDocument.GetAllocator());


	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);
	return Buffer.GetString();
}

int MovePacket::Length()
{
	return (int)ToString().length();
}

char MovePacket::GetInput() const
{
	return dir;
}



