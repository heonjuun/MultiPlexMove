#include "pch.h"

#include "NetUtil.h"

int SendAll(SOCKET ReciverSocket, const char* Data, int Size)
{
    int TotalSendDataSize = 0;
    int WantSendDataSize = Size;
    int SentBytes = 0;
    int Count = 0;
    do
    {
        SentBytes = send(ReciverSocket, Data + TotalSendDataSize, WantSendDataSize - TotalSendDataSize, 0);
        TotalSendDataSize += SentBytes;
        if (SentBytes <= 0)
        {
            return SentBytes;
        }
    } while (TotalSendDataSize < WantSendDataSize);
    return WantSendDataSize;
}

void DisconnectSocket(SOCKET DisconnectedSocket, fd_set* Sockets)
{
    SOCKADDR_IN ClosedSockAddr;
    memset(&ClosedSockAddr, 0, sizeof(ClosedSockAddr));
    int ClosedSockAddrLength = sizeof(ClosedSockAddr);

    SOCKET ClosedSocket = DisconnectedSocket;
    getpeername(ClosedSocket, (SOCKADDR*)&ClosedSockAddr, &ClosedSockAddrLength);
    FD_CLR(DisconnectedSocket, &Sockets);
    closesocket(ClosedSocket);
}
