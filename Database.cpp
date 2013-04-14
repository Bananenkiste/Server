#include "Database.hpp"
#include "Mutex.hpp"

#include <cstring>


#include "sqlite3.h"

sf::Mutex Database::databaseAccess;

void Database::test()
{
    std::cout<<"test"<<std::endl;
    getData("Select * FROM players");
}


void Database::insertData(std::string data)
{
    databaseAccess.lock();


    databaseAccess.unlock();
}

std::string Database::getData(std::string request)
{
    std::cout<<"getdata"<<std::endl;
    //databaseAccess.lock();

    sqlite3 * db;
	int result;
	result = sqlite3_open( "game_db", &db );
    if ( result == SQLITE_OK )
    {
        const char * rqst = request.c_str();
        const char * error = NULL;

        sqlite3_stmt* statement;
        result = sqlite3_prepare_v2(db,rqst,strlen(rqst)+1,&statement, &error);
        if(result==SQLITE_OK)
        {
            int columns = sqlite3_column_count( statement );

            /*while ( sqlite3_step( statement ) == SQLITE_ROW )
            {
				std::cout << "Row : " ;
				for ( int c = 0; c < columns; c++ )
				{
					const char * cell = (const char *)sqlite3_column_text( statement, c );
					std::cout << cell << " ";
				}
				std::cout << std::endl;
			}*/
        }

    }
    else
    {
        std::cout<<"Database problem"<<std::endl;
    }
    sqlite3_close(db);

	//databaseAccess.unlock();
}
