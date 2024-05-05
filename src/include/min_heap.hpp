// ================================================================================
// ================================================================================
// - File:    min_heap.hpp
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

#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <string>
#include <ctime>
#include <vector>
#include "/usr/include/sqlite3.h"
#include "db.hpp"
// --------------------------------------------------------------------------------

struct PriorityQueue
{
    int id;
    std::string task;
    std::string due_date_string;
    std::tm datetime;

    PriorityQueue(int input_id, std::string intput_task, std::string input_due_date_string) : 
                  id(input_id), task(intput_task), due_date_string(input_due_date_string)
                  {
                    parseDateString();
                  }
    void parseDateString();

    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& datetime);

    friend bool operator<(const PriorityQueue& pq1, const PriorityQueue& pq2);

    friend bool operator>(const PriorityQueue& pq1, const PriorityQueue& pq2);

    bool operator()(const PriorityQueue& pq1, const PriorityQueue& pq2) const
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
};
// --------------------------------------------------------------------------------

std::vector<PriorityQueue> db_to_vector(DB& db);

// --------------------------------------------------------------------------------

PriorityQueue next_task(std::vector<PriorityQueue>vec);

// --------------------------------------------------------------------------------
#endif
