#include "pch.h"
#include "ChatPacket.h"
#include<sstream>
//{
//	"UserID":"ninja1",
//	"Message":"hello?",
//
//}

//	// 2. Modify it by DOM.
//Value& s = d["stars"];
//s.SetInt(s.GetInt() + 1);
//cout << d["name"].GetString() << std::endl;
//d["name"] = "ภืบาบา";
//// 3. Stringify the DOM
//StringBuffer buffer;
//Writer<StringBuffer> writer(buffer);
//d.Accept(writer);
//
//// Output {"project":"rapidjson","stars":11}
//std::cout << buffer.GetString() << std::endl;
void ChatPacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	UserID = JSONDocument["UserID"].GetString();
	Message = JSONDocument["Message"].GetString();
	Gold = JSONDocument["Gold"].GetInt();
}

std::string ChatPacket::ToString()
{

	std::stringstream Stream;

	//Stream << "{";
	//Stream << "\"UserID\" :";
	//Stream << UserID;
	//Stream << "\"Message\" :";
	//Stream << Message;
	//Stream << "\"Gold\" :";
	//Stream << Gold;
	//Stream << "}";

	JSONDocument.SetObject();
	JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
	JSONDocument.AddMember("Message", Message, JSONDocument.GetAllocator());
	JSONDocument.AddMember("Gold", Gold, JSONDocument.GetAllocator());
	
	//JSONDocument["Message"] = Message;
	//JSONDocument["Gold"] = Gold;

	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();

}



int ChatPacket::Length()
{
	return (int)ToString().length();
}
