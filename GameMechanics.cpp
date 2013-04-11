#include "GameMechanics.hpp"

#include "Player.hpp"
#include "Game.hpp"
#include "Level.hpp"


GameMechanics::GameMechanics(std::vector<Player*>*playerlist)
{
    players=playerlist;
    state=PREGAME;
    level = new Level("something");
    //std::cout<<level->getLevelString().size()<<std::endl;
    //Game::sendCommandToPlayers("RQST|MDE|5|"+level->getLevelString());
    //std::cout<<level->getLevelString()<<std::endl;
}

void GameMechanics::handleInput(Player* player,std::string input)
{

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

        }

    }
}
