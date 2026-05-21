#pragma once

#include "pch.h"

enum class EpacketType :unsigned short
{
	C2S_login = 100,
	S2C_login,
	S2C_Spawn,
	S2C_Destroy,
	C2S_Move,
	S2C_Move,
	ChatPacket,
	Max

};

#pragma pack(push,1)
struct Header
{
	unsigned short PacketSize;
	unsigned short PacketType;
};
#pragma pack(pop)
extern int SendAll(SOCKET ReciverSocket,const char *Data,int Size);
extern void DisconnectSocket(SOCKET DisconnectedSocket, fd_set *Sockets);

