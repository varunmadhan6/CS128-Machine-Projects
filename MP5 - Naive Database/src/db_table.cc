#include "db_table.hpp"

// const unsigned int kRowGrowthRate = 2;
void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
    if (col_descs_.size() == row_col_capacity_) {
        Resize();
    }
    col_descs_.push_back(col_desc);
    size_t idx = col_descs_.size() - 1;
    for (unsigned int k = 0; k < next_unique_id_; ++k) {
        auto& row = rows_[k];
        DataType data_type = Extract(col_desc);
        if (data_type == DataType::kString) {
            row[idx] = static_cast<void*>(new std::string());
        } else if (data_type == DataType::kDouble) {
            row[idx] = static_cast<void*>(new double(0.0));
        } else {
            row[idx] = static_cast<void*>(new int(0));
        }
    }
}

void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
    if (col_idx >= col_descs_.size()) {
        throw std::out_of_range("out of range index");
    } else if (col_descs_.size() == 1 && !rows_.empty()) {
        throw std::runtime_error("rows and columns are not same size");
    } else {
        for (auto& row : rows_) { 
            DataType data_type = Extract(col_descs_.at(col_idx));
            if (data_type == DataType::kString) {
                delete static_cast<std::string*>(row.second[col_idx]);
            } else if (data_type == DataType::kDouble) {
                delete static_cast<double*>(row.second[col_idx]);
            } else {
                delete static_cast<int*>(row.second[col_idx]);
            }
            DeleteRowsFromMap(row.second, col_idx);
        }
    }
    col_descs_.erase(col_descs_.begin() + col_idx);
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
    if (col_data.size() != col_descs_.size()) {
        throw std::invalid_argument("sizes do not match");
    }
    rows_[next_unique_id_] = new void*[row_col_capacity_];
    void** array = rows_[next_unique_id_];
    std::vector<std::string> vec = ListIntoVector(col_data);
    for (size_t i = 0; i < col_descs_.size(); ++i) {
        DataType data_type = Extract(col_descs_.at(i));
        if (data_type == DataType::kString) {
            array[i] = static_cast<void*>(new std::string(vec.at(i)));
        } else if (data_type == DataType::kDouble) {
            double double_value = std::stod(vec.at(i));
            array[i] = static_cast<void*>(new double(double_value));
        } else {
            int int_value = std::stoi(vec.at(i));
            array[i] = static_cast<void*>(new int(int_value));
        }
    }
    next_unique_id_++;
}

void DbTable::DeleteRowById(unsigned int id) {
    auto it = rows_.find(id);
    if (it == rows_.end()) {
        throw std::invalid_argument("id not found");
    } else {
        void** array = rows_[id];
        for (size_t i = 0; i < col_descs_.size(); ++i) {
            DataType data_type = Extract(col_descs_.at(i));
            if (data_type == DataType::kString) {
                delete static_cast<std::string*>(array[i]);
            } else if (data_type == DataType::kDouble) {
                delete static_cast<double*>(array[i]);
            } else {
                delete static_cast<int*>(array[i]);
            } 
        }
        delete[] array;
        rows_.erase(id);
    }
}

DbTable::DbTable(const DbTable& rhs) {
    next_unique_id_ = rhs.next_unique_id_;
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;
    for (const auto& pair : rhs.rows_) {
        unsigned int key = pair.first;
        void** original_array = pair.second;
        void** new_array = new void*[row_col_capacity_];
        for (unsigned int i = 0; i < col_descs_.size(); ++i) {
            DataType data_type = Extract(col_descs_.at(i));
            if (data_type == DataType::kString) {
                 std::string& str = *(static_cast<std::string*>(original_array[i]));
                 new_array[i] = static_cast<void*>(new std::string(str));
            } else if (data_type == DataType::kDouble) {
                double num = *(static_cast<double*>(original_array[i]));
                new_array[i] = static_cast<void*>(new double(num));
            } else if (data_type == DataType::kInt) {
                int val = *(static_cast<int*>(original_array[i]));
                new_array[i] = static_cast<void*>(new int(val));
            }
        }
        rows_[key] = new_array;
    }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
    //Copy assignment operator. Implements a deep copy policy for DbTable typed objects.
    if (this == &rhs) {
        return *this;
    }
    DbTable temp(rhs);
    std::swap(next_unique_id_, temp.next_unique_id_);
    std::swap(row_col_capacity_, temp.row_col_capacity_);
    std::swap(col_descs_, temp.col_descs_);
    std::swap(rows_, temp.rows_);

    return *this;
}

DbTable::~DbTable() {
    for (unsigned int row_id = 0; row_id < next_unique_id_; ++row_id) {
        if(rows_.find(row_id) != rows_.end()) {
            DeleteRowById(row_id);
        }    
    }
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
    const auto& obj = table.col_descs_[0];
    os << obj.first << "(";
    DataType data_type = table.Extract(obj);
    if (data_type == DataType::kString) {
        os << "std::string)";
    } else if (data_type == DataType::kDouble) {
        os << "double)";
    } else {
        os << "int)";
    }
    for (size_t i = 1; i < table.col_descs_.size(); ++i) {
        const auto& item = table.col_descs_[i];
        os << ", " << item.first << "(";
        data_type = table.Extract(item);
        if (data_type == DataType::kString) {
            os << "std::string)";
        } else if (data_type == DataType::kDouble) {
            os << "double)";
        } else {
            os << "int)";
        }
    }
    return table.Rizz(os, table);
}

std::ostream& DbTable::Rizz(std::ostream& os, const DbTable& table) const {
    for (const auto& row : table.rows_) {
        os << '\n';
        void** row_data = row.second;
        DataType data_type = table.Extract(table.col_descs_.at(0));
        if (data_type == DataType::kString) {
            os << *(static_cast<std::string*>(row_data[0]));
        } else if (data_type == DataType::kDouble) {
            os << *(static_cast<double*>(row_data[0]));
        } else {
            os << *(static_cast<int*>(row_data[0]));
        }
        for (size_t i = 1; i < table.col_descs_.size(); ++i) {
            os << ", ";
            data_type = table.Extract(table.col_descs_.at(i));
            if (data_type == DataType::kString) {
                os << *(static_cast<std::string*>(row_data[i]));
            } else if (data_type == DataType::kDouble) {
                os << *(static_cast<double*>(row_data[i]));
            } else {
                os << *(static_cast<int*>(row_data[i]));
            }
        }
    }
    return os;
}

void DbTable::Resize() {
    row_col_capacity_ *= 2;
    for (unsigned int k = 0; k < next_unique_id_; ++k) {
        auto& row = rows_[k];
       void** arr = new void*[row_col_capacity_];
       for (unsigned int i = 0; i < col_descs_.size(); ++i) {
        arr[i] = row[i];
       }
       delete[] row;
       row = arr;
    }
}

DataType DbTable::Extract(const std::pair<std::string, DataType>& col_desc) const {
    return col_desc.second;
}

void DbTable::DeleteRowsFromMap(void**& arr, unsigned int col_idx) {
    for (unsigned int i = col_idx; i < col_descs_.size(); ++i) {
        if (i == col_descs_.size() - 1) {
            arr[i] = nullptr;
        } else {
            arr[i] = arr[i + 1]; 
        }
    }
}

std::vector<std::string> DbTable::ListIntoVector(const std::initializer_list<std::string>& col_data) {
    std::vector<std::string> vect;
    for (const std::string& str : col_data) {
        vect.push_back(str);
    }
    return vect;
}