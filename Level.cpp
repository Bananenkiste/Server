#include "Level.hpp"

//#include "TextureBuffer.hpp"


Level::Level(std::string lvl)
{
    std::string alvl="000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020000000000000000020202020202020000000000000000";
    levelstring=alvl;

    for(int x=0;x<=9;++x)
    {
        for(int y=0;y<=15;++y)
        {
            leveldata[x][y]=alvl.at(x+y);
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
