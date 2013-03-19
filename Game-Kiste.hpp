#ifndef GAME_H
#define GAME_H

#include<sfml/window.hpp>
#include<sfml/graphics.hpp>

#include <vector>

class Player;
class Chatwindow;

class Game
{
    public:
        static void init();
        static void run();

        static void removePlayer(Player* player);
        static void sendMessageToPlayers(std::string msg);
        static void sendCommandToPlayers(std::string msg);

    private:
        static int findFreeId();
        static void mainSocket(void* a);
        static void identify(void* a);
        static void windowControl();
        static void draw();

        static std::vector<Player*> players;
        static int coresocket;
        static int udp;
        static bool end;
        static sf::RenderWindow* window;
};

#endif // GAME_H
