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
    
    std::string filename{"../data/planner.db"};

    DB db(filename);
    // db.createPlanner();

    // std::string task = "Clean my room";
    // std::string due_date = "2024-05-04";
    // db.insertTask(task, due_date);

    // std::vector<Task> task_vec;

    // Task task1 = {"Feed the Marge", "2024-02-01"};
    // Task task2 = {"Feed the Jonald", "2023-01-02"};
    // Task task3 = {"Marges Birthday", "2017-04-25"};
    // Task task4 = {"Learn Cpp", "2024-06-24"};
    // Task task5 = {"Make cool things", "2025-09-01"};
    // Task task6 = {"Learn piano", "2024-01-02"};

    // task_vec.insert(task_vec.end(), {task1, task2, task3, task4, task5, task6});
    
    // db.bulkInsertTasks(task_vec);

    // db.completeTask(2);

    UpdateRow update{"DUE_DATE", "2027-10-21", "ID", "1"};
    db.updatePlanner(update);
    db.printPlanner();

    return 0;
}
// ================================================================================
// ================================================================================
//eof