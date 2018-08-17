#ifndef NURSE_H
#define NURSE_H

#include <vector>

#include "nurse.h"
#include "day_shift.h"

class Shift;

class Nurse{
public:
  int id;
  int shiftsCount;

  std::vector<int> preferences;
  std::vector<Shift*> shifts;

  Nurse(int id);
  void setPreferences(std::vector<int> pref);
  int penalizeShiftPreference();
  int penalizeConsecutiveSameShifts();
  int penalizeConsecutiveShifts(int maxConShifts);
  int penalizeFreeDays(int minAss, int maxAss);
};
#endif
