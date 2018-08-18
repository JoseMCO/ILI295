#ifndef NURSE_H
#define NURSE_H

#include <vector>

#include "nurse.h"

class Shift;

class Nurse{
public:
  int id;
  int shiftsCount;

  std::vector<int> preferences;
  std::vector<int> shifts;

  Nurse(int id, int shifts);
  void setPreferences(std::vector<int> pref);
};
#endif
