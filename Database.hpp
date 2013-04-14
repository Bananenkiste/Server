#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <sfml/System.hpp>


class Database
{
    public:
        static void insertData(std::string data);
        static std::string getData(std::string request);
        static bool Login(std::string name,std::string pw);
        static void test();
    protected:
    private:
        static sf::Mutex databaseAccess;
};

#endif // DATABASE_H
