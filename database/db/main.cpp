#include <iostream>
#include <string>
#include <vector>
#include "database.h"

int main() {
    Database db;
    std::string command;
    // Loading database from file
    db.loadFromFile("database.txt");
    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> command;

        if (command == "createTable") {
            std::string table_name;
            std::vector<std::string> columns;
            std::string column;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cin.ignore(); // Ignore newline character left in the buffer
            std::cout << "Enter column names (end with 'done'): ";
            while (std::cin >> column && column != "done") {
                columns.push_back(column);
            }
            db.createTable(table_name, columns);
        }
        else if (command == "showTables") {
            db.showTables();
        }
        else if (command == "describeTable") {
            std::string table_name;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            db.describeTable(table_name);
        }
        else if (command == "printTable") {
            std::string table_name;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            db.printTable(table_name);
        }
        else if (command == "insertRow") {
            std::string table_name;
            std::vector<std::string> row;
            std::string value;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cin.ignore(); // Ignore newline character left in the buffer
            std::cout << "Enter row values (end with 'done'): ";
            while (std::cin >> value && value != "done") {
                row.push_back(value);
            }
            db.insertRow(table_name, row);
        }
        else if (command == "deleteRow") {
            std::string table_name;
            int row_index;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cout << "Enter row index: ";
            std::cin >> row_index;
            db.deleteRow(table_name, row_index);
        }
        else if (command == "updateRow") {
            std::string table_name;
            int row_index;
            std::vector<std::string> new_values;
            std::string value;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cout << "Enter row index: ";
            std::cin >> row_index;
            std::cin.ignore(); // Ignore newline character left in the buffer
            std::cout << "Enter new values (end with 'done'): ";
            while (std::cin >> value && value != "done") {
                new_values.push_back(value);
            }
            db.updateRow(table_name, row_index, new_values);
        }
        else if (command == "addColumn") {
            std::string table_name, column_name, default_value;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cout << "Enter column name: ";
            std::cin >> column_name;
            std::cout << "Enter default value: ";
            std::cin >> default_value;
            db.addColumn(table_name, column_name, default_value);
        }
        else if (command == "select") {
            std::string table_name;
            std::vector<std::string> columns;
            std::string column;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            std::cin.ignore(); // Ignore newline character left in the buffer
            std::cout << "Enter column names to select (end with 'done'): ";
            while (std::cin >> column && column != "done") {
                columns.push_back(column);
            }
            auto selected_data = db.select(table_name, columns);
            std::cout << "Selected Data:\n";
            for (const auto& row : selected_data) {
                for (const auto& cell : row) {
                    std::cout << cell << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (command == "countRows") {
            std::string table_name;
            std::cout << "Enter table name: ";
            std::cin >> table_name;
            int row_count = db.countRows(table_name);
            if (row_count >= 0) {
                std::cout << "Row count: " << row_count << std::endl;
            }
            else {
                std::cout << "Table not found!" << std::endl;
            }
        }
        else if (command == "innerJoin") {
            std::string table1_name, table2_name;
            std::cout << "Enter first table name: ";
            std::cin >> table1_name;
            std::cout << "Enter second table name: ";
            std::cin >> table2_name;
            Table result = db.innerJoin(table1_name, table2_name);
            std::cout << "Inner Join Result:\n";
            for (const auto& column : result.columns) {
                std::cout << column << " ";
            }
            std::cout << std::endl;
            for (const auto& row : result.data) {
                for (const auto& cell : row) {
                    std::cout << cell << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Unknown command!" << std::endl;
        }
    }
    // Saving database to file
    db.saveToFile("database.txt");
    std::cout << "\n";

    // Clearing current database
    db = Database();

 
}
