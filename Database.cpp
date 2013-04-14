#include "Database.hpp"

#include <cstring>


#include "sqlite3.h"

sf::Mutex Database::databaseAccess;

void Database::test()
{
    std::cout<<getData("Select * FROM players")<<std::endl;
    std::cout<<getData("Select * FROM players")<<std::endl;
    Login("Daniel","asdf");
    Login("Martin","asdf");
    insertData("INSERT INTO main.usr(usr_name,usr_password) VALUES ('Martin','asdf')");
    Login("Martin","asdf");
}


void Database::insertData(std::string data)
{
    databaseAccess.lock();

    //"INSERT INTO main.usr(usr_name,usr_password) VALUES ("Daniel","asdf")"

    sqlite3 * db;
    int result;
    result = sqlite3_open( "bomberman_db", &db );
    if ( result == SQLITE_OK )
    {
        const char * rqst = data.c_str();
        char * error = NULL;
        result = sqlite3_exec(db,rqst,NULL,NULL,&error);
        if(result!=SQLITE_OK)
        {
            std::cout<<"insert error"<<std::endl;
        }
    }
    else
    {
        std::cout<<"database error"<<std::endl;
    }
    sqlite3_close(db);
    databaseAccess.unlock();
}

std::string Database::getData(std::string request)
{
    databaseAccess.lock();

    sqlite3 * db;
	int result;
	std::string data="";
	result = sqlite3_open( "bomberman_db", &db );
    if ( result == SQLITE_OK )
    {
        const char * rqst = request.c_str();
        int row,colum;
        char** out;
        char * error = NULL;

        result = sqlite3_get_table(db,rqst,&out,&row,&colum,&error);

        if(result==SQLITE_OK)
        {
            for(int y=1;y<row+1;++y)
            {
                for(int x=0;x<colum;++x)
                {
                    data+=out[(y*colum)+x];
                    data+="|";
                }
            }
        }
        sqlite3_free_table(out);
    }
    else
    {
        std::cout<<"Database problem"<<std::endl;
    }
    sqlite3_close(db);
	databaseAccess.unlock();
	return(data);
}

bool Database::Login(std::string name, std::string pw)
{
    std::string request = "select * from usr where usr_name='"+name+"' AND usr_password='"+pw+"';";
    std::string result = getData(request);
    std::cout<<request<<":"<<result.length()<<std::endl;
    if(result.length()==0)
    {
        return(false);
    }
    return(true);
}

