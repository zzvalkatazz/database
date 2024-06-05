#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

struct Table {
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> data;
};

class Database {
public:
    void createTable(const std::string& name, const std::vector<std::string>& columns);
    void showTables();
    void describeTable(const std::string& table_name);
    void printTable(const std::string& table_name);
    bool insertRow(const std::string& table_name, const std::vector<std::string>& row);
    bool deleteRow(const std::string& table_name, int row_index);
    bool updateRow(const std::string& table_name, int row_index, const std::vector<std::string>& new_values);
    bool addColumn(const std::string& table_name, const std::string& column_name, const std::string& default_value);
    std::vector<std::vector<std::string>> select(const std::string& table_name, const std::vector<std::string>& columns);
    int countRows(const std::string& table_name);
    Table innerJoin(const std::string& table1_name, const std::string& table2_name);

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::vector<Table> database;
};

#endif // DATABASE_H