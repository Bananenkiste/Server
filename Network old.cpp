#include "Network.hpp"
#include <sstream>
#include <stdio.h>

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
SOCKET Network::createSocket()
{
    if(!i)
    {
        init();
    }
    int node = socket(AF_INET, SOCK_STREAM, 0);
    if(node == INVALID_SOCKET)
    {
        std::cout << "TCP Socket konnte nicht erstellt werden!" << std::endl;
        return NULL;
    }
    std::cout << "TCP Socket konnte erstellt werden!" << std::endl;
    return node;
}

SOCKET Network::createUdpSocket()
{
    if(!i)
    {
        init();
    }
    int node = socket(AF_INET, SOCK_DGRAM, 0);
    if(node == INVALID_SOCKET)
    {
        std::cout << "UDP Socket konnte nicht erstellt werden!" << std::endl;
        return NULL;
    }
    std::cout << "UDPSocket konnte erstellt werden!" << std::endl;
    return node;
}

int Network::connectTo(SOCKET Socket,std::string Ip,int port)
{
    sockaddr_in service;             // Normale IPv4 Struktur
    service.sin_family = AF_INET;   // AF_INET für IPv4, für IPv6 wäre es AF_INET6
    service.sin_port = htons(port);
    service.sin_addr.s_addr = inet_addr(Ip.c_str());

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

void Network::sendData(SOCKET node,std::string msg)
{
    send(node,msg.c_str(),strlen(msg.c_str()),0);
}

std::string Network::recieveData(SOCKET node)
{
    float rc;
    FD_ZERO(&Network::fdset);
    FD_SET(node,&Network::fdset);
    struct timeval timeout;
    timeout.tv_sec=0.001;
    timeout.tv_usec=0;

    rc=select(0,&Network::fdset,NULL,NULL,&timeout);
    if(rc==SOCKET_ERROR)
    {
        return("CLOSE");
    }

    if(FD_ISSET(node,&Network::fdset))
    {
        char buffer[256];
        int rc;
        rc=recv(node,buffer,256,0);
        buffer[rc]=0;
        std::string msg = buffer;
        return msg;
    }
    return("");
}

void Network::closeSocket(SOCKET node)
{
    sendData(node,"CLOSE");
    closesocket(node);
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

std::string Network::getIP()
{
    char hostname[128];
    if(gethostname(hostname,128)==0)
    {
        struct hostent* host;
        host = gethostbyname(hostname);
        if(host!=NULL)
        {
            for(int i=0;host->h_addr_list[i]!=NULL;i++)
            {
                std::string str;
                int j;

                for( j = 0; j < host->h_length; j++ )
                {
                    std::stringstream addr;
                    if( j > 0 )
                    {
                        str+=".";
                    }
                    addr<<((unsigned int) ((unsigned char*)host->h_addr_list[i])[j]);
                    str += addr.str();
                }
                return (str);
            }
        }
    }
}

std::string Network::broadcastRecieve(SOCKET node)
{
    SOCKADDR_IN remoteAddr;
    int remoteAddrLen=sizeof(SOCKADDR_IN);

    char buf[256];
    int rc;
    rc=recvfrom(node,buf,256,0,(SOCKADDR*)&remoteAddr,&remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
      std::cout<<"Fehler: recvfrom, fehler code:"<<WSAGetLastError()<<std::endl;
      return("");
    }
    else
    {
      buf[rc]='\0';
      std::cout<<"udp:"<<buf<<std::endl;
      return(buf);
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
