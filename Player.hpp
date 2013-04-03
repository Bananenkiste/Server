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
    private:
        void networkthread();
        int id;
        std::string name;
        glm::mat4 position;
        SOCKET socket;
        sf::Thread nthread;
        bool ready;
};

#endif // PLAYER_H
