#include "TextureBuffer.hpp"
#include "Animation.hpp"


std::map<std::string,sf::Texture> TextureBuffer::textures;
std::map<std::string,Animation> TextureBuffer::animations;
sf::Font TextureBuffer::stdfont;

sf::Sprite* TextureBuffer::LoadTexture(std::string filename,bool middle,float x,float y)
{
    sf::Sprite* newSprite = LoadTexture(filename,middle);
    newSprite->setPosition(x,y);
    return(newSprite);
}


sf::Sprite* TextureBuffer::LoadTexture(std::string filename,bool middle)
{
    std::map<std::string,sf::Texture>::iterator it;
    it=textures.find(filename);
    if(it!=TextureBuffer::textures.end())
    {
            sf::Sprite* temp = new sf::Sprite(it->second);
            if(middle)
            {
                sf::Vector2u vec = it->second.getSize();
                temp->setOrigin(vec.x/2,vec.y/2);
            }
            return(temp);
    }
    else
    {
        LoadTextureFromFile(filename);
        it=textures.find(filename);
        if(it!=TextureBuffer::textures.end())
        {
            sf::Sprite* temp = new sf::Sprite(it->second);
            if(middle)
            {
                sf::Vector2u vec = it->second.getSize();
                temp->setOrigin(vec.x/2,vec.y/2);
            }
            return(temp);
        }
        else
        {
            return(NULL);
        }
    }
}

void TextureBuffer::LoadTextureFromFile(std::string filename)
{
    sf::Texture newImg;
    std::string nfilename = filename;
    nfilename.append(".png");
    std::string path = "Images\\";
    path.append(nfilename);
    if(newImg.loadFromFile(path))
    {
        TextureBuffer::textures.insert(std::pair<std::string,sf::Texture>(filename,newImg));
    }
}

Animation TextureBuffer::LoadAnimation(std::string animname)
{
    std::map<std::string,Animation>::iterator it = animations.find(animname);
    if(it!=animations.end())
    {
        return((*it).second);
    }
    else
    {
        TextureBuffer::animations.insert(std::pair<std::string,Animation>(animname,Animation(animname,0)));
        it = animations.find(animname);
        if(it!=animations.end())
        {
            return((*it).second);
        }
    }
}

sf::Font TextureBuffer::getFont()
{
    return stdfont;
}

void TextureBuffer::loadFont(std::string fontname)
{
    std::string nfilename = fontname;
    nfilename.append(".ttf");
    std::string path = "Font\\";
    path.append(nfilename);

    if(!stdfont.loadFromFile(path))
    {
        std::cout<<"Font is missing"<<std::endl;
    }
}
