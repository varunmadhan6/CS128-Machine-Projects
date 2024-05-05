#include "elevation_dataset.hpp"

ElevationDataset::ElevationDataset(const std::string& filename, size_t width, size_t height) {
    width_ = width;
    height_ = height;
    data_ = std::vector<std::vector<int>>(height_, std::vector<int>(width_));
    int num = 0;
    std::ifstream ifs{filename};
    //filling the vector with data
    for (size_t i = 0; i < data_.size(); i++)  
    { 
        for (size_t j = 0; j < data_[i].size(); j++) {
            ifs >> num;
            if (ifs.fail()) {
                throw std::runtime_error("Either too little data or format read error");
            } else {
                data_.at(i).at(j) = num;
            }
        }
    }
    //checking if input file is largre
    ifs >> num;
    if (ifs.good()) {
        throw std::runtime_error("Too much data");
    }
    //getting min and max
    min_ele_ = data_.at(0).at(0);
    max_ele_ = data_.at(0).at(0);
    for (size_t i = 0; i < height_; i++)  
    { 
        for (size_t j = 0; j < width_; j++) {
            if (data_.at(i).at(j) < min_ele_) {
                min_ele_ = data_.at(i).at(j);
            } else if (data_.at(i).at(j) > max_ele_) {
                max_ele_ = data_.at(i).at(j);
            }
        }
    }
    std::cout << "max" << max_ele_ << "\n" << std::endl;
    std::cout << "min" << min_ele_ << "\n" << std::endl;
}

size_t ElevationDataset::Width() const {
    return width_;
};

size_t ElevationDataset::Height() const {
    return height_;
}

int ElevationDataset::MaxEle() const {
    return max_ele_;
};

int ElevationDataset::MinEle() const {
    return min_ele_;
}

int ElevationDataset::DatumAt(size_t row, size_t col) const {
    return data_.at(row).at(col);
}