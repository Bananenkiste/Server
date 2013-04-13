#include "GameMechanics.hpp"

#include "Player.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Bomb.hpp"

Level* GameMechanics::level;
std::vector<Bomb*> GameMechanics::bombs;

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


    level = new Level("301111111111103000002020202020000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020300000000000003");
    Game::sendCommandToPlayers("RQST|MDE|6|"+level->getLevelString());
    bombs.clear();
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

    if(movement&16)
    {
        sf::Vector2f pos =player->getPosition();
        int x=(pos.x-30)/30;
        int y=(pos.y-30)/30;
        std::cout<<"BOMB:"<<x<<","<<y<<std::endl;
        bombs.push_back(new Bomb(sf::Vector2i(x,y)));
        movement-=16;
    }
    switch(movement)
    {
        case 0:
        {
            player->setDir(movement);
            break;
        }
        case 1:
        {
            player->setDir(movement);
            break;
        }
        case 2:
        {
            player->setDir(movement);
            break;
        }
        case 4:
        {
            player->setDir(movement);
            break;
        }
        case 8:
        {
            player->setDir(movement);
            break;
        }
    }
}

bool GameMechanics::collisionCheck(Player* player,int pdir)
{
    sf::Vector2f pos = player->getPosition();
    sf::Vector2i dir = sf::Vector2i(0,0);
    int x,y;
    x=(pos.x-30)/30;
    y=(pos.y-30)/30;
    if(x<0||y<0||x>15||y>9)
    {
        return(true);
    }

    switch(pdir)
    {
        case 1:
        {
            dir = sf::Vector2i(0,-1);
            break;
        }
        case 2:
        {
            dir = sf::Vector2i(0,1);
            break;
        }
        case 4:
        {
            dir = sf::Vector2i(-1,0);
            break;
        }
        case 8:
        {
            dir = sf::Vector2i(1,0);
            break;
        }
    }
    x+=dir.x;
    y+=dir.y;
    if(x<0||y<0||x>15||y>9)
    {
        return(true);
    }
    switch(level->getTile(x,y))
    {
        case 0:
        {
            return(false);
            break;
        }
        case 1:
        {
            player->setDir(0);
            return(true);
            break;
        }
        case 2:
        {
            player->setDir(0);
            return(true);
            break;
        }
    }


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
                state=GAME;
            }
        }
        case GAME:
        {
            for(std::vector<Player*>::iterator it=players->begin();it!=players->end();++it)
            {
                (*it)->update(deltatime);

            }
            for(std::vector<Bomb*>::iterator it=bombs.begin();it!=bombs.end();++it)
            {
                (*it)->update(deltatime);

            }
        }
    }
}

void GameMechanics::draw(sf::RenderWindow* window)
{
    level->draw(window);
    for(std::vector<Bomb*>::iterator it=bombs.begin();it!=bombs.end();++it)
    {
        (*it)->draw(window);

    }
    for(std::vector<Player*>::iterator it=players->begin();it!=players->end();++it)
    {
        (*it)->draw(window);

    }
}

void GameMechanics::explodeBomb(Bomb* bomb,int strenght)
{
    sf::Vector2i pos = bomb->getGridPos();
    bool up=true,down=true,left=true,right=true;
    for(int x=1;x<=strenght;++x)
    {
        if(up)
        {
            if(pos.y-x<0||level->getTile(pos.x,pos.y-x)!=2)
            {
                up=false;
            }
            else
            {
                level->setTile(pos.x,pos.y-x,0);
            }
        }
        if(down)
        {
            if(pos.y+x>9||level->getTile(pos.x,pos.y+x)==2)
            {
                down=false;
            }
            else
            {
                level->setTile(pos.x,pos.y+x,0);
            }
        }
        if(left)
        {
            if(pos.x-x<0||level->getTile(pos.x-x,pos.y)==2)
            {
                left=false;
            }
            else
            {
                level->setTile(pos.x-x,pos.y,0);
            }
        }
        if(right)
        {
            if(pos.x+x>15||level->getTile(pos.x+x,pos.y)==2)
            {
                right=false;
            }
            else
            {
                level->setTile(pos.x+x,pos.y,0);
            }
        }

    }
}
