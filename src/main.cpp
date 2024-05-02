// ================================================================================
// ================================================================================
// - File:    main.cpp
// - Purpose: main integration file for the planner program.
//
// Source Metadata
// - Author:  Jillian Webb
// - Date:    April 28, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jilly Webb Inc.
// ================================================================================
// ================================================================================
#include "include/db.hpp"
#include <iostream>
#include <string>
#include <sqlite3.h>
// ================================================================================
// ================================================================================


int main() 
{
    // Call the createDB function
    // const char* filename{"../data/planner.db"};
   
    // createDB(filename);
    // createTable(filename);
    // std::string task = "Complete assignment";
    // std::string dueDate = "2024-05-01";

    // int result = insertRow(filename, task, dueDate);
    // if (result == SQLITE_OK) {
    //     std::cout << "Row inserted successfully." << std::endl;
    // } else {
    //     std::cerr << "Error inserting row: " << result << std::endl;
    // }
    const char* filename{"../data/planner.db"};
    // int row_num{2};
    // int result = deleteRow(filename, row_num);
    // if (result == SQLITE_OK) {
    //     std::cout << "Row deleted successfully." << std::endl;
    // } else {
    //     std::cerr << "Error deleting row: " << result << std::endl;
    // }

    printTable(filename);
    return 0;
}
// ================================================================================
// ================================================================================
//eof