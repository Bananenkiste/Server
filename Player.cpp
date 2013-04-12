#include "Player.hpp"
#include "Network.hpp"
#include "Game.hpp"
#include "Chatwindow.hpp"
#include "GameMechanics.hpp"
#include <sstream>
#include <windows.h>
#include <process.h>

Player::Player(int aid, SOCKET asocket): id(aid), socket(asocket), nthread(&Player::networkthread,this)
{
    nthread.launch();
    name="";
    ready=false;
    return;
}

Player::~Player()
{

}

int Player::getId()
{
    return (id);
}

std::string Player::getName()
{
    return(name);
}


SOCKET Player::getSocket()
{
    return(socket);
}

void Player::update(float step)
{

}

void Player::networkthread()
{
    while(true)
    {
        if(socket!= INVALID_SOCKET)
        {
            std::string msg = Network::recieveData(socket);
            if(strcmp("",msg.c_str())!=0)
            {
                std::string key = msg.substr(0,msg.find_first_of("|"));
                if(strcmp("CLOSE",key.c_str())==0)
                {
                    std::string nmsg = name+" left the game.";
                    Game::sendMessageToPlayers(nmsg);
                    Chatwindow::addText(nmsg);
                    closesocket(socket);
                    socket = INVALID_SOCKET;
                    Game::removePlayer(this);
                    return;
                }
                if(strcmp("IDENT",key.c_str())==0)
                {
                    name = msg.substr(msg.find_first_of("|")+1);
                    std::stringstream cid;
                    cid<<this->id;

                    std::string nmsg = "PJOIN|"+cid.str()+"|"+name.c_str()+"|";
                    //Game::sendCommandToPlayers(nmsg);
                    Game::sendPlayersToPlayer(this);
                    Chatwindow::addText(nmsg);

                }
                if(strcmp("MSG",key.c_str())==0)
                {
                    std::string text = msg.substr(msg.find_first_of("|")+1);
                    std::string nmsg = name+": "+text+"\n";
                    std::cout<<nmsg<<std::endl;
                    Game::sendMessageToPlayers(nmsg);
                    Chatwindow::addText(nmsg);
                }
                if(strcmp("RDY",key.c_str())==0)
                {
                    ready=!ready;
                    std::stringstream cid;
                    cid<<this->id;
                    std::string nmsg = "PACT|RDY|"+cid.str();
                    Game::sendCommandToPlayers(nmsg);
                    if(ready)
                    {
                        Game::startGame();
                    }
                }
                if(strcmp("INTF",key.c_str())==0)
                {
                    std::stringstream intf;
                    intf<<msg.substr(msg.find_first_of("|")+1);
                    intf>>state;
                    std::cout<<"awesome fucking state: "<<state<<"string manipulation: "<< intf.str()<<std::endl;
                }
                if(strcmp("MOV",key.c_str())==0)
                {
                    msg=msg.substr(msg.find_first_of("|")+1);
                    GameMechanics::handleInput(this,msg);
                    float x=position.x,y=position.y;
                    std::stringstream cid,mov_x,mov_y;
                    cid<<id;
                    mov_x<<position.x;
                    mov_y<<position.y;

                    Game::sendCommandToPlayers("PACT|MOV|"+cid.str()+"|"+mov_x.str()+"|"+mov_y.str()+"|"+msg);
                }
            }
        }
    }
}

bool Player::getReady()
{
    return(ready);
}

int Player::getState()
{
    return(state);
}

sf::Vector2f Player::getPosition()
{
    return(position);
}

void Player::setPosition(sf::Vector2f npos)
{
    position=npos;
}

sf::Vector2f Player::getDir()
{
    return(dir);
}

void Player::setDir(sf::Vector2f ndir)
{
    dir=ndir;
}


