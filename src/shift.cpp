#include "shift.h"

Shift::Shift(int id, int type, int minCon, int maxCon, int minAss, int maxAss) {
  this->id = id;
  this->type = type;
  this->minConsecutive = minCon;
  this->maxConsecutive = maxCon;
  this->minAssignments = minAss;
  this->maxAssignments = maxAss;
  this->coverage = 0;
  this->nursesCount = 0;
}

void Shift::setCoverage(int cov) {
  this->coverage = cov;
}

bool Shift::validate() {
  return coverage <= nursesCount;
}
