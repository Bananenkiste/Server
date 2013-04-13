#ifndef BOMB_HPP
#define BOMB_HPP

#include<sfml/graphics.hpp>
#include<sfml/window.hpp>

class Bomb
{
    public:
        Bomb(sf::Vector2i pos);
        virtual ~Bomb();

        void update(float step);
        void draw(sf::RenderWindow* window);

        sf::Vector2i getGridPos();
    protected:
    private:
        sf::Sprite* bomb;
        float timer;
        int strength;
        sf::Vector2i gridpos;
};

#endif // BOMB_HPP
