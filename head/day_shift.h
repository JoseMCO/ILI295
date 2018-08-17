#ifndef DAY_SHIFT_H
#define DAY_SHIFT_H

#include <vector>

#include "day_shift.h"
#include "nurse.h"

class Shift;
class Nurse;

class Day {
public:
  int id;
  int shiftsCount;

  std::vector<Shift*> shifts;

  Day();
  Day(int id, int sCount);
};

class Shift {
public:
  int id;
  int minAssignments;
  int maxAssignments;
  int minConsecutive;
  int maxConsecutive;
  int coverage;
  int nursesCount;
  int type;

  Day* day;
  std::vector<Nurse*> nurses;

  Shift(int id, int type, int minAss, int maxAss, int minCon, int maxCon, Day* day);
  void setCoverage(int cov);
  int penalizeCoverage();
  int validCoverage();
};
#endif