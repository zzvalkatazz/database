#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

// Data type definition
enum class DataType { INTEGER, FLOAT, STRING };

// Table structure
struct Table {
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> data;
};

class Database {
public:
    bool openDatabase(const std::string& path);
    void closeDatabase();
    void saveDatabase(const std::string& filename);

    bool importTable(const std::string& filename);
    void showTables();
    void describeTable(const std::string& table_name);
    void printTable(const std::string& table_name, int page_size = 10);
    bool exportTable(const std::string& table_name, const std::string& filename);

    // Additional function prototypes...
    int countRows(const std::string& table_name);
    Table innerJoin(const std::string& table1_name, const std::string& table2_name);
    bool insertRow(const std::string& table_name, const std::vector<std::string>& row);
    bool deleteRow(const std::string& table_name, int row_index);
    bool updateRow(const std::string& table_name, int row_index, const std::vector<std::string>& new_values);

private:
    std::vector<Table> database;
};

#endif // DATABASE_H