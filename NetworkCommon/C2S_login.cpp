#include "pch.h"
#include "C2S_login.h"

void C2S_login::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());
	UserID = JSONDocument["UserID"].GetString();
	HashKey= JSONDocument["HashKey"].GetString();
	
}

std::string C2S_login::ToString()
{
	JSONDocument.SetObject();
	JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
	JSONDocument.AddMember("HashKey", HashKey, JSONDocument.GetAllocator());


	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();
}
