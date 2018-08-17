#include "nurse.h"

Nurse::Nurse(int id) {
  this->id = id;
  this->shiftsCount = 0;
}

void Nurse::setPreferences(std::vector<int> pref) {
  this->preferences = pref;
}

int Nurse::penalizeShiftPreference() {
  int totalPen = 0;
  int penalization = 10;
  for (int i = 0; i < shifts.size(); ++i) {
    int preference = preferences.at(shifts.at(i)->id);
    totalPen = totalPen + (4-preference)*penalization;
  }

  return totalPen;
}

int Nurse::penalizeConsecutiveSameShifts() {
  int totalPen = 0;
  int penalization = 15;
  int lastShift = 0;
  int repetitions = 0;
  for (int i = 0; i < shifts.size(); ++i) {
    if (lastShift == shifts.at(i)->type) {
      if (repetitions >= shifts.at(i)->maxConsecutive) {
        totalPen = totalPen + penalization;
      }
      repetitions++;
    }
    else {
      lastShift = shifts.at(i)->type;
      repetitions = 1;
    }
  }
  return totalPen;
}

int Nurse::penalizeConsecutiveShifts(int maxConShifts) {
  int totalPen = 0;
  int penalization = 15;
  int lastShift = -99;
  int repetitions = 0;
  for (int i = 0; i < shifts.size(); ++i) {
    if (lastShift+1 == shifts.at(i)->id) {
      repetitions++;
      if (repetitions > maxConShifts) {
        totalPen = totalPen + penalization;
      }
    }
    else {
      lastShift = shifts.at(i)->id;
      repetitions = 1;
    }
  }
  return totalPen;
}

int Nurse::penalizeFreeDays(int minAss, int maxAss) {
  int penalization = 20;
  int totalPen = 0;
  if (shifts.size() < minAss){
    totalPen = penalization*(minAss - shifts.size());
  }
  else if (shifts.size() > maxAss){
    totalPen = penalization*(shifts.size()-maxAss);
  }
  return totalPen;
}
