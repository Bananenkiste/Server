#ifndef CHATWINDOW_H
#define CHATWINDOW_H


#include <sfml/window.hpp>
#include <sfml/graphics.hpp>

#include <vector>

class Message;

class Chatwindow
{
    public:
        Chatwindow();
        virtual ~Chatwindow();
        static void update(float step);
        static void draw(sf::RenderWindow* window);
        static void addText(std::string msg);
        static void removeText(Message* del);
    private:
        static std::vector<Message*> chat;
        static sf::Text text;
};

#endif // CHATWINDOW_H
