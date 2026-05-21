#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include"NetUtil.h"

#include"MovePacket.h"
#include"PositionPacket.h"
#include<WinSock2.h>
#include<iostream>
#include<map>
#include<string>
#include "json.hpp"
#include"GameMap.h"


#pragma comment(lib,"ws2_32")
#pragma comment(lib,"NetworkCommon")
using namespace std;

char Buffer[1024] = { 0, };

struct PlayerState
{
	std::string UserID;
	int X;
	int Y;
};
std::map<SOCKET, PlayerState>Players;
int NextPlayerID = 1;
GameMap ServerMapView;

//blocking, synchrous, multiplexing(polling)
int main()
{
	cout << "server start" << endl;

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(35000);

	//already use port 이미 포트 사용중
	::bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, SOMAXCONN);



	//blocking, synchronous(TimeOut)
	TIMEVAL TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 500000;

	fd_set ReadSockets;
	fd_set CopyReadSockets;

	FD_ZERO(&ReadSockets);
	FD_SET(ListenSocket, &ReadSockets);

	while (true)
	{
		CopyReadSockets = ReadSockets;

		//0.5초씩 blocking
		int ChangeCount = select(0, &CopyReadSockets, 0, 0, &TimeOut);

		if (ChangeCount <= 0)
		{
			//Server Work
			//0.5초한번 서버 작업을 하는거
			continue;
		}

		//몬가 자료 있다.
		for (int i = 0; i < (int)ReadSockets.fd_count; ++i)
		{
			if (FD_ISSET(ReadSockets.fd_array[i], &CopyReadSockets))
			{
				if (ReadSockets.fd_array[i] == ListenSocket)
				{
					//connect process
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int ClientSockSockLength = sizeof(ClientSockAddr);

					//blocking, synchronous
					SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockSockLength);

					cout << "connect client " << inet_ntoa(ClientSockAddr.sin_addr) << endl;

					PlayerState NewPlayer;
					NewPlayer.UserID = "Player:" + std::to_string(NextPlayerID++);
					NewPlayer.X = 0;
					NewPlayer.Y = 0;
					Players[ClientSocket] = NewPlayer;

					FD_SET(ClientSocket, &ReadSockets);
				}
				else
				{
					//Data Receive

					//header
					unsigned short PacketSize = 0;
					int RecvBytes = recv(ReadSockets.fd_array[i], (char*)&PacketSize, sizeof(PacketSize), MSG_WAITALL);
					if (RecvBytes <= 0)
					{
						cout << "header recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}

					PacketSize = ntohs(PacketSize);
					cout << "packet size : " << PacketSize << endl;
					memset(Buffer, 0, sizeof(Buffer));

					//data JSON
					RecvBytes = recv(ReadSockets.fd_array[i], Buffer, PacketSize, MSG_WAITALL);
					cout << "body recv bytes : " << RecvBytes << endl;
					if (RecvBytes <= 0)
					{
						int ErrorCode = WSAGetLastError();
						cout << "data recv fail, error : " << ErrorCode << endl;
						cout << "data recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}
					else
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLength = sizeof(ClientSockAddr);

						getpeername(ReadSockets.fd_array[i], (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

						cout << "client(" << inet_ntoa(ClientSockAddr.sin_addr);
						cout << ")" << Buffer << " send" << endl;
						//모든 접속한 유저한테 전달

						// 1. 클라이언트가 보낸 이동 패킷 파싱
						MovePacket MoveData;						
						MoveData.Parse(Buffer);

						char Dir = MoveData.GetInput();

						// 2. 현재 소켓에 해당하는 플레이어 위치 가져오기
						SOCKET ClientSocket = ReadSockets.fd_array[i];
						PlayerState& State = Players[ClientSocket];

						// 3. 방향에 따라 서버에서 위치 변경
						switch (Dir)
						{
						case'W':
							State.Y--;
							break;
						case 'S':
							State.Y++;
							break;
						case 'A':
							State.X--;
							break;
						case'D':
							State.X++;
							break;
						default:
							cout << "invalid move input" << endl;
							break;
						}
						//cout << "이동한 위치 x:" << State.X << ",y:" << State.Y << endl;



						PositionPacket PosData;

						PosData.SetPosition(State.UserID,State.X,State.Y);
						std::string SendString = PosData.ToString();


						ServerMapView.SetPosition(State.UserID, State.X, State.Y);
						ServerMapView.Draw();

						for (int j = 0; j < (int)ReadSockets.fd_count; ++j)
						{
							//자기꺼는 그냥 찍고 안 받으면 안되요?
							//클라이언트에서는 처리 안함.
							if (ReadSockets.fd_array[j] != ListenSocket)
							{
								//PacketSize = (unsigned short)strlen(Buffer);
								unsigned short HostPacketSize = (unsigned short)SendString.length();
								unsigned short NetworkPacketSize = htons(HostPacketSize);


								//header
								int SentBytes = SendAll(
									ReadSockets.fd_array[j],
									(char*)&NetworkPacketSize,
									sizeof(NetworkPacketSize)
								);
								if (SentBytes <= 0)
								{
									cout << "header send fail." << endl;
									DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
									continue;
								}

								//Data
								SentBytes = SendAll(ReadSockets.fd_array[j], SendString.c_str(), HostPacketSize);
								if (SentBytes <= 0)
								{
									cout << "Data send fail." << endl;
									DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
									continue;
								}
							}
						}
					}
				}
			}
		}
	}






	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}

