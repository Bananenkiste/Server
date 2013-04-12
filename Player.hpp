#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <sfml/system.hpp>

class Player
{
    public:
        Player(int aid, SOCKET asocket);
        virtual ~Player();

        int getId();
        std::string getName();

        void setSocket(SOCKET asocket);
        SOCKET getSocket();
        void update(float step);
        bool getReady();
        int getState();
        sf::Vector2f getPosition();
        void setPosition(sf::Vector2f npos);

        sf::Vector2f getDir();
        void setDir(sf::Vector2f ndir);

    private:
        void networkthread();
        int id;
        std::string name;
        sf::Vector2f position;
        sf::Vector2f dir;
        float speed;
        SOCKET socket;
        sf::Thread nthread;
        bool ready;
        int state;

};

#endif // PLAYER_H
