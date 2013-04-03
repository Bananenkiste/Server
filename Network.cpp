#include "Network.hpp"


//Global Variables
bool Network::i;
FD_SET Network::fdset;




void Network::init()
{
    WSADATA w;
    if(int result = WSAStartup(MAKEWORD(2,2), &w) != 0)
    {
        std::cout << "Winsock 2 konnte nicht gestartet werden! Error #" << result << std::endl;
        return;
    }
    i=true;
}

SOCKET Network::createTcpSocket()
{
    if(!i)
    {
        init();
    }
    int node = socket(AF_INET,SOCK_STREAM, 0);
    if(node == INVALID_SOCKET)
    {
        std::cout << "Socket konnte nicht erstellt werden!" << std::endl;
        return NULL;
    }
    std::cout << "Socket konnte erstellt werden!" << std::endl;
    return node;
}

SOCKET Network::createUdpSocket()
{
    if(!i)
    {
        init();
    }
    int node = socket(AF_INET,SOCK_DGRAM, 0);
    if(node == INVALID_SOCKET)
    {
        std::cout << "Socket konnte nicht erstellt werden!" << std::endl;
        return NULL;
    }
    std::cout << "Socket konnte erstellt werden!" << std::endl;
    return node;
}


int Network::connectTo(SOCKET Socket,std::string ip,int port)
{
    sockaddr_in service;             // Normale IPv4 Struktur
    service.sin_family = AF_INET;   // AF_INET für IPv4, für IPv6 wäre es AF_INET6
    service.sin_port = htons(port);
    service.sin_addr.s_addr = inet_addr(ip.c_str());

    int result = connect(Socket, reinterpret_cast<sockaddr*>(&service), sizeof(service));
    if(result == -1)
    {
        std::cout << "Verbindung fehlgeschlagen!" <<WSAGetLastError()<< std::endl;
        return -1;
    }
    return 1;
}

SOCKET Network::waitForConnection(SOCKET node,int port)
{
    sockaddr_in addr;             // Normale IPv4 Struktur
    addr.sin_family = AF_INET;   // AF_INET für IPv4, für IPv6 wäre es AF_INET6
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind( node, (SOCKADDR*) &addr, sizeof(addr) ) != SOCKET_ERROR )
    {
        if(listen(node,SOMAXCONN)!=SOCKET_ERROR)
        {
            do
            {
                std::cout<<"Listen...."<<std::endl;
                SOCKET s;
                s = accept( node, NULL, NULL );
                closesocket(node);
                return s;
            }
            while(1);
        }
        else
        {
            std::cout<<"nope:"<<WSAGetLastError()<<std::endl;
        }
    }
    else
    {
        std::cout<<"asdf:"<<WSAGetLastError()<<std::endl;
    }
}

void Network::WaitForClient(SOCKET node, SOCKET s)
{
    s = accept( node, NULL, NULL );
    if(s!=INVALID_SOCKET)
    {
        std::cout<<"no error"<<std::endl;
    }
    else
    {
        std::cout<<"error"<<std::endl;
    }
}

void Network::sendTcpData(SOCKET node,std::string msg)
{
    std::cout<<"out:"<<msg<<std::endl;
    send(node,msg.c_str(),strlen(msg.c_str()),0);
}

std::string Network::recieveData(SOCKET node)
{
        char buffer[256];
        int rc;
        rc=recv(node,buffer,256,0);
        buffer[rc]=0;
        std::cout<<"N-Message"<<buffer<<std::endl;
        std::string msg = buffer;
        return msg;
}

udpMessage Network::recieveUdpData(SOCKET node)
{
        char buffer[256];
        int rc;
        sockaddr_in sender;
        int sendersize = sizeof(sender);
        rc=recvfrom(node,buffer,256,0,(SOCKADDR*)&sender,&sendersize);
        buffer[rc]=0;
        udpMessage msg;
        msg.ip = inet_ntoa(sender.sin_addr);
        msg.msg=buffer;
        msg.port=sender.sin_port;
        return msg;
}

void Network::closeSocket(SOCKET node)
{
    sendTcpData(node,"CLOSE");
    closesocket(node);
}

std::string Network::getIP()
{
    char hostname[255];
    gethostname(hostname,sizeof(hostname));

    struct hostent* host;
    struct in_addr add;

    host = gethostbyname(hostname);
    if(host != NULL)
    {
        switch (host->h_addrtype)
        {
            case AF_INET:
            {
                //IP V4
                int x=0;
                if(host->h_addr_list[0] != 0)
                {
                    add.s_addr = *(u_long*) host->h_addr_list[0];
                    return(inet_ntoa(add));
                }
                break;
            }
            case AF_INET6:
            {
                //IP V6
                std::cout<<"IP v6 is not supported"<<std::endl;
                break;
            }
        }
    }
    return ("ERROR");
}

void Network::broadcastSend(SOCKET node,int port, std::string msg)
{
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    std::string ip = getIP();
    ip.resize(ip.find_last_of(".")+1);
    ip.append("255");
    addr.sin_addr.s_addr=inet_addr(ip.c_str());

    sendto(node,msg.c_str(),strlen(msg.c_str()),0,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
}

void Network::udpSend(SOCKET node,std::string ip,int port, std::string msg)
{
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_addr.s_addr=inet_addr(ip.c_str());

    sendto(node,msg.c_str(),strlen(msg.c_str()),0,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
}

udpMessage Network::broadcastRecieve(SOCKET node)
{
    sockaddr_in remoteAddr;
    int remoteAddrLen=sizeof(remoteAddr);

    udpMessage msg;
    char buf[256];
    int rc;
    rc=recvfrom(node,buf,256,0,(SOCKADDR*)&remoteAddr,&remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
        std::cout<<"Fehler: recvfrom, fehler code:"<<WSAGetLastError()<<std::endl;
        return(msg);
    }
    else
    {
        buf[rc]='\0';
        msg.msg = buf;
        msg.ip = inet_ntoa(remoteAddr.sin_addr);
        msg.port = ntohs(remoteAddr.sin_port);
        std::cout<<msg.ip<<"|"<<msg.port<<std::endl;
        return(msg);
    }
}

void Network::bindSocket(SOCKET node,int port)
{
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=INADDR_ANY;

    int rc=bind(node,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
    if(rc==SOCKET_ERROR)
    {
        std::cout<<"bind error"<<std::endl;
        return;
    }
}

void Network::bindSocket(SOCKET node)
{
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;

    int rc=bind(node,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
    if(rc==SOCKET_ERROR)
    {
        std::cout<<"bind error"<<std::endl;
        return;
    }
}

