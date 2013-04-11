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


    level = new Level("000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020000000000000000");
    Game::sendCommandToPlayers("RQST|MDE|6|"+level->getLevelString());
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
