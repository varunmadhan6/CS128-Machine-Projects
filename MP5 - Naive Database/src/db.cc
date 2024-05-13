#include "db.hpp"

void Database::CreateTable(const std::string& table_name) {
    if (tables_.find(table_name) == tables_.end()) {
        tables_[table_name] = new DbTable();
    }
}

void Database::DropTable(const std::string& table_name) {
    auto it = tables_.find(table_name);
    if (it == tables_.end()) {
        throw std::invalid_argument("key not found");
    } else {
        delete tables_[table_name];
        tables_.erase(table_name);
    }
}

DbTable& Database::GetTable(const std::string& table_name) {
    return *(tables_[table_name]);
}

Database::Database(const Database& rhs) {
    for (const auto& pair : rhs.tables_) {
        tables_[pair.first] = new DbTable(*pair.second);
    }
}

Database& Database::operator=(const Database& rhs) {
    if (this == &rhs) {
        return *this;
    }
    for (auto& pair : tables_) {
        delete pair.second; 
    }
    tables_.clear(); 

    for (const auto& pair : rhs.tables_) {
        auto* new_table = new DbTable(*pair.second); 
        tables_[pair.first] = new_table; 
    }
    return *this;
}

Database::~Database() {
    for (auto& pair : tables_) {
        delete pair.second; 
    }
    tables_.clear();
}