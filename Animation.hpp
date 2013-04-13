#ifndef ANIMATION_H
#define ANIMATION_H

#include "TextureBuffer.hpp"
#include <sfml/Graphics.hpp>
#include <glm/glm.hpp>
#include <vector>

class Animation
{
    public:
        Animation(std::string animname,float rate);
        Animation(std::string animname,float rate, glm::vec2 apos);
        virtual ~Animation();

        bool update(float step, int offsetX, int offsetY);
        bool update(float step);
        sf::Sprite* getAnim();
        void setFps(float rate);
    private:
        std::vector<sf::Sprite> animation;
        int state;
        float step;
        float fps;
        glm::vec2 pos;
};

#endif // ANIMATION_H
