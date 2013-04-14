#include "Encryption.hpp"

std::string Encryption::encrypt(std::string data)
{
    for(int x=data.length()-1;x>=0;--x)
    {
        data[x]+=2;
    }
    return(data);
}

std::string Encryption::decrypt(std::string data)
{
    for(int x=data.length()-1;x>=0;--x)
    {
        data[x]-=2;
    }
    return(data);
}
