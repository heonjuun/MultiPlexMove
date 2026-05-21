#include "pch.h"
#include "S2C_login.h"


void S2C_login::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());
	Message = JSONDocument["Message"].GetString();	
}

std::string S2C_login::ToString()
{
	JSONDocument.SetObject();
	JSONDocument.AddMember("Message", Message, JSONDocument.GetAllocator());

	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();
}



