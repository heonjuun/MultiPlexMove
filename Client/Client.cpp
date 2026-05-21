#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include"ChatPacket.h"
#include"MovePacket.h"
#include"PositionPacket.h"
#include "NetUtil.h"
#include"GameMap.h"


#include<WinSock2.h>
#include<Windows.h>
#include<iostream>
#include<process.h>
#include<conio.h>





//using json = nlohmann::json;
//using namespace rapidjson;


#pragma comment(lib,"ws2_32")
#pragma comment(lib,"NetworkCommon")

using namespace std;

char SendBuffer[1024] = { 0, };
char RecvBuffer[1024] = { 0, };


bool IsSendThreadRunning = true;
bool IsRecvThreadRunning = true;
GameMap MapView;

unsigned WINAPI RecvThread(void* Argument)
{
	//소켓을 서버소켓으로 강제로 =(SOCKET*)Socket;
	//c언어는 문법을 강제하지않고 죽으면 내책임/ c++은 다이나믹 캐스트등 있음
	SOCKET ServerSocket = *(SOCKET*)Argument;
	while (IsRecvThreadRunning)
	{
		unsigned short PacketSize = 0;
		//1:1로 주고 받는다.
		int RecvBytes = recv(ServerSocket, (char*)&PacketSize, sizeof(PacketSize), MSG_WAITALL);
		if (RecvBytes <= 0)
		{
			cout << "recv fail." << endl;
			break;
		}
		PacketSize = ntohs(PacketSize);

		memset(RecvBuffer, 0, sizeof(RecvBuffer));
		//data JSON
		RecvBytes = recv(ServerSocket, RecvBuffer, PacketSize, MSG_WAITALL);
		if (RecvBytes <= 0)
		{
			cout << "recv fail " << endl;
			break;
		}

		//ChatPacket Data;
		PositionPacket Data;

		Data.Parse(RecvBuffer);
		MapView.SetPosition(
			Data.GetUserID(),
			Data.GetX(),
			Data.GetY()
		);

		MapView.Draw();

		cout << Data.UserID << " : " << Data.PositionX << " " << Data.PositionY << endl;
	}
	return 0;
}
unsigned WINAPI SendThread(void* Argument)
{
	SOCKET ServerSocket = *(SOCKET*)Argument;
	while (IsSendThreadRunning)
	{
		cin.getline(SendBuffer, sizeof(SendBuffer));

		ChatPacket Data;
		Data.UserID = "ninja";
		Data.Message = SendBuffer;
		Data.Gold = 1000;
		std::string JSONString = Data.ToString();

		unsigned short PacketSize = (unsigned short)JSONString.length();
		PacketSize = htons(PacketSize);

		//header
		int SentBytes = SendAll(ServerSocket, (char*)&PacketSize, 2);
		if (SentBytes <= 0)
		{
			cout << "header send fail." << endl;
			break;
		}

		//Data
		SentBytes = SendAll(ServerSocket, JSONString.c_str(), ntohs(PacketSize));
		if (SentBytes <= 0)
		{
			cout << "data send fail." << endl;
			break;
		}

	}

	return 0;
}

char PlayerInput()
{
	char Input = _getch();
	switch (Input)
	{
	case'w':
	case'W':
		return 'W';
	case's':
	case'S':
		return 'S';
	case 'a':
	case 'A':
		return 'A';
	case 'd':
	case 'D':
		return 'D';
	default:
		return 0;
	}
}

unsigned WINAPI MoveSendThread(void* Argument)
{
	SOCKET ServerSocket = *(SOCKET*)Argument;

	while (IsSendThreadRunning)
	{
		char Dir = PlayerInput();

		if (Dir == 0)
		{
			continue;
		}

		MovePacket Data;
		Data.dir = Dir;

		std::string JSONString = Data.ToString();

		cout << "input dir : " << Dir << endl;
		cout << "send json : " << JSONString << endl;

		unsigned short HostPacketSize = (unsigned short)JSONString.length();
		unsigned short NetworkPacketSize = htons(HostPacketSize);

		int SentBytes = SendAll(ServerSocket, (char*)&NetworkPacketSize, sizeof(NetworkPacketSize));
		if (SentBytes <= 0)
		{
			cout << "move header send fail, error : " << WSAGetLastError() << endl;
			break;
		}

		SentBytes = SendAll(ServerSocket, JSONString.c_str(), HostPacketSize);
		cout << "body sent bytes : " << SentBytes << endl;

		if (SentBytes <= 0)
		{
			cout << "move data send fail, error : " << WSAGetLastError() << endl;
			break;
		}
	}

	return 0;
}


int main()
{
	// 1. Parse a JSON string into DOM.
	//const char* json = R"(
	//				{"project":"rapidjson",
	//				"stars":10,
	//				"name":"김김김"
	//				}
	//					)";
	//Document d;
	//d.Parse(json);

	//// 2. Modify it by DOM.
	//Value& s = d["stars"];
	//s.SetInt(s.GetInt() + 1);
	//cout << d["name"].GetString() << std::endl;
	//d["name"] = "잉불불";
	//// 3. Stringify the DOM
	//StringBuffer buffer;
	//Writer<StringBuffer> writer(buffer);
	//d.Accept(writer);

	//// Output {"project":"rapidjson","stars":11}
	//std::cout << buffer.GetString() << std::endl;
	//return 0;

	//SetConsoleOutputCP(CP_UTF8);	
	//// Using (raw) string literals and json::parse
	//json ex1 = json::parse(u8R"(
	//  {
	//	"pi": 3.141,
	//	"happy": true,
	//	"name":"김헌준"
	//  }
	//)");
	//cout << ex1["pi"] << endl;
	//cout << ex1["name"] << endl;
	//ex1["name"] = u8"김김김";
	//cout << ex1["name"] << endl;



	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.0.194");
	ServerSockAddr.sin_port = htons(35000);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	cout << "client connect" << endl;

	//os에서 제공하는 함수
	HANDLE ThreadHandles[2] = { 0, };
	//non blocking ,asynchrous
	ThreadHandles[0]=(HANDLE)_beginthreadex(0,0, RecvThread,&ServerSocket,0,0);
	//ThreadHandles[1] = (HANDLE)_beginthreadex(0, 0, SendThread,&ServerSocket, 0, 0);
	ThreadHandles[1] = (HANDLE)_beginthreadex(0, 0, MoveSendThread, &ServerSocket, 0, 0);


	//ThreadHandles[0] = (HANDLE)_beginthreadex(0, 0, RecvThread, (SOCKET*)&ServerSocket, CREATE_SUSPEND, 0); //CREATE_SUSPEND 입력방지

	//blocking
	WaitForMultipleObjects(2, ThreadHandles, FALSE, INFINITE);


	closesocket(ServerSocket);

	IsRecvThreadRunning = false;
	IsSendThreadRunning = false;

	
	CloseHandle(ThreadHandles[0]);
	CloseHandle(ThreadHandles[1]);
	WSACleanup();
	return 0;
}

	