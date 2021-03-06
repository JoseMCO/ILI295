#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include "scheduler.h"
#include "shift.h"
#include "nurse.h"
#include "instance.h"

class Scheduler{
public:
  bool debug;
  int daysCount;
  int shiftsCount;
  int shiftsTypes;
  int nursesCount;
  int minAssignments;
  int maxAssignments;
  int minConShifts;
  int maxConShifts;
  int currentScore;

  std::vector<Shift> shifts;
  std::vector<Nurse> nurses;
  std::vector<Instance> instances;

  std::ofstream* file;

  Scheduler(bool debug, std::ofstream* file);
  void log(std::string text);
  void log(std::string text, bool end);
  void log(int number);
  void log(int number, bool end);
  void printShiftsCoverage();
  void printNursesPreferences();
  void printCurrentInstance(bool toFile);
  std::vector<int> penalizeShiftPreference();
  std::vector<int> penalizeConsecutiveSameShifts();
  std::vector<int> penalizeConsecutiveShifts();
  std::vector<int> penalizeShiftsAssignments();
  std::vector<int> penalizeFreeDays();
  int  calculatePenalization(bool toFile);
  bool validateInstance();
  bool initWithFiles(std::string format, std::string instance);
  void randomInstance();
  void assignNurseToShift(int nurse, int shift);
  void removeNurseFromShift(int nurse, int shift);
  void assignNursesFromInstance(Instance instance);
  void assignNursesFromInstance();
  void assignInstanceFromNurses(int iterations);
  void toggleShift(int nurse, int shift);
  void localSearch(int iterations, int retries);

};
#endif
