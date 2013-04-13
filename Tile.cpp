#include "Tile.hpp"
#include "TextureBuffer.hpp"



Tile::Tile(std::string name,float x, float y)
{
    std::stringstream stream;
    stream<<name;
    stream>>type;
    sprite = TextureBuffer::LoadTexture(name,true,x,y);
}

Tile::~Tile()
{
}

int Tile::getTile()
{
    return(type);
}

void Tile::draw(sf::RenderWindow* window)
{
    window->draw(*sprite);
}

void Tile::setTexture(std::string Texturename)
{
    std::stringstream stream;
    stream<<Texturename;
    stream>>type;
    sprite=TextureBuffer::LoadTexture(Texturename,false,sprite->getPosition().x,sprite->getPosition().y);
}
