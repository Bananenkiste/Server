#include "Level.hpp"

//#include "TextureBuffer.hpp"
#include "Tile.hpp"

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
                        spawn.push_back(sf::Vector2f(30+(x*30),30+(y*30)));
                        break;
                    }
                }
                ++i;
            }
            leveldata[x][y]=new Tile(tile,(x*30)+30,(y*30)+30);
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
            leveldata[x][y]->draw(window);
        }
    }
}

sf::Vector2f Level::getSpawn(int id)
{
    if(id<spawn.size())
    {
        return(spawn[id]);
    }
}

int Level::getTile(int x, int y)
{
    return(leveldata[x][y]->getTile());
}

void Level::setTile(int x, int y, int value)
{
    std::stringstream stream;
    stream<<value;
    std::string t = stream.str();
    levelstring[(x*9)+y]=t[1];
    leveldata[x][y]->setTexture(stream.str());
}
void Level::setType(int x, int y,int type)
{
    leveldata[x][y]->setTile(type);
}

