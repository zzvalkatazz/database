#include <iostream>
#include "database.h"

int main() {
    Database db;

    // Test database operations
    db.openDatabase("database.db");

    db.showTables();
    db.describeTable("Table1");
    db.printTable("Table1");

    db.importTable("new_table.csv");
    db.exportTable("Table1", "exported_table.csv");

    std::vector<std::string> new_row = {"value1", "value2", "value3"};
    db.insertRow("Table1", new_row);

    db.deleteRow("Table1", 2);

    std::vector<std::string> updated_values = {"updated_value1", "updated_value2", "updated_value3"};
    db.updateRow("Table1", 1, updated_values);

    // Test new functions
    db.addColumn("Table1", "NewColumn", "default_value");

    std::vector<std::string> selected_columns = {"Column1", "NewColumn"};
    auto selected_data = db.select("Table1", selected_columns);

    std::cout << "Selected Data:" << std::endl;
    for (const auto& row : selected_data) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    db.closeDatabase();

    return 0;
}
