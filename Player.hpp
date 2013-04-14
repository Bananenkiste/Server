#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <iostream>
#include <winsock2.h>
#include <windows.h>


#include <sfml/graphics.hpp>
#include <sfml/window.hpp>
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
        void draw(sf::RenderWindow* window);
        bool getReady();
        int getState();
        sf::Vector2f getPosition();
        void setPosition(sf::Vector2f npos);
        void setAlive(bool state);
        int getDir();
        void setDir(int ndir);

    private:
        void networkthread();
        int id;
        std::string name;
        sf::Vector2f position;
        sf::Vector2f target;
        int dir;
        int tempdir;

        sf::Sprite* pawn;
        float speed;
        SOCKET socket;
        sf::Thread nthread;
        bool ready;
        int state;
        bool alive;
};

#endif // PLAYER_H
