#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "nurse.h"
#include "shift.h"

class Instance {
public:
  int score;
  int iterations;

  std::vector<Nurse> nurses;

  Instance(std::vector<Shift> shifts, std::vector<Nurse> newNurses);
  Instance(std::vector<Nurse> nurses, int score, int iterations);
  void print(bool debug, bool toFile, std::ofstream* file);
};

#endif