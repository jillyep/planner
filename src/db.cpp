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
#include <string_view>
#include <iomanip>
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
    "ID INTEGER PRIMARY KEY, "
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
// --------------------------------------------------------------------------------


int insertRow(const char* filename,
                    std::string& task,
                    std::string& due_date)
{
    sqlite3* db;

    // Open the database connection
    int exit = sqlite3_open(filename, &db);
    if (exit != SQLITE_OK){
        sqlite3_close(db);
        return exit;
    }

    // Prepare SQL Statement with Placeholders
    std::string sql = "INSERT INTO PLANNER (ID, TASK, DUE_DATE) VALUES (NULL, ?, ?);";
    sqlite3_stmt* stmt;
    exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    // Bind Parameters
    exit = sqlite3_bind_text(stmt, 1, task.c_str(), -1, SQLITE_STATIC);
    if (exit != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    exit = sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
    if (exit != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    // Execute the statement
    exit = sqlite3_step(stmt);
    if(exit != SQLITE_DONE){
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    //Finalize the statement and close db
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return SQLITE_OK;
}
// --------------------------------------------------------------------------------


int deleteRow(const char* filename, int& row_num)
{
    sqlite3* db;

    int exit = sqlite3_open(filename, &db);
    if (exit != SQLITE_OK){
        sqlite3_close(db);
        return exit;
    }

    std::string sql = "DELETE FROM PLANNER WHERE ID = (?);";
    sqlite3_stmt* stmt;
    exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK){
        std::cerr << "Error DELETE" << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    exit = sqlite3_bind_int(stmt, 1, row_num);
    if (exit != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    // Execute the statement
    exit = sqlite3_step(stmt);
    if(exit != SQLITE_DONE){
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return exit;
    }

    //Finalize the statement and close db
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return SQLITE_OK;


}
// --------------------------------------------------------------------------------


int callback(void* not_used, int argc, char** argv, char** azColName)
{
    // for (int i = 0; i < argc; i++){
    //     std::cout << azColName[i] << ": " << argv[i] << std::endl;
    // }
    // std::cout << std::endl;

    // return 0;

     // Print column headers
    static bool headersPrinted = false;

    // Print column headers and separator only once
    if (!headersPrinted) {
        for (int i = 0; i < argc; i++) {
            std::cout << std::setw(20) << azColName[i] << " | ";
        }
        std::cout << std::endl;

        for (int i = 0; i < argc; i++) {
            std::cout << std::setw(20) << std::setfill('-') << "" << " | ";
        }
        std::cout << std::endl;

        headersPrinted = true;
    }

    // Print rows
    for (int i = 0; i < argc; i++) {
        std::cout << std::setw(20) << std::setfill(' ') << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << std::endl;

    return 0;
}
// --------------------------------------------------------------------------------


int printTable(const char* filename)
{
    sqlite3* db;

    // Open the database
    int exit = sqlite3_open(filename, &db);
    if (exit != SQLITE_OK){
        sqlite3_close(db);
        return exit;
    }

    std::string sql = "SELECT * FROM PLANNER;";

    sqlite3_exec(db, sql.c_str(), callback, NULL, NULL);

    return 0;

}