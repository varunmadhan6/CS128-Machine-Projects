#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

#include "constants.hpp"
#include "group.hpp"

/*Write your main driver in this file! Remember to seperate
out things into functions to help with readability and reusability.

See specifications for how to get started and
go to office hours if you need help!*/

//function declarations
std::vector<Volunteer> CreateVolunteerList(const std::string& FileName, int GroupAmount);
std::vector<Group> PutVolunteersIntoGroups(std::vector<Volunteer> vec, int num);
bool Swapper(std::vector<Group> &GroupList, int amount);
bool Check(std::vector<Group>& vec);

int main(int argc, char* argv[]) {
  // DO NOT EDIT THIS LINE or add another srand call.
  srand(time(nullptr));
  //error handling
  if (argc != 4) {
    std::cout << "Wrong amount of arguments inputted into command line" << std::endl;
  }
  // reads command line arguments
  std::string input_file_name = argv[1];
  int num_of_volunteers = std::stoi(argv[2]);
  std::string output_file_name = argv[3];
  int num_of_groups = num_of_volunteers/kSizeGroups;
  //creates place to store volunteers
  std::vector<Volunteer> volunteers_from_input = CreateVolunteerList(input_file_name, num_of_volunteers);
  std::vector<Group> groups_list = PutVolunteersIntoGroups(volunteers_from_input, num_of_volunteers);
  bool success = Swapper(groups_list, num_of_groups);
  if (success) {
    std::ofstream ofs {output_file_name};
    for (const auto& g : groups_list) {
      ofs << g << "\n"; 
    }
    ofs << std::endl;
  } else {
    std::cout << "No groups found" << std::endl;
  }
}

std::vector<Volunteer> CreateVolunteerList(const std::string& FileName, int GroupAmount) {
  //creates variables to read from the input stream ifs
  //unpacks input file to read from
  std::ifstream ifs{FileName};
  std::vector<Volunteer> volunteers;
  std::string name_of_volunteer;
  int xp = 0;
  int physstamina = 0;
  int returningmem = 0;
  for (int i = 0; i < GroupAmount; i++) {
    ifs >> name_of_volunteer >> xp >> physstamina >> returningmem;
    bool b = true;
    if (returningmem == 0) {
      b = false;
    }
    Volunteer vol(name_of_volunteer, xp, physstamina, b);
    volunteers.push_back(vol);
  }
  return volunteers;
}

std::vector<Group> PutVolunteersIntoGroups(std::vector<Volunteer> vec, int num) {
  std::vector<Group> groups_unsorted;
  int index = 0;
  for (int i = 0; i < num/kSizeGroups; i++) {
    Group g;
    for (int j = 0; j < kSizeGroups; j++) {
      g.AddVolunteer(vec[index]);
      index++;
    }
    groups_unsorted.push_back(g);
  }
  return groups_unsorted;
}

bool Swapper(std::vector<Group> &GroupList, int amount) {
  bool success = true;
  unsigned int swaps = 0;
  while (!(Check(GroupList)) && success) {
    int group_num1 = rand() % amount;
    int group_num2 = rand() % amount;
    while (group_num1 == group_num2) {
      group_num2 = rand() % amount;
    }
    std::cout << group_num1 << std::endl;
    std::cout << group_num2 << std::endl;
    Volunteer& v1 = GroupList[group_num1].GetRandomVolunteer();
    Volunteer& v2 = GroupList[group_num2].GetRandomVolunteer();
    std::cout << v1.GetName() << std::endl;
    std::cout << v2.GetName() << std::endl;
    std::swap(v1, v2);
    std::cout << v1.GetName() << std::endl;
    std::cout << v2.GetName() << std::endl;
    ++swaps;
    if (swaps == kMaxIterations) {
      success = false;
    }
  }
  return success;
}

bool Check(std::vector<Group>& vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    if (vec[i].GetAvgBuildingExp() < kMinAvgBuildingExp || vec[i].GetReturningMembers() < kMinReturning
    || vec[i].GetAvgStamina() < kMinAvgStamina) {
      return false;
    }
  }
  return true;
}