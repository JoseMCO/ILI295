#ifndef SHIFT_H
#define SHIFT_H

#include <vector>

class Shift {
public:
  int id;
  int minConsecutive;
  int maxConsecutive;
  int minAssignments;
  int maxAssignments;
  int coverage;
  int nursesCount;
  int type;

  Shift(int id, int type, int minCon, int maxCon, int minAss, int maxAss);
  void setCoverage(int cov);
  bool validate();
};
#endif