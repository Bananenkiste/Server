#include "Player.hpp"
#include "Network.hpp"
#include "Game.hpp"
#include "Chatwindow.hpp"
#include <sstream>

Player::Player(int aid): id(aid)
{
    sf::Thread nthread(&Player::networkthread,this);
    nthread.launch();
}

Player::~Player()
{
    //dtor
}

int Player::getId()
{
    return (id);
}

void Player::setSocket(SOCKET asocket)
{
    socket = asocket;
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
                std::cout<<msg<<std::endl;
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
                    cid<<id;
                    std::string nmsg = "PJOIN|"+cid.str() +"|"+name.c_str()+"|";
                    Game::sendCommandToPlayers(nmsg);
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
            }
        }
    }
}
