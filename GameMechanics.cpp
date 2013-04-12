#include "GameMechanics.hpp"

#include "Player.hpp"
#include "Game.hpp"
#include "Level.hpp"


GameMechanics::GameMechanics(std::vector<Player*>*playerlist)
{
    players=playerlist;
    state=PREGAME;
    /*
    000000000000000
    020202020202020
    000000000000000
    020202020202020
    000000000000000
    020202020202020
    000000000000000
    020202020202020
    000000000000000
    */


    level = new Level("300000000000003020202020202020000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020300000000000003");
    Game::sendCommandToPlayers("RQST|MDE|6|"+level->getLevelString());
    for(std::vector<Player*>::iterator it=players->begin();it!=players->end();++it)
    {
        (*it)->setPosition(level->getSpawn((*it)->getId()));
    }
}

void GameMechanics::handleInput(Player* player,std::string input)
{
    std::stringstream stream;
    stream<<input;
    int movement;
    stream>>movement;
    std::cout<<"Input:"<<input<<"int:"<<movement<<std::endl;
    sf::Vector2f pos = player->getPosition();
    std::cout<<"pos"<<pos.x<<","<<pos.y<<std::endl;
    switch(movement)
    {
        case 1:
        {
            pos.y-=30;
            break;
        }
        case 2:
        {
            pos.y+=30;
            break;
        }
        case 4:
        {
            pos.x-=30;
            break;
        }
        case 8:
        {
            pos.x+=30;
            break;
        }
    }


    std::cout<<"pos"<<pos.x<<","<<pos.y<<std::endl;
    player->setPosition(pos);
}

void GameMechanics::update(float deltatime)
{
    if(players->size()<1)
    {
        std::cout<<"BACK TO LOBBY MODE"<<std::endl;
        Game::changeState();
        return;
    }
    switch(state)
    {
        case PREGAME:
        {
            bool start=true;
            for(std::vector<Player*>::iterator it =players->begin();it!=players->end();++it)
            {
                if((*it)->getState()!=5)
                {
                    start=false;
                }
            }
            if(start)
            {
                std::cout<<"NOW THE LEVEL DUDE !"<<std::endl;
                state=GAME;
            }
        }
        case GAME:
        {
            for(std::vector<Player*>::iterator it=players->begin();it!=players->end();++it)
            {
                (*it)->update(deltatime);
            }
        }
    }
}
