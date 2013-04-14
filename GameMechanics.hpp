#ifndef GAMEMECHANICS_HPP
#define GAMEMECHANICS_HPP


#include <sfml/system.hpp>
#include <iostream>
#include <vector>

#include <sfml/graphics.hpp>
#include <sfml/window.hpp>

class Player;
class Level;
class Bomb;

class GameMechanics
{
    public:

        enum State{PREGAME,GAME,DEBRIEF};

        GameMechanics(std::vector<Player*>*playerlist);
        static void handleInput(Player* player,std::string input);
        void update(float deltatime);
        void draw(sf::RenderWindow* window);
        static bool collisionCheck(Player* players,int pdir);
        static void placeBomb(int x,int y);
        static void explodeBomb(Bomb* bomb,int strenght);
    protected:
    private:
        static std::vector<Player*>*players;
        static std::vector<Bomb*>* bombs;
        static Level* level;
        int state;

};

#endif // GAMEMECHANICS_HPP
