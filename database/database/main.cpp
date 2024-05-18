#include "database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

bool Database::openDatabase(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open database file!" << std::endl;
        return false;
    }

    // Clear the existing database
    database.clear();

    // Read each table from the file and populate the 'database' vector
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Table table;
        // Read table name
        iss >> table.name;
        // Read column names
        std::string column_name;
        while (iss >> column_name) {
            table.columns.push_back(column_name);
        }
        // Read table data
        std::vector<std::vector<std::string>> table_data;
        while (std::getline(file, line) && line != "---") {
            std::istringstream data_iss(line);
            std::vector<std::string> row;
            std::string cell;
            while (data_iss >> cell) {
                row.push_back(cell);
            }
            table_data.push_back(row);
        }
        table.data = table_data;
        // Add table to database
        database.push_back(table);
    }

    file.close();
    std::cout << "Database loaded successfully!" << std::endl;
    return true;
}

void Database::closeDatabase() {
    saveDatabase("database.db");
    database.clear();
    std::cout << "Database closed successfully!" << std::endl;
}

void Database::saveDatabase(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to create or open database file!" << std::endl;
        return;
    }

    // Write each table to the file
    for (const auto& table : database) {
        file << table.name << " ";
        for (const auto& column : table.columns) {
            file << column << " ";
        }
        file << std::endl;
        for (const auto& row : table.data) {
            for (const auto& cell : row) {
                file << cell << " ";
            }
            file << std::endl;
        }
        file << "---" << std::endl;
    }

    file.close();
    std::cout << "Database saved successfully!" << std::endl;
}

bool Database::importTable(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'!" << std::endl;
        return false;
    }

    Table table;
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> table.name;
        std::string column_name;
        while (iss >> column_name) {
            table.columns.push_back(column_name);
        }
    }
    else {
        std::cerr << "Error: Empty file!" << std::endl;
        file.close();
        return false;
    }

    table.data.clear();
    while (std::getline(file, line)) {
        if (line == "---") break;
        std::istringstream data_iss(line);
        std::vector<std::string> row;
        std::string cell;
        while (data_iss >> cell) {
            row.push_back(cell);
        }
        table.data.push_back(row);
    }

    database.push_back(table);
    file.close();
    std::cout << "Table imported successfully!" << std::endl;
    return true;
}

void Database::showTables() {
    if (database.empty()) {
        std::cout << "No tables in the database!" << std::endl;
        return;
    }
    std::cout << "Tables in the database:" << std::endl;
    for (const auto& table : database) {
        std::cout << " - " << table.name << std::endl;
    }
}

void Database::describeTable(const std::string& table_name) {
    bool found = false;
    for (const auto& table : database) {
        if (table.name == table_name) {
            found = true;
            std::cout << "Table: " << table.name << std::endl;
            std::cout << "Columns: ";
            for (const auto& column : table.columns) {
                std::cout << column << " ";
            }
            std::cout << std::endl;
            break;
        }
    }
    if (!found) {
        std::cout << "Table not found!" << std::endl;
    }
}

void Database::printTable(const std::string& table_name, int page_size) {
    bool found = false;
    for (const auto& table : database) {
        if (table.name == table_name) {
            found = true;
            std::cout << "Table: " << table.name << std::endl;
            std::cout << "Columns: ";
            for (const auto& column : table.columns) {
                std::cout << std::setw(15) << std::left << column;
            }
            std::cout << std::endl;
            int count = 0;
            for (const auto& row : table.data) {
                for (const auto& cell : row) {
                    std::cout << std::setw(15) << std::left << cell;
                }
                std::cout << std::endl;
                if (++count % page_size == 0) {
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cout << std::endl;
                }
            }
            break;
        }
    }
    if (!found) {
        std::cout << "Table not found!" << std::endl;
    }
}

bool Database::exportTable(const std::string& table_name, const std::string& filename) {
    for (const auto& table : database) {
        if (table.name == table_name) {
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: Unable to create or open file '" << filename << "'!" << std::endl;
                return false;
            }
            // Write column names
            for (size_t i = 0; i < table.columns.size(); ++i) {
                file << table.columns[i];
                if (i != table.columns.size() - 1) {
                    file << ",";
                }
            }
            file << std::endl;
            // Write table data
            for (const auto& row : table.data) {
                for (size_t i = 0; i < row.size(); ++i) {
                    file << row[i];
                    if (i != row.size() - 1) {
                        file << ",";
                    }
                }
                file << std::endl;
            }
            file.close();
            std::cout << "Table exported successfully to '" << filename << "'!" << std::endl;
            return true;
        }
    }
    std::cerr << "Error: Table '" << table_name << "' not found!" << std::endl;
    return false;
}

int Database::countRows(const std::string& table_name) {
    for (const auto& table : database) {
        if (table.name == table_name) {
            return table.data.size();
        }
    }
    return -1; // Table not found
}

Table Database::innerJoin(const std::string& table1_name, const std::string& table2_name) {
    Table result_table;
    bool found_table1 = false, found_table2 = false;
    for (const auto& table : database) {
        if (table.name == table1_name) {
            result_table.columns.insert(result_table.columns.end(), table.columns.begin(), table.columns.end());
            found_table1 = true;
        }
        if (table.name == table2_name) {
            result_table.columns.insert(result_table.columns.end(), table.columns.begin(), table.columns.end());
            found_table2 = true;
        }
    }
    if (!found_table1 || !found_table2) {
        std::cerr << "Error: One or both tables not found for inner join!" << std::endl;
        return result_table;
    }

    for (const auto& row1 : database) {
        if (row1.name == table1_name) {
            for (const auto& row2 : database) {
                if (row2.name == table2_name) {
                    for (const auto& data1 : row1.data) {
                        for (const auto& data2 : row2.data) {
                            std::vector<std::string> combined_data = data1;
                            combined_data.insert(combined_data.end(), data2.begin(), data2.end());
                            result_table.data.push_back(combined_data);
                        }
                    }
                }
            }
        }
    }
    return result_table;
}

bool Database::insertRow(const std::string& table_name, const std::vector<std::string>& row) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row.size() != table.columns.size()) {
                std::cerr << "Error: Number of values does not match number of columns!" << std::endl;
                return false;
            }
            table.data.push_back(row);
            return true;
        }
    }
    std::cerr << "Error: Table '" << table_name << "' not found!" << std::endl;
    return false;
}

bool Database::deleteRow(const std::string& table_name, int row_index) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row_index >= 0 && row_index < table.data.size()) {
                table.data.erase(table.data.begin() + row_index);
                return true;
            }
            else {
                std::cerr << "Error: Row index out of range!" << std::endl;
                return false;
            }
        }
    }
    std::cerr << "Error: Table '" << table_name << "' not found!" << std::endl;
    return false;
}

bool Database::updateRow(const std::string& table_name, int row_index, const std::vector<std::string>& new_values) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row_index >= 0 && row_index < table.data.size()) {
                if (new_values.size() != table.columns.size()) {
                    std::cerr << "Error: Number of values does not match number of columns!" << std::endl;
                    return false;
                }
                table.data[row_index] = new_values;
                return true;
            }
            else {
                std::cerr << "Error: Row index out of range!" << std::endl;
                return false;
            }
        }
    }
    std::cerr << "Error: Table '" << table_name << "' not found!" << std::endl;
    return false;
}