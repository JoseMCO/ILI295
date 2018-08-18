#include "nurse.h"

Nurse::Nurse(int id, int shifts) {
  this->id = id;
  this->shiftsCount = 0;
  this->shifts = std::vector<int>(shifts, 0);
}

void Nurse::setPreferences(std::vector<int> pref) {
  this->preferences = pref;
}