// ================================================================================
// ================================================================================
// - File:    db.cpp
// - Purpose: This file manages a sqlite3 database.
//
// Source Metadata
// - Author:  Jillian Webb
// - Date:    April 28, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jilly Webb Inc.
// ================================================================================
// ================================================================================

#include "include/db.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include "/usr/include/sqlite3.h"
#include <exception>
// ================================================================================
// ================================================================================


int createDB(const char* filename)
{
    sqlite3* db;
    int exit = 0;

    exit = sqlite3_open(filename, &db);
    if (exit != SQLITE_OK){
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return exit;
    }
    std::cout << "Database: " << filename << " opened successfully" << std::endl;

    sqlite3_close(db);

    if (exit != SQLITE_OK) {
        std::cerr << "Error closing database: " << sqlite3_errmsg(db) << std::endl;
        return exit;
    }

    std::cout << "Database: " << filename << " closed successfully" << std::endl;

    return 0;
}
// --------------------------------------------------------------------------------


int createTable(const char* filename)
{
    sqlite3* db;

    std::string sql = "CREATE TABLE IF NOT EXISTS PLANNER("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "TASK TEXT NOT NULL, "
    "DUE_DATE VARCHAR(10) );";

    try
    {
        int exit = 0;
        exit = sqlite3_open(filename, &db);

        char* messageError;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK){
            std::cerr << "Error with Create Table" << std::endl;
            sqlite3_free(messageError);
        }
        else
            std::cout << "Table created successfully" << std::endl;
        sqlite3_close(db);
    }
    catch(const std::exception& e){
        std::cerr << e.what();
    }
    return 0;
}