#include "instance.h"

Instance::Instance(std::vector<Day> days, int nursesCount) {
  if (days.size() == 0) {
    std::vector<int> zeros = std::vector<int>(0, 0);
    nurses = std::vector< std::vector<int> >(nurses.size(), zeros);
    return;
  }

  int shiftsCount = days.size()*days.at(0).shifts.size();
  std::vector< std::vector<int> > coverages = std::vector< std::vector<int> >();
  for (int i = 0; i < days.size(); ++i) {
    for (int j = 0; j < days.at(i).shifts.size(); ++j) {
      int c = days.at(i).shifts.at(j)->coverage;
      c = c + (rand() % (nursesCount-c));
      std::vector<int> coverage = std::vector<int>();
      for (int k = 0; k < c; ++k){
        coverage.push_back(1);
      }
      for (int k = c; k < nursesCount; ++k){
        coverage.push_back(0);
      }
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine e(seed);
      std::shuffle(coverage.begin(),coverage.end(), e);
      coverages.push_back(coverage);
    }
  }
  std::vector<int> base = std::vector<int>(shiftsCount, 0);
  nurses = std::vector< std::vector<int> >(nursesCount, base);

  for (int i = 0; i < coverages.size(); ++i) {
    for (int j = 0; j < coverages.at(i).size(); ++j) {
      nurses.at(j).at(i) = coverages.at(i).at(j);
    }
  }

}

Instance::Instance(std::vector<Nurse> nurses, int shifts, int score, int iterations) {
  this->score = score;
  this->iterations = iterations;
  std::vector<int> zeros = std::vector<int>(shifts, 0);
  this->nurses = std::vector< std::vector<int> >(nurses.size(), zeros);
  for (int i = 0; i < nurses.size(); ++i) {
    for (int j = 0; j < nurses.at(i).shifts.size(); ++j) {
      int nurseId = nurses.at(i).id;
      int shiftId = nurses.at(i).shifts.at(j)->id;
      this->nurses.at(nurseId).at(shiftId) = 1;
    }
  }
}

void Instance::print() {
  std::cout << std::endl;
  for (int i = 0; i < this->nurses.size(); ++i) {
    for (int j = 0; j < this->nurses.at(i).size(); ++j) {
      std::cout << this->nurses.at(i).at(j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
