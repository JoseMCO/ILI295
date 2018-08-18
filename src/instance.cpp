#include "instance.h"

Instance::Instance(std::vector<Shift> shifts, std::vector<Nurse> newNurses) {
  nurses = newNurses;
  for (int i = 0; i < nurses.size(); ++i) {
    nurses.at(i).shiftsCount = 0;
  }

  int shiftsCount = shifts.size();
  std::vector< std::vector<int> > coverages = std::vector< std::vector<int> >();
  for (int j = 0; j < shiftsCount; ++j) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    srand(seed);

    int c = shifts.at(j).coverage;
    // c = c + (rand() % (nurses.size()-c));
    std::vector<int> coverage = std::vector<int>();
    for (int k = 0; k < c; ++k){
      coverage.push_back(1);
    }
    for (int k = c; k < nurses.size(); ++k){
      coverage.push_back(0);
    }

    std::shuffle(coverage.begin(),coverage.end(), e);
    coverages.push_back(coverage);
  }
  
  for (int i = 0; i < coverages.size(); ++i) {
    for (int j = 0; j < coverages.at(i).size(); ++j) {
      nurses.at(j).shifts.at(i) = coverages.at(i).at(j);
      nurses.at(j).shiftsCount++;
    }
  }
}

Instance::Instance(std::vector<Nurse> nurses, int score, int iterations) {
  this->score = score;
  this->iterations = iterations;
  this->nurses = nurses;
}

void Instance::print(bool debug, bool toFile, std::ofstream* file) {
  std::cout << std::endl;
  for (int i = 0; i < this->nurses.size(); ++i) {
    for (int j = 0; j < this->nurses.at(i).shifts.size(); ++j) {
      if(debug) std::cout << this->nurses.at(i).shifts.at(j) << " ";
      if(toFile) *file << this->nurses.at(i).shifts.at(j) << " ";
      // if ((j+1)%3==0) std::cout << " ";
    }
    if(debug) std::cout << std::endl;
    if(toFile) *file << std::endl;
  }
  if(debug) std::cout << std::endl;
  if(toFile) *file << std::endl;
}
