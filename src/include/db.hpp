#ifndef DB_H
#define DB_H
#include <string>

int createDB(const char* filename);

int createTable(const char* filename);

int insertRow(const char* filename, std::string& task, std::string& due_date);

int deleteRow(const char* filename, int& row_num);

int callback(void* not_used, int argc, char** argv, char** azColName);

int printTable(const char* filename);

#endif