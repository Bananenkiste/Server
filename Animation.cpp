#include "Animation.hpp"
#include <sstream>
#include <iostream>

Animation::Animation(std::string animname,float arate):fps(arate)
{
    state=0;
    step = 0;
    int nr=1;
    bool end=false;
    do
    {
        std::stringstream file;
        file<<animname<<nr;

        if(TextureBuffer::LoadTexture(file.str(),true)==NULL)
        {
            end=true;
            return;
        }
        else
        {
            sf::Sprite anim = (*TextureBuffer::LoadTexture(file.str(),true));
            std::cout<<file.str()<<std::endl;
            animation.push_back(anim);
        }
        ++nr;
    }
    while(!end);

}
Animation::Animation(std::string animname, float rate, glm::vec2 apos):fps(rate),pos(apos)
{
    state=0;
    step = 0;
    int nr=1;
    bool end=false;
    do
    {
        std::stringstream file;
        file<<animname<<nr;

        if(TextureBuffer::LoadTexture(file.str(),true)==NULL)
        {
            end=true;
            return;
        }
        else
        {
            sf::Sprite anim = (*TextureBuffer::LoadTexture(file.str(),true));
            //std::cout<<file.str()<<std::endl;
            animation.push_back(anim);
        }
        ++nr;
    }
    while(!end);

}

Animation::~Animation()
{
    //dtor
}

void Animation::setFps(float rate)
{
    fps = rate;
}

bool Animation::update(float tick,int offsetX, int offsetY)
{
    step+=tick;
    if(step>=fps)
    {
        step = 0.0f;
        state++;
        if(state>=animation.size())
        {
            state=0;
        }
        animation[state].setPosition(pos.x+offsetX,pos.y+offsetY);
        return(true);
    }
    animation[state].setPosition(pos.x+offsetX,pos.y+offsetY);
    return (false);
}

bool Animation::update(float tick)
{
    if(fps>=0)
    {
        step+=tick;
        if(step>=fps)
        {
            step = 0.0f;
            state++;
            if(state>=animation.size())
            {
                state=0;
            }
            return(true);
        }
    }
    return (false);
}

sf::Sprite* Animation::getAnim()
{
    return (&animation[state]);
}
