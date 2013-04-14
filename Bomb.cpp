#include "Bomb.hpp"

#include "GameMechanics.hpp"
#include "TextureBuffer.hpp"

Bomb::Bomb(sf::Vector2i pos)
{
    gridpos=pos;
    bomb = TextureBuffer::LoadTexture("bomb",true,30+(pos.x*30),30+(pos.y*30));
    TextureBuffer::LoadTexture("buffer",false);
    timer=2;
    strength=1;
}

Bomb::~Bomb()
{
    //dtor
}

void Bomb::update(float step)
{
    timer-=step;
    if(timer<=0)
    {
        std::cout<<"EXPLOSIOOOON !"<<std::endl;
        GameMechanics::explodeBomb(this,strength);
        timer=9000;
        return;
    }
}

void Bomb::draw(sf::RenderWindow* window)
{
    window->draw(*bomb);
}

sf::Vector2i Bomb::getGridPos()
{
    return(gridpos);
}
