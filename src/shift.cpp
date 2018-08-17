#include "day_shift.h"

Shift::Shift(int id, int type, int minAss, int maxAss, int minCon, int maxCon, Day* day) {
  this->id = id;
  this->type = type;
  this->minAssignments = minAss;
  this->maxAssignments = maxAss;
  this->minConsecutive = minCon;
  this->maxConsecutive = maxCon;
  this->day = day;
  this->coverage = 0;
  this->nursesCount = 0;
}

void Shift::setCoverage(int cov) {
  this->coverage = cov;
}

int Shift::penalizeCoverage() {
  int penalization = 50;
  int totalPen = 0;
  if (nurses.size() < coverage){
    totalPen = penalization*(coverage - nurses.size());
  }
  return totalPen;
}

int Shift::validCoverage() {
  if (nursesCount < coverage){
    return false;
  }
  return true;
}
