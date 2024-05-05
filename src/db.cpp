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
#include "/usr/include/sqlite3.h"
#include <exception>
#include <iomanip>

// ================================================================================
// ================================================================================

void DB::checkDBErrors() {
    if( rc ){
        // Show error message
        std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
    }
}
// --------------------------------------------------------------------------------

int DB::callback(void* not_used, int argc, char** argv, char** azColName)
{
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
// --------------------------------------------------------------------------------


 //   public:

DB::DB(std::string filename) : 
    filename(filename),
    error_msg(nullptr),
    rc(0) 
{
    rc = sqlite3_open(filename.c_str(), &db);
    checkDBErrors();
}
// --------------------------------------------------------------------------------
DB::~DB()
{
    DB::closeDB();
    std::cout << "Planner has been successfully closed.\n";
}
// --------------------------------------------------------------------------------

void DB::closeDB(){
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}
// --------------------------------------------------------------------------------

int DB::createPlanner() 
{
    std::string sql = "CREATE TABLE IF NOT EXISTS PLANNER("
                        "ID INTEGER PRIMARY KEY, "
                        "TASK TEXT NOT NULL, "
                        "DUE_DATE VARCHAR(10) );";

rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg);
if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        closeDB(); 
        return rc; 
    }
std::cout << "Table created successfully" << std::endl;
return SQLITE_OK;
}
// --------------------------------------------------------------------------------


int DB::insertTask(std::string& task, std::string& due_date)
{
    // Prepare SQL Statement with Placeholders
    std::string sql = "INSERT INTO PLANNER (ID, TASK, DUE_DATE) VALUES (NULL, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Handle error
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return rc; // Return error code
    }

    // Bind Parameters
    rc = sqlite3_bind_text(stmt, 1, task.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        // Handle error
        std::cerr << "Error binding parameter 1: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt); // Clean up resources
        return rc; // Return error code
    }

    rc = sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        // Handle error
        std::cerr << "Error binding parameter 1: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt); // Clean up resources
        return rc; // Return error code
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // Handle error
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt); // Clean up resources
        return rc; // Return error code
    }

    //Finalize the statement
    sqlite3_finalize(stmt);
    std::cout << "Row inserted successfully\n";
    return SQLITE_OK;
}   
// --------------------------------------------------------------------------------

int DB::completeTask(int row_num)
{
    std::string sql_delete = "DELETE FROM PLANNER WHERE ID = (?);";
    sqlite3_stmt* stmt_delete;
    rc = sqlite3_prepare_v2(db, sql_delete.c_str(), -1, &stmt_delete, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing delete statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    rc = sqlite3_bind_int(stmt_delete, 1, row_num);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding parameter for delete statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt_delete);
        return rc;
    }

    rc = sqlite3_step(stmt_delete);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing delete statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt_delete);
        return rc;
    }

    sqlite3_finalize(stmt_delete);

    // Update the IDs of remaining rows
    std::string sql_update = "UPDATE PLANNER SET ID = ID - 1 WHERE ID > ?;";
    sqlite3_stmt* stmt_update;
    rc = sqlite3_prepare_v2(db, sql_update.c_str(), -1, &stmt_update, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    rc = sqlite3_bind_int(stmt_update, 1, row_num);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding parameter for update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt_update);
        return rc;
    }

    rc = sqlite3_step(stmt_update);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt_update);
        return rc;
    }

    sqlite3_finalize(stmt_update);
    std::cout << "Delete successful.\n";

    return SQLITE_OK;
}
// --------------------------------------------------------------------------------

int DB::printPlanner()
{ 
    std::string sql = "SELECT * FROM PLANNER;";

    sqlite3_exec(db, sql.c_str(), callback, NULL, NULL);

    return 0;
}
// --------------------------------------------------------------------------------

int DB::updatePlanner(UpdateRow& updated_row)
{
 std::string sql = "UPDATE PLANNER SET " + updated_row.set_column_name + " = ? WHERE " +
                      updated_row.id_column_name + " = ?;";

    // Prepare SQL Statement with Placeholders
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Bind Parameters
    rc = sqlite3_bind_text(stmt, 1, updated_row.set_new_value.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding parameter for update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    rc = sqlite3_bind_text(stmt, 2, updated_row.id_column_value.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding parameter for update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    //Finalize the statement and close db
    sqlite3_finalize(stmt);
    std::cout << "Update successful\n";

    return SQLITE_OK;       
}
// --------------------------------------------------------------------------------

int DB::bulkInsertTasks(std::vector<Task>& tasks)
{
    for(auto& task : tasks)
    {
        rc = insertTask(task.task, task.due_date);
        // checkDBErrors();
        if(rc != SQLITE_OK) {
            std::cerr << "Error inserting task: " << sqlite3_errmsg(db) << std::endl;
            return rc; // Return the error code
        }
    }
    std::cout << "Bulk insert successful\n";   
    return SQLITE_OK;
}
// ================================================================================
// ================================================================================
//eof