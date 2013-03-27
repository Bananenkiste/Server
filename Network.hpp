#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <winsock2.h>
#include <windows.h>
//libws2_32.a einbinden

struct udpMessage
{
    std::string ip;
    int port;
    std::string msg;
};

class Network
{
    public:
        static void init();
        static SOCKET createTcpSocket();
        static SOCKET createUdpSocket();

        static int connectTo(SOCKET Socket,std::string ip,int port);
        static SOCKET waitForConnection(SOCKET node, int port);
        static void WaitForClient(SOCKET node, SOCKET s);
        static void bindSocket(SOCKET node,int port);
        static void bindSocket(SOCKET node);

        static void sendTcpData(SOCKET node,std::string msg);
        static void udpSend(SOCKET node,std::string ip,int port, std::string msg);
        static void broadcastSend(SOCKET node,int port, std::string msg);

        static std::string recieveData(SOCKET node);
        static udpMessage recieveUdpData(SOCKET node);

        static std::string broadcastRecieve(SOCKET node);

        static void closeSocket(SOCKET node);
        static std::string getIP();




    protected:
    private:
        static bool i;
        static FD_SET fdset;
};

#endif // SOCKET_H
