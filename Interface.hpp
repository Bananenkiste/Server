#ifndef INTERFACE_H
#define INTERFACE_H

#include <sfml/window.hpp>
#include <sfml/graphics.hpp>


class Interface
{
    public:
        Interface();
        virtual ~Interface();

        virtual void draw(sf::RenderWindow* window);

    protected:
    private:
};

#endif // INTERFACE_H
