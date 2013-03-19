#include "Message.hpp"

Message::Message(std::string text)
{
    message.setString(text);
    message.setCharacterSize(12);
    message.setStyle(sf::Text::Regular);
    message.setColor(sf::Color::White);
}

Message::~Message()
{
    //dtor
}

void Message::update(float step)
{

}

void Message::draw(sf::RenderWindow* window)
{
    window->draw(message);
}

void Message::setPosition(float x,float y)
{
    message.setPosition(x,y);
}
