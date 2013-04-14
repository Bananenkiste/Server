#include "GameMechanics.hpp"

#include "Player.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Bomb.hpp"

Level* GameMechanics::level;
std::vector<Bomb*>* GameMechanics::bombs;
std::vector<Player*>* GameMechanics::players;

GameMechanics::GameMechanics(std::vector<Player*>*playerlist)
{
    players=playerlist;
    state=PREGAME;
    /*
    301111111111103
    021212121212120
    111130111031111
    121202020202121
    111111111111111
    020202121202121
    111130111031111
    021212121212120
    301111111111103
    */
    level = new Level("301111111111103021212121212120111130111031111121202020202121111111111111111020202121202121111130111031111021212121212120301111111111103");
    Game::sendCommandToPlayers("RQST|MDE|6|"+level->getLevelString());
    bombs = new std::vector<Bomb*>();
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
        //std::cout<<"INTF|BOMB|C|"<<x<<","<<y<<std::endl;
        std::stringstream posx,posy;
        posx<<x;
        posy<<y;
        Game::sendCommandToPlayers("INTF|BOMB|C|"+posx.str()+","+posy.str());
        level->setType(x,y,5);
        bombs->push_back(new Bomb(sf::Vector2i(x,y)));
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
        default :
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
            for(std::vector<Bomb*>::iterator it=bombs->begin();it!=bombs->end();++it)
            {
                (*it)->update(deltatime);

            }
        }
    }
}

void GameMechanics::draw(sf::RenderWindow* window)
{
    level->draw(window);
    for(std::vector<Bomb*>::iterator it=bombs->begin();it!=bombs->end();++it)
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
    level->setType(pos.x,pos.y,0);
    int strenght2 = strenght*strenght;
    for(std::vector<Player*>::iterator it=players->begin();it!=players->end();++it)
    {
        sf::Vector2f playerpos = (*it)->getPosition();
        playerpos.x = (playerpos.x-30)/30;
        playerpos.y = (playerpos.y-30)/30;
        sf::Vector2i dist;
        dist=pos-(sf::Vector2i)playerpos;
        dist.x*=dist.x;
        dist.y*=dist.y;
        std::stringstream cid;
        cid<<(*it)->getId();

        if((dist.x<=strenght2&&pos.x==playerpos.x)||(dist.y<=strenght2&&pos.y==playerpos.y))
        {
            (*it)->setAlive(false);
            Game::sendCommandToPlayers("PACT|DTH|"+cid.str());
        }
    }
    std::stringstream posx,posy;
    std::vector<sf::Vector2i> box;
    posx<<pos.x;
    posy<<pos.y;
    Game::sendCommandToPlayers("INTF|BOMB|R|"+posx.str()+","+posy.str());
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
                box.push_back(sf::Vector2i(pos.x,pos.y-x));
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
                box.push_back(sf::Vector2i(pos.x,pos.y+x));
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
                box.push_back(sf::Vector2i(pos.x-x,pos.y));
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
                box.push_back(sf::Vector2i(pos.x+x,pos.y));
            }
        }

    }
    std::stringstream stream;
    stream<<box.size();
    std::string boxcmd = "INTF|EXPL|"+stream.str()+"|";
    stream.clear();
    std::stringstream stream2;
    for(std::vector<sf::Vector2i>::iterator it=box.begin();it!=box.end();++it)
    {
        std::stringstream posx,posy;
        posx<<(*it).x;
        posy<<(*it).y;
        std::cout<<"string"<<posx.str()<<","<<posy.str()<<"int:"<<(*it).x<<","<<(*it).y<<std::endl;
        boxcmd+=posx.str()+","+posy.str()+"|";

    }
    Game::sendCommandToPlayers(boxcmd);
    for(std::vector<Bomb*>::iterator it=bombs->begin();it!=bombs->end();++it)
    {
        if((*it)==bomb)
        {
            //bombs->pop_back();
            //bombs->erase(it);
            return;
        }
    }


}
