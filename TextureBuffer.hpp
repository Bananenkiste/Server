#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <iostream>
#include <map>
#include <sfml/Graphics.hpp>

class Animation;

class TextureBuffer
{
    public:
        static sf::Sprite* LoadTexture(std::string filename, bool middle);
        static sf::Sprite* LoadTexture(std::string filename, bool middle,float x,float y);
        static Animation LoadAnimation(std::string animname);
        static void loadFont(std::string fontname);
        static sf::Font getFont();
    private:
        static sf::Font stdfont;
        static void LoadTextureFromFile(std::string filename);
        static std::map<std::string,sf::Texture> textures;
        static std::map<std::string,Animation> animations;
};

#endif // TEXTUREBUFFER_H
