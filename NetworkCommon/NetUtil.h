#pragma once

#include "pch.h"

extern int SendAll(SOCKET ReciverSocket,const char *Data,int Size);
extern void DisconnectSocket(SOCKET DisconnectedSocket, fd_set *Sockets);

