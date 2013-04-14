#include "Tile.hpp"
#include "TextureBuffer.hpp"



Tile::Tile(std::string name,float x, float y)
{
    txtr=name;
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
void Tile::setTile(int state)
{
    type=state;
}

void Tile::draw(sf::RenderWindow* window)
{
    window->draw(*sprite);
}

std::string Tile::getTxtr()
{

}

void Tile::setTexture(std::string Texturename)
{
    txtr=Texturename;
    std::stringstream stream;
    stream<<Texturename;
    stream>>type;
    sprite=TextureBuffer::LoadTexture(Texturename,false,sprite->getPosition().x,sprite->getPosition().y);
}
