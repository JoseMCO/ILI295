#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "nurse.h"
#include "day_shift.h"

class Instance {
public:
  int id;
  int shiftsCount;
  int score;
  int iterations;

  std::vector< std::vector<int> > nurses;

  Instance(std::vector<Day> days, int nurses);
  Instance(std::vector<Nurse> nurses, int shifts, int score, int iterations);
  void print();
};

#endif