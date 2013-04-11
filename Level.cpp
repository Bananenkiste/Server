#include "Level.hpp"

#include "TextureBuffer.hpp"


Level::Level(std::string lvl)
{
    levelstring=lvl;

    int i=0;
    for(int y=0;y<9;++y)
    {
        for(int x=0;x<15;++x)
        {
            std::string tile="0";
            if(i<lvl.size())
            {
                switch(lvl.at(i))
                {
                    case '0':
                    {
                        tile="0";
                        break;
                    }
                    case '1':
                    {
                        tile="1";
                        break;
                    }
                    case '2':
                    {
                        tile="2";
                        break;
                    }
                    case '3':
                    {
                        tile="0";
                        spawn.push_back(sf::Vector2i(x,y));
                        break;
                    }
                }
                ++i;
            }
            leveldata[x][y]=TextureBuffer::LoadTexture(tile,false,(x*30)+30,(y*30)+30);
        }
    }
}

Level::~Level()
{
}

std::string Level::getLevelString()
{
    return(levelstring);
}

void Level::draw(sf::RenderWindow* window)
{
    for(int y=0;y<9;++y)
    {
        for(int x=0;x<15;++x)
        {
            window->draw(*leveldata[x][y]);
        }
    }
}

sf::Vector2i Level::getSpawn(int id)
{
    if(id<spawn.size())
    {
        return(spawn[id]);
    }
}

