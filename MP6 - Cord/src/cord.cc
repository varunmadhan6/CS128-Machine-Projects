#include "cord.hpp"

#include <stdexcept>

#include "cord_utilities.hpp"

Cord::Cord(std::string_view sv) {
    if (sv.empty()) {
        throw std::invalid_argument("sv has size 0");
    }
    data_ = new char[sv.size() + 1];
    for (size_t i = 0; i < sv.size(); ++i) {
        data_[i] = sv.at(i);
    }
    data_[sv.size()] = '\0';
    length_ = static_cast<unsigned int>(sv.size());
    left_ = nullptr;
    right_ = nullptr;
}

Cord::~Cord() {
    delete[] data_;
}

SharedPointer<char> Cord::ToString() const {
    if (data_ != nullptr) {
        char* to_string = new char[length_ + 1];
        for (unsigned int i = 0; i < length_; ++i) {
            to_string[i] = data_[i];
        }
        to_string[length_] = '\0';
        return {to_string, kSharedPointerType::kArray};
    }
    unsigned int len = CordLength();
    char* arr = new char[len + 1];
    unsigned int iterator = 0;
    RealToString(arr, iterator);
    arr[len] = '\0';
    return {arr, kSharedPointerType::kArray};
}

void Cord::RealToString(char* c_style_arr_, unsigned int& iterator_) const {
    if (data_ != nullptr) {
        for (unsigned int i = 0; i < length_; ++i) {
            c_style_arr_[iterator_] = data_[i];
            iterator_ += 1;
        }
    } else {
        left_->RealToString(c_style_arr_, iterator_);
        right_->RealToString(c_style_arr_, iterator_);
    }
}

unsigned int Cord::CordLength() const {
    if (data_ != nullptr) {
        return length_;
    } else {
        return left_->CordLength() + right_->CordLength();
    }
}

void Cord::SetLength() {
    length_ = CordLength();
    if (left_.Get() != nullptr) {
        left_->SetLength();
    }
    if (right_.Get() != nullptr) {
        right_->CordLength();
    }
}

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                 SharedPointer<Cord> right_cord) {
                                    if ((left_cord.Get() == nullptr) || (right_cord.Get() == nullptr)) {
                                       throw std::runtime_error("1+ cord(s) are null"); 
                                    }
                                    Cord* c = new Cord();
                                    c->left_ = left_cord;
                                    c->right_ = right_cord;
                                    c->length_ = c->CordLength();
                                    return {c, kSharedPointerType::kObject};
                                 }