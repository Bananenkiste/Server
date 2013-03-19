#include <iostream>
#include "Game.hpp"



int main()
{
    Game game;
    game.init();
    return 0;
}

/*
    WSADATA w;
    if(int result = WSAStartup(MAKEWORD(2,2), &w) != 0)
    {
        cout << "Winsock 2 konnte nicht gestartet werden! Error #" << result << endl;
        return 1;
    }

//*/
