#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <process.h>


#include "Game.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "Chatwindow.hpp"
#include "Time.hpp"
#include "GameMechanics.hpp"
#include "TextureBuffer.hpp"
#include "Database.hpp"

bool Game::end;
int Game::state;
std::vector<Player*> Game::players;
int Game::coresocket;
sf::RenderWindow* Game::window;
int Game::udp;
GameMechanics* Game::gamemech;

void Game::run()
{
    while(Game::end != true)
    {
        Time::update();
        windowControl();
        ////////////////////////////////////
        //////////  Update  ////////////////
        ////////////////////////////////////
        switch(state)
        {
            case INGAME:
            {
                Game::gamemech->update(Time::step());
            }

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

    if(state==INGAME)
    {
        //gamemech->draw(window);
    }

    Chatwindow::draw(window);

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
    //Load all graphics
    TextureBuffer::LoadTexture("0",false);
    TextureBuffer::LoadTexture("1",false);
    TextureBuffer::LoadTexture("2",false);
    TextureBuffer::LoadTexture("p0",false);
    TextureBuffer::LoadTexture("p1",false);
    TextureBuffer::LoadTexture("p2",false);
    TextureBuffer::LoadTexture("p3",false);
    TextureBuffer::LoadTexture("p4",false);
    TextureBuffer::LoadTexture("p5",false);
    TextureBuffer::LoadTexture("p6",false);
    TextureBuffer::LoadTexture("p7",false);
    TextureBuffer::LoadTexture("bomb",false);
    //Real Program
    Game::end = false;
    Game::udp = Network::createUdpSocket();
    Network::bindSocket(udp,40001);
    //sf::Thread msock(&mainSocket);

    _beginthread(Game::mainSocket,0,NULL);
    _beginthread(Game::identify,0,NULL);
    //////////////////////////////////////////////////////
    //Init Window and Stage - start Gameloop
    window = new sf::RenderWindow(sf::VideoMode(800,600,sf::VideoMode::getDesktopMode().bitsPerPixel),"Bomberman");
    //window->setVerticalSyncEnabled( true );
    Chatwindow::addText("Networktest - Server");
    Chatwindow::addText(Network::getIP().c_str());
    state = LOBBY;
    //Database::test();

    run();

}

void Game::mainSocket(void* a)
{
    while(end!=true)
    {
        int newsocket = 0;
        newsocket = Network::waitForConnection(Network::createTcpSocket(),40000);
        Chatwindow::addText("incoming connection");
        int x = getID();
        std::cout<<"ID:"<<x<<std::endl;
        Player* nplayer = new Player(x,newsocket);
        players.push_back(nplayer);
    }
}

void Game::identify(void* a)
{
    while(end!=true)
    {
        udpMessage rec = Network::broadcastRecieve(udp);
        std::string msg = rec.msg.c_str();
        std::string key = msg.substr(0,msg.find_first_of("|"));
        if(strcmp("BBM",key.c_str())==0)
        {
            if(strcmp("BBM|RQSRV",msg.c_str())==0&&state==LOBBY)
            {
                Network::udpSend(udp,rec.ip,rec.port,"BBM|SRV|Bomberman Server|"+Network::getIP()+"|");
                //Network::broadcastSend(udp,40002,"BBM|SRV|Bomberman Server|"+Network::getIP());
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
    for(std::vector<Player*>::iterator it = players.begin();it!=players.end();++it)
    {
        Network::sendTcpData((*it)->getSocket(),"MSG|"+msg+"|");
    }
}

void Game::sendCommandToPlayers(std::string cmd)
{
    for(std::vector<Player*>::iterator it = players.begin();it!=players.end();++it)
    {
        Network::sendTcpData((*it)->getSocket(),cmd);
    }
}

void Game::sendPlayersToPlayer(Player* pl)
{
    for(std::vector<Player*>::iterator lst = players.begin();lst!=players.end();++lst)
    {
        std::stringstream stream;
        stream<<(*lst)->getId();
        std::string msg = "PJOIN|"+stream.str();
        msg = msg+"|"+(*lst)->getName().c_str()+"|";
        Network::sendTcpData(pl->getSocket(),msg);
    }
}

std::vector<Player*> Game::getPlayers()
{
    return(players);
}

int Game::getID()
{
    int id=0;
    bool valid;
    while(true)
    {
        valid = true;
        for(std::vector<Player*>::iterator it = players.begin();it!=players.end();++it)
        {
            if(id==(*it)->getId())
            {
                valid = false;
            }
        }
        if(valid)
        {
            return (id);
        }
        ++id;
    }
}

void Game::startGame()
{
    if(players.size()>0)
    {
        bool start=true;
        for(std::vector<Player*>::iterator it = players.begin();it!=players.end();++it)
        {
            if(!(*it)->getReady())
            {
                start=false;
                sendMessageToPlayers("Lets get ready to rumble !!!");
            }
        }
        if(start)
        {
            //sendMessageToPlayers("Game start");
            Chatwindow::addText("StartGame");
            Game::gamemech = new GameMechanics(&players);
            state=INGAME;
        }
    }
    else
    {
        sendMessageToPlayers("Not enough Players");
        Chatwindow::addText("Not enough Players");
    }
}

void Game::changeState()
{
    state=LOBBY;
    players.clear();

}
