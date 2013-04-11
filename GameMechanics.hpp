#ifndef GAMEMECHANICS_HPP
#define GAMEMECHANICS_HPP

#include <iostream>
#include <vector>

class Player;
class Level;

class GameMechanics
{
    public:

        enum State{PREGAME,GAME,DEBRIEF};

        GameMechanics(std::vector<Player*>*playerlist);
        static void handleInput(Player* player,std::string input);
        void update(float deltatime);
        void draw();
    protected:
    private:
        std::vector<Player*>*players;
        Level* level;
        int state;
};

#endif // GAMEMECHANICS_HPP
