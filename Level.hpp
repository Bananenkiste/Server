#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <vector>

#include <sfml/graphics.hpp>
#include <sfml/window.hpp>
#include <sfml/system.hpp>


class Level
{
    public:
        Level(std::string lvl);
        virtual ~Level();

        std::string getLevelString();
        void draw(sf::RenderWindow* window);
        sf::Vector2f getSpawn(int id);
        int getTile(int x, int y);

    private:
        std::vector<sf::Vector2f> spawn;
        std::string levelstring;
        sf::Sprite* leveldata[16][10];
};

#endif // LEVEL_HPP
