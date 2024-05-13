#include "cord_utilities.hpp"
#include <stdexcept>

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                               unsigned int lower_idx,
                               unsigned int upper_idx) {
                                 if (lower_idx < 0 || upper_idx > curr_cord->Length() || lower_idx >= upper_idx) {
                                  throw std::invalid_argument("invalid indices");
                                 } // error handling
                                 SharedPointer<Cord> new_cord = RealSubString(curr_cord, lower_idx, upper_idx); 
                                 return new_cord;
                               }

SharedPointer<Cord> RealSubString(SharedPointer<Cord> curr_cord,
                               unsigned int lower_idx,
                               unsigned int upper_idx) {
                                unsigned int len = curr_cord->Length();
                                 if ((curr_cord->Right().Get() == nullptr) && (curr_cord->Left().Get() == nullptr)) {
                                  if ((upper_idx == len) && (lower_idx == 0)) {
                                    return curr_cord;
                                  }
                                  char* c = StringView(curr_cord, lower_idx, upper_idx);
                                  Cord* node_one = new Cord(c);
                                  delete[] c;
                                  return {node_one, kSharedPointerType::kObject};
                                 } else if (upper_idx == len && lower_idx == 0) {
                                  return curr_cord;
                                 }
                                 unsigned int len_of_left_cord = curr_cord->Left()->Length();
                                 if (upper_idx <= len_of_left_cord) {
                                  return RealSubString(curr_cord->Left(), lower_idx, upper_idx);
                                 } else if ((lower_idx < len_of_left_cord) && (upper_idx > len_of_left_cord)) {
                                  return ConcatCords(RealSubString(curr_cord->Left(), lower_idx, len_of_left_cord), 
                                  RealSubString(curr_cord->Right(), 0, upper_idx - len_of_left_cord));
                                 } else if (lower_idx >= len_of_left_cord) {
                                  return RealSubString(curr_cord->Right(),lower_idx - len_of_left_cord, upper_idx - len_of_left_cord);
                                 } 
                                return curr_cord; 
                               }

char* StringView(SharedPointer<Cord> curr_cord, unsigned int lower_idx, unsigned int upper_idx) {
  char* c = new char[upper_idx - lower_idx + 1];
  for (unsigned int iterator = lower_idx; iterator < upper_idx; ++iterator) {
    c[iterator - lower_idx] = curr_cord->Data()[iterator];
  }
  c[upper_idx - lower_idx] = '\0';
  return c;
}

