#include <iostream>
#include <cstdlib>
#include <ctime>
#include "group.hpp"
#include "volunteer.hpp"

void Group::AddVolunteer(const Volunteer& vol) {
    volunteerlist_.push_back(vol);
}

double Group::GetAvgBuildingExp() const {
    if (volunteerlist_.empty()) {
        return 0;
    }
    double avgexp = 0.0;
    for (const auto& v : volunteerlist_) {
        avgexp += v.GetBuildingExperience();
    }
    return avgexp/static_cast<double>(volunteerlist_.size());
}

int Group::GetReturningMembers() const {
    if (volunteerlist_.empty()) {
        return 0;
    }
    int total_returning = 0;
    for (const auto& v : volunteerlist_) {
        if (v.IsReturning()) {
            total_returning ++;
        }
    }
    return total_returning;
}

double Group::GetAvgStamina() const {
    if (volunteerlist_.empty()) {
        return 0;
    }
    double avgstamina = 0.0;
    for (const auto& v : volunteerlist_) {
        avgstamina += v.GetPhysicalStamina();
    }
    return avgstamina/static_cast<double>(volunteerlist_.size());
}

std::ostream& operator<<(std::ostream& os, const Group& g_in) {
    os << "Building Exp: " << g_in.GetAvgBuildingExp() << ", Stamina: " 
    << g_in.GetAvgStamina() << ", Returning Members: " << g_in.GetReturningMembers() << "\n";
    for (const auto& v : g_in.volunteerlist_) {
        os << v.GetName() << "\n";
    }
    return os;
}

Volunteer& Group::GetRandomVolunteer() {
    return volunteerlist_[rand() % volunteerlist_.size()];
}