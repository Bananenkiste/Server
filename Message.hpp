#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>

class Message
{
    public:
        Message(std::string text);
        virtual ~Message();

        void update(float step);
        void draw(sf::RenderWindow* window);
        void setPosition(float x, float y);

    private:
        sf::Text message;
        float msgtime;
};

#endif // MESSAGE_H
