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
#include "include/min_heap.hpp"
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <iomanip>
// ================================================================================
// ================================================================================

int main() 
{
    
    std::string filename{"../data/planner.db"};

    DB db(filename);

    std::vector<PriorityQueue> vec = db_to_vector(db);

    for (auto& item : vec){
        std::cout << item << std::endl;
    }

    PriorityQueue next = next_task(vec);

    std::cout << std::endl << "Your next task is: " << next << std::endl << std::endl;

    return 0;

}
// ================================================================================
// ================================================================================
//eof