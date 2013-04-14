#include "Player.hpp"
#include "Network.hpp"
#include "Game.hpp"
#include "Chatwindow.hpp"
#include "GameMechanics.hpp"
#include "TextureBuffer.hpp"
#include "Database.hpp"
#include <sstream>
#include <windows.h>
#include <process.h>

Player::Player(int aid, SOCKET asocket): id(aid), socket(asocket), nthread(&Player::networkthread,this)
{
    nthread.launch();
    name="";
    ready=false;
    dir=0;
    tempdir=0;
    speed=50;
    std::stringstream stream;
    stream<<id;
    pawn = TextureBuffer::LoadTexture("p"+stream.str(),true);
    alive=true;
}

Player::~Player()
{
    std::stringstream stream;
    stream<<id;
    Game::sendCommandToPlayers("PLEAVE|"+stream.str()+"|");
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

bool Player::getAlive()
{
    return(alive);
}

void Player::update(float step)
{
    if(!alive)
    {
        return;
    }

    if(dir==0&&tempdir!=0)
    {
        std::cout<<"new dir"<<std::endl;
        if(!GameMechanics::collisionCheck(this,tempdir))
        {
            dir=tempdir;
            target=position;
            if(tempdir&1)
            {
                target-=sf::Vector2f(0,30);
            }
            if(tempdir&2)
            {
                target+=sf::Vector2f(0,30);
            }
            if(tempdir&4)
            {
                target-=sf::Vector2f(30,0);
            }
            if(tempdir&8)
            {
                target+=sf::Vector2f(30,0);
            }
            std::stringstream cid,mov_x,mov_y,cdir;
            cid<<id;
            mov_x<<position.x;
            mov_y<<position.y;
            cdir<<dir;
            Game::sendCommandToPlayers("PACT|MOV|"+cid.str()+"|"+mov_x.str()+"|"+mov_y.str()+"|"+cdir.str()+"|");
        }
    }
    if(dir!=0)
    {
        sf::Vector2f mov;
        if(dir&1)
        {
            mov-=sf::Vector2f(0,1);
        }
        if(dir&2)
        {
            mov+=sf::Vector2f(0,1);
        }
        if(dir&4)
        {
            mov-=sf::Vector2f(1,0);
        }
        if(dir&8)
        {
            mov+=sf::Vector2f(1,0);
        }
        position.x+=(mov.x*speed)*step;
        position.y+=(mov.y*speed)*step;

        sf::Vector2f dif = position-target;
        dif.x*=dif.x;
        dif.y*=dif.y;
        if(dif.x<=4&&dif.y<=4)
        {
            position=target;
            if(!GameMechanics::collisionCheck(this,tempdir))
            {
                if(tempdir&1)
                {
                    target-=sf::Vector2f(0,30);
                }
                if(tempdir&2)
                {
                    target+=sf::Vector2f(0,30);
                }
                if(tempdir&4)
                {
                    target-=sf::Vector2f(30,0);
                }
                if(tempdir&8)
                {
                    target+=sf::Vector2f(30,0);
                }
                if(dir!=tempdir)
                {
                    dir=tempdir;
                    std::stringstream cid,mov_x,mov_y,cdir;
                    cid<<id;
                    mov_x<<position.x;
                    mov_y<<position.y;
                    cdir<<dir;
                    Game::sendCommandToPlayers("PACT|MOV|"+cid.str()+"|"+mov_x.str()+"|"+mov_y.str()+"|"+cdir.str()+"|");
                }
            }
            else
            {
                dir=0;
                std::stringstream cid,mov_x,mov_y,cdir;
                cid<<id;
                mov_x<<position.x;
                mov_y<<position.y;
                cdir<<dir;
                Game::sendCommandToPlayers("PACT|MOV|"+cid.str()+"|"+mov_x.str()+"|"+mov_y.str()+"|"+cdir.str()+"|");
            }
        }
    }

}

void Player::draw(sf::RenderWindow* window)
{
    if(alive)
    {
        pawn->setPosition(position);
        window->draw(*pawn);
    }
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
                if(strcmp("REG",key.c_str())==0)
                {
                    msg = msg.substr(msg.find_first_of("|")+1);
                    name = msg.substr(0,msg.find_first_of("|"));
                    msg = msg.substr(msg.find_first_of("|")+1);
                    std::string password = msg.substr(msg.find_first_of("|")+1);
                    std::stringstream cid;
                    cid<<this->id;
                    Database::insertData("INSERT INTO main.usr(usr_name,usr_password)Values('"+name+"','"+password+"');");
                    if(Database::Login(name,password))
                    {
                        Network::sendTcpData(socket,"RQST|MDE|4|");
                        std::string nmsg = "PJOIN|"+cid.str()+"|"+name.c_str()+"|";
                        Game::sendCommandToPlayers(nmsg);
                        Game::sendPlayersToPlayer(this);
                        Chatwindow::addText(nmsg);
                    }
                    else
                    {
                        Network::sendTcpData(socket,"RQST|MDE|3|");
                        Network::closeSocket(socket);
                        Game::removePlayer(this);
                    }
                }
                if(strcmp("IDENT",key.c_str())==0)
                {
                    msg = msg.substr(msg.find_first_of("|")+1);
                    name = msg.substr(0,msg.find_first_of("|"));
                    msg = msg.substr(msg.find_first_of("|")+1);
                    std::string password = msg.substr(0,msg.find_first_of("|"));
                    std::stringstream cid;
                    cid<<this->id;
                    //login
                    if(Database::Login(name,password))
                    {
                        Network::sendTcpData(socket,"RQST|MDE|4|");
                        std::string nmsg = "PJOIN|"+cid.str()+"|"+name.c_str()+"|";
                        Game::sendCommandToPlayers(nmsg);
                        Game::sendPlayersToPlayer(this);
                        Chatwindow::addText(nmsg);
                    }
                    else
                    {
                        Network::sendTcpData(socket,"RQST|MDE|3|");
                        Network::closeSocket(socket);
                        Game::removePlayer(this);
                    }
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
                    std::string nmsg = "PACT|RDY|"+cid.str()+"|";
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
                }
                if(strcmp("MOV",key.c_str())==0)
                {
                    if(alive)
                    {
                        msg=msg.substr(msg.find_first_of("|")+1);
                        GameMechanics::handleInput(this,msg);
                    }
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

int Player::getDir()
{
    return(dir);
}

void Player::setDir(int ndir)
{
    tempdir=ndir;
}

void Player::setAlive(bool state)
{
    alive=state;
}

int Player::getBombCount()
{
    return(bombcount);
}

void Player::bombCount()
{
    ++bombcount;
}

