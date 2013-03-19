#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <process.h>


#include "Game.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "Chatwindow.hpp"
#include "Time.hpp"

bool Game::end;
std::vector<Player*> Game::players;
int Game::coresocket;
sf::RenderWindow* Game::window;
int Game::udp;

void Game::run()
{
    while(Game::end != true)
    {
        windowControl();
        ////////////////////////////////////
        //////////  Update  ////////////////
        ////////////////////////////////////
        for(int i = players.size()-1;i>=0;--i)
        {
            players.at(i)->update(Time::step());
        }
        ////////////////////////////////////
        ///////  Draw  /////////////////////
        ////////////////////////////////////
        draw();
    }
    sendCommandToPlayers("CLOSE");
    WSACleanup();
    return;
}

void Game::draw()
{
    window->clear();
    window->pushGLStates();

    Chatwindow::draw(window);

    window->popGLStates();
    window->display();
}


void Game::windowControl()
{
    sf::Event event;
	while (window->pollEvent(event))
	{
		if ( event.type == sf::Event::Closed ) // Close window : exit
		{
			Game::end=true;
		}
	}
}

void Game::init()
{
    ///////////////////////////////////////////////
    //Network start
    /////////////////////////////////
    WSADATA w;
    if(int result = WSAStartup(MAKEWORD(2,2), &w) != 0)
    {
        std::cout << "Winsock 2 konnte nicht gestartet werden! Error #" << result << std::endl;
        return;
    }
    //Real Program
    Game::end = false;
    Game::udp = Network::createUdpSocket();
    Network::bindSocket(udp,40001);
    _beginthread(Game::mainSocket,0,NULL);
    _beginthread(Game::identify,0,NULL);
    //////////////////////////////////////////////////////
    //Init Window and Stage - start Gameloop
    window = new sf::RenderWindow(sf::VideoMode(800,600,sf::VideoMode::getDesktopMode().bitsPerPixel),"Bomberman");
    //window->setVerticalSyncEnabled( true );
    Chatwindow::addText("Networktest - Server");
    Chatwindow::addText(Network::getIP().c_str());
    run();
}

void Game::mainSocket(void* a)
{
    while(end!=true)
    {
        int newsocket = 0;
        newsocket = Network::waitForConnection(Network::createSocket(),40000);
        if(newsocket!=0)
        {
            Chatwindow::addText("incoming connection");
            Player* nplayer = new Player(findFreeId());
            nplayer->setSocket(newsocket);
            players.push_back(nplayer);
        }
    }
}
int Game::findFreeId()
{
    int x=0;
    bool f=true;
    do
    {
        for(std::vector<Player*>::iterator it=players.begin();it!=players.end();it++)
        {
            if((*it)->getId()==x)
            {
                f=false;
            }
        }
        if(f)
        {
            return x;
        }
        x++;
    }
    while(true);
}

void Game::identify(void* a)
{
    while(end!=true)
    {
        std::string msg = Network::broadcastRecieve(udp);
        std::string key = msg.substr(0,msg.find_first_of("|"));
        if(strcmp("BBM",key.c_str())==0)
        {
            if(strcmp("BBM|RQSRV",msg.c_str())==0)
            {
                Network::broadcastSend(udp,40002,"BBM|SRV|Bomberman Server|"+Network::getIP());
                Chatwindow::addText("Send indent");
            }
        }
    }
}

void Game::removePlayer(Player* player)
{
    for(std::vector<Player*>::iterator it = players.begin();it!=players.end();++it)
    {
        if((*it)==player)
        {
            players.erase(it);
            free(player);
            return;
        }
    }
}

void Game::sendMessageToPlayers(std::string msg)
{
    for(int x=players.size()-1;x>=0;--x)
    {
        Network::sendData(players[x]->getSocket(),"MSG|"+msg);
    }
}

void Game::sendCommandToPlayers(std::string cmd)
{
    for(int x=players.size()-1;x>=0;--x)
    {
        Network::sendData(players[x]->getSocket(),cmd);
    }
}
