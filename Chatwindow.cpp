#include "Chatwindow.hpp"
#include "Message.hpp"


std::vector<Message*> Chatwindow::chat;
sf::Text Chatwindow::text;

Chatwindow::Chatwindow()
{
    //ctor
}

Chatwindow::~Chatwindow()
{
    //dtor
}

void Chatwindow::update(float step)
{

}

void Chatwindow::draw(sf::RenderWindow* window)
{
    for(std::vector<Message*>::iterator it = chat.begin();it!=chat.end();++it)
    {
        (*it)->draw(window);
    }
}

void Chatwindow::addText(std::string msg)
{
    chat.insert(chat.begin(),new Message(msg));
    if(chat.size()>10)
    {
        chat.resize(10);
    }
    for(int x=0;x<chat.size();++x)
    {
        chat[x]->setPosition(20,560-(20*x));
    }
}
