#include "illini_book.hpp"
#include <iostream>

int main() {
  IlliniBook fanum("tests/persons.csv", "tests/relations.csv");
  std::cout << fanum.CountGroups(std::vector<std::string>{ "128", "173" }) << " " << fanum.CountGroups(std::vector<std::string>{ "128", "124", "173" });
}
