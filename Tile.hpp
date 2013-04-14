#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <sstream>

#include <sfml/graphics.hpp>
#include <sfml/window.hpp>
#include <sfml/system.hpp>

class Tile
{
    public:
        Tile(std::string name,float x, float y);
        virtual ~Tile();

        void draw(sf::RenderWindow* window);
        int getTile();
        void setTile(int state);
        void setTexture(std::string Texturename);
        std::string getTxtr();
    protected:
    private:
        std::string txtr;
        int type;
        sf::Sprite* sprite;
};

#endif // TILE_HPP
