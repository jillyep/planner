// ================================================================================
// ================================================================================
// - File:    min_heap.cpp
// - Purpose: This file manages uses min heap to display the next task with the 
//            closest due date.
//
// Source Metadata
// - Author:  Jillian Webb
// - Date:    April 28, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jilly Webb Inc.
// ================================================================================
// ================================================================================

#include "include/min_heap.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>
#include "/usr/include/sqlite3.h"
#include "include/db.hpp"
#include <vector>
#include <queue>

// ================================================================================
// ================================================================================


void PriorityQueue::parseDateString()
{
    std::stringstream ss(due_date_string);
    ss >> std::get_time(&datetime, "%Y-%m-%d");
    if (ss.fail()) {
        std::cerr << "Error parsing date string: " << due_date_string << std::endl;
        }
}
// --------------------------------------------------------------------------------


std::vector<PriorityQueue> db_to_vector(DB& db)
{
    std::vector<PriorityQueue> rows;

    std::string sql = "SELECT ID, TASK, DUE_DATE FROM PLANNER;";

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db.db, sql.c_str(), -1, &stmt, NULL);

    if(rc != SQLITE_OK){
        std::cerr << "Failed to prepare SQL statement << sqlite3_errmsg(db.db)" << std::endl;
        return rows;
    }

    // Fetch rows one by one
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* task = sqlite3_column_text(stmt, 1);
        const unsigned char* due_date = sqlite3_column_text(stmt, 2);

        // Convert data to Priority Queue strut data types
        std::string task_str(reinterpret_cast<const char*>(task));
        std::string due_date_str(reinterpret_cast<const char*>(due_date));
        PriorityQueue row(id, task_str, due_date_str);

        rows.push_back(row);
    }

    sqlite3_finalize(stmt);

    return rows;
}
// --------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const PriorityQueue& pq) {
    os << "ID: " << pq.id << ", Task: " << pq.task << ", Due Date: "
       << std::put_time(&pq.datetime, "%c");
    return os;
}
// --------------------------------------------------------------------------------

bool operator<(const PriorityQueue& pq1, const PriorityQueue& pq2)
{
    // Compare the year first
    if (pq1.datetime.tm_year < pq2.datetime.tm_year)
        return true;
    else if (pq1.datetime.tm_year > pq2.datetime.tm_year)
        return false;

    // If years are equal, compare the month
    if (pq1.datetime.tm_mon < pq2.datetime.tm_mon)
        return true;
    else if (pq1.datetime.tm_mon > pq2.datetime.tm_mon)
        return false;

    // If months are equal, compare the day
    if (pq1.datetime.tm_mday < pq2.datetime.tm_mday)
        return true;
    else if (pq1.datetime.tm_mday > pq2.datetime.tm_mday)
        return false;

    // If day is also equal, return false (pq1 is not less than pq2)
    return false;
}
// --------------------------------------------------------------------------------

bool operator>(const PriorityQueue& pq1, const PriorityQueue& pq2)
{
    // Compare the year first
    if (pq1.datetime.tm_year > pq2.datetime.tm_year)
        return true;
    else if (pq1.datetime.tm_year < pq2.datetime.tm_year)
        return false;

    // If years are equal, compare the month
    if (pq1.datetime.tm_mon > pq2.datetime.tm_mon)
        return true;
    else if (pq1.datetime.tm_mon < pq2.datetime.tm_mon)
        return false;

    // If months are equal, compare the day
    if (pq1.datetime.tm_mday > pq2.datetime.tm_mday)
        return true;
    else if (pq1.datetime.tm_mday < pq2.datetime.tm_mday)
        return false;

    // If day is also equal, return false (pq1 is not less than pq2)
    return false;
}
// --------------------------------------------------------------------------------


PriorityQueue next_task(std::vector<PriorityQueue> vec)
{
    // Define priority queue
    std::priority_queue<PriorityQueue, std::vector<PriorityQueue>, std::greater<PriorityQueue>> pq;

    // Push vector on priority queue
    for (PriorityQueue item : vec){
        pq.push(item);
    }

    return pq.top();
}
// ================================================================================
// ================================================================================
//eof