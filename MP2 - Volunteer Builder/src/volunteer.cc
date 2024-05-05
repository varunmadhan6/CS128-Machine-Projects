#include "volunteer.hpp"

Volunteer::Volunteer(const std::string& name,
            int building_experience,
            int physical_stamina,
            bool returning) : name_(name), exp_(building_experience), stamina_(physical_stamina), return_(returning) {
                name_ = name;
                exp_ = building_experience;
                stamina_ = physical_stamina;

                // checking min and max for stamina

                if (physical_stamina < kMinStamina) {
                    stamina_ = kMinStamina;
                } else if (physical_stamina > kMaxStamina) {
                    stamina_ = kMaxStamina;
                }

                // checking min and max for building exp

                if (building_experience < kMinBuildingExp) {
                    exp_ = kMinBuildingExp;
                } else if (building_experience > kMaxBuildingExp) {
                    exp_ = kMaxBuildingExp;
                }
            }

std::string Volunteer::GetName() const {
    return name_;
}

int Volunteer::GetBuildingExperience() const {
    return exp_;
}

int Volunteer::GetPhysicalStamina() const {
    return stamina_;
}

bool Volunteer::IsReturning() const {
    return return_;
}