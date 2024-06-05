#include "database.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Database::createTable(const std::string& name, const std::vector<std::string>& columns) {
    Table table;
    table.name = name;
    table.columns = columns;
    database.push_back(table);
    std::cout << "Table " << name << " created successfully!\n";
}

void Database::showTables() {
    if (database.empty()) {
        std::cout << "No tables in the database!\n";
        return;
    }
    std::cout << "Tables in the database:\n";
    for (const auto& table : database) {
        std::cout << " - " << table.name << "\n";
    }
}

void Database::describeTable(const std::string& table_name) {
    for (const auto& table : database) {
        if (table.name == table_name) {
            std::cout << "Table: " << table.name << "\nColumns: ";
            for (const auto& column : table.columns) {
                std::cout << column << " ";
            }
            std::cout << "\n";
            return;
        }
    }
    std::cout << "Table not found!\n";
}

void Database::printTable(const std::string& table_name) {
    for (const auto& table : database) {
        if (table.name == table_name) {
            std::cout << "Table: " << table.name << "\n";
            for (const auto& column : table.columns) {
                std::cout << column << "\t";
            }
            std::cout << "\n";
            for (const auto& row : table.data) {
                for (const auto& cell : row) {
                    std::cout << cell << "\t";
                }
                std::cout << "\n";
            }
            return;
        }
    }
    std::cout << "Table not found!\n";
}

bool Database::insertRow(const std::string& table_name, const std::vector<std::string>& row) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row.size() != table.columns.size()) {
                std::cout << "Error: Number of values does not match number of columns!\n";
                return false;
            }
            table.data.push_back(row);
            return true;
        }
    }
    std::cout << "Table not found!\n";
    return false;
}

bool Database::deleteRow(const std::string& table_name, int row_index) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row_index < 0 || row_index >= table.data.size()) {
                std::cout << "Error: Row index out of range!\n";
                return false;
            }
            table.data.erase(table.data.begin() + row_index);
            return true;
        }
    }
    std::cout << "Table not found!\n";
    return false;
}

bool Database::updateRow(const std::string& table_name, int row_index, const std::vector<std::string>& new_values) {
    for (auto& table : database) {
        if (table.name == table_name) {
            if (row_index < 0 || row_index >= table.data.size()) {
                std::cout << "Error: Row index out of range!\n";
                return false;
            }
            if (new_values.size() != table.columns.size()) {
                std::cout << "Error: Number of values does not match number of columns!\n";
                return false;
            }
            table.data[row_index] = new_values;
            return true;
        }
    }
    std::cout << "Table not found!\n";
    return false;
}

bool Database::addColumn(const std::string& table_name, const std::string& column_name, const std::string& default_value) {
    for (auto& table : database) {
        if (table.name == table_name) {
            table.columns.push_back(column_name);
            for (auto& row : table.data) {
                row.push_back(default_value);
            }
            return true;
        }
    }
    std::cout << "Table not found!\n";
    return false;
}

std::vector<std::vector<std::string>> Database::select(const std::string& table_name, const std::vector<std::string>& columns) {
    std::vector<std::vector<std::string>> result;
    for (const auto& table : database) {
        if (table.name == table_name) {
            std::vector<int> column_indices;
            for (const auto& column : columns) {
                auto it = std::find(table.columns.begin(), table.columns.end(), column);
                if (it != table.columns.end()) {
                    column_indices.push_back(std::distance(table.columns.begin(), it));
                }
                else {
                    std::cout << "Error: Column " << column << " not found in table " << table_name << "!\n";
                    return result;
                }
            }
            for (const auto& row : table.data) {
                std::vector<std::string> selected_row;
                for (int index : column_indices) {
                    selected_row.push_back(row[index]);
                }
                result.push_back(selected_row);
            }
            return result;
        }
    }
    std::cout << "Table not found!\n";
    return result;
}

int Database::countRows(const std::string& table_name) {
    for (const auto& table : database) {
        if (table.name == table_name) {
            return table.data.size();
        }
    }
    std::cout << "Table not found!\n";
    return -1;
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
        std::cout << "Error: One or both tables not found for inner join!\n";
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

void Database::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Unable to create or open file '" << filename << "'!\n";
        return;
    }

    for (const auto& table : database) {
        file << table.name << "\n";
        for (const auto& column : table.columns) {
            file << column << " ";
        }
        file << "\n";
        for (const auto& row : table.data) {
            for (const auto& cell : row) {
                file << cell << " ";
            }
            file << "\n";
        }
        file << "---\n";
    }

    file.close();
    std::cout << "Database saved successfully to '" << filename << "'!\n";
}

void Database::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Unable to open file '" << filename << "'!\n";
        return;
    }

    // Clear existing database
    database.clear();

    std::string line;
    while (std::getline(file, line)) {
        Table table;
        table.name = line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string column_name;
        while (iss >> column_name) {
            table.columns.push_back(column_name);
        }
        while (std::getline(file, line) && line != "---") {
            std::istringstream data_iss(line);
            std::vector<std::string> row;
            std::string cell;
            while (data_iss >> cell) {
                row.push_back(cell);
            }
            table.data.push_back(row);
        }
        database.push_back(table);
    }

    file.close();
    std::cout << "Database loaded successfully from '" << filename << "'!\n";
}