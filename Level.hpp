#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>

#include <sfml/graphics.hpp>
#include <sfml/system.hpp>


class Level
{
    public:
        Level(std::string lvl);
        virtual ~Level();

        std::string getLevelString();
    private:
        std::string levelstring;
        char leveldata[9][15];
};

#endif // LEVEL_HPP
