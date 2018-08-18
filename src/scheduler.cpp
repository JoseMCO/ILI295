#include "scheduler.h"

Scheduler::Scheduler(bool debug, std::ofstream *file) {
  this->debug = debug;
  this->currentScore = 0;
  this->file = file;
}

void Scheduler::log(std::string text) {
  if(!debug) return;
  std::cout << text;
}
void Scheduler::log(std::string text, bool last) {
  if(!debug) return;
  log(text);
  if (last) {
    std::cout << std::endl;
  }
}
void Scheduler::log(int number) {
  if(!debug) return;
  std::cout << number;
  std::cout << " ";
}
void Scheduler::log(int number, bool last) {
  if(!debug) return;
  log(number);
  if (last) {
    std::cout << std::endl;
  }
}

void Scheduler::printCurrentInstance(bool toFile) {
  instances.at(instances.size()-1).print(debug, toFile, file);
}

std::vector<int> Scheduler::penalizeShiftPreference() {
  std::vector<int> penalizations = std::vector<int>(nursesCount,0);
  int penalization = 10;
  for (int n = 0; n < nurses.size(); ++n) {
    std::vector<int> nShifts = nurses.at(n).shifts;
    for (int s = 0; s < shiftsCount; ++s) {
      if (nShifts.at(s) == 1) {
        penalizations.at(n) += (4-nurses.at(n).preferences.at(s))*penalization;
      }
    }
  }
  log("penalizeShiftPreference",1);
  for (int i = 0; i < penalizations.size(); ++i) {
    log(penalizations.at(i));
  }
  log("\n",1);
  return penalizations;
}

std::vector<int> Scheduler::penalizeConsecutiveSameShifts() {
  std::vector<int> penalizations = std::vector<int>(nursesCount,0);
  int penalization = 15;
  for (int n = 0; n < nurses.size(); ++n) {
    std::vector<int> reps = std::vector<int>(shiftsTypes,0);
    std::vector<int> nShifts = nurses.at(n).shifts;
    for (int st = 0; st < shiftsTypes; ++st) {
      int shiftMin = shifts.at(st).minConsecutive;
      int shiftMax = shifts.at(st).maxConsecutive;
      int reps = 0, pens = 0;
      for (int d = 0; d < daysCount; ++d) {
        if (nShifts.at(st+(d*shiftsTypes)) == 1) reps++;
        if (reps > 0 && (nShifts.at(st+(d*shiftsTypes)) == 0 || d+1 == daysCount)) {
          if(reps > shiftMax) {
            pens += (reps-shiftMax)*penalization;
          }
          else if(reps < shiftMin) {
            pens += (shiftMin-reps)*penalization;
          }
          reps = 0;
        }
      }
      penalizations.at(n) += pens;
    }
  }
  log("penalizeConsecutiveSameShifts",1);
  for (int i = 0; i < penalizations.size(); ++i) {
    log(penalizations.at(i));
  }
  log("\n",1);
  return penalizations;
}

std::vector<int> Scheduler::penalizeConsecutiveShifts() {
  std::vector<int> penalizations = std::vector<int>(nursesCount,0);
  int penalization = 20;
  for (int n = 0; n < nurses.size(); ++n) {
    int reps = 0;
    std::vector<int> nShifts = nurses.at(n).shifts;
    for (int s = 0; s < shiftsCount; ++s) {
      if (nShifts.at(s) == 1) reps++;
      if (reps > 0 && (nShifts.at(s) == 0 || s+1 == shiftsCount)) {
        if(reps > maxConShifts) {
          penalizations.at(n) += (reps-maxConShifts)*penalization;
        }
        else if(reps < minConShifts) {
          penalizations.at(n) += (minConShifts-reps)*penalization;
        }
        reps = 0;
      }
    }
  }
  log("penalizeConsecutiveShifts",1);
  for (int i = 0; i < penalizations.size(); ++i) {
    log(penalizations.at(i));
  }
  log("\n",1);
  return penalizations;
}

std::vector<int> Scheduler::penalizeShiftsAssignments() {
  std::vector<int> penalizations = std::vector<int>(shiftsCount,0);
  int penalization = 25;
  for (int i = 0; i < shiftsCount; ++i) {
    int count = shifts.at(i).nursesCount;
    if (count > shifts.at(i).maxAssignments){
      penalizations.at(i) = (count-shifts.at(i).maxAssignments)*penalization;
    }
    else if (shifts.at(i).minAssignments > count){
      penalizations.at(i) = (shifts.at(i).minAssignments-count)*penalization;
    }
  }
  log("penalizeShiftsAssignments",1);
  for (int i = 0; i < penalizations.size(); ++i) {
    log(penalizations.at(i));
  }
  log("\n",1);
  return penalizations;
}

std::vector<int> Scheduler::penalizeFreeDays() {
  std::vector<int> penalizations = std::vector<int>(nursesCount,0);
  int penalization = 1;
  for (int n = 0; n < nurses.size(); ++n) {
    int reps = 0;
    std::vector<int> nShifts = nurses.at(n).shifts;
    for (int s = 0; s < shiftsCount; ++s) {
      if (nShifts.at(s) == 1) reps++;
    }
    if (reps > maxAssignments) {
      penalizations.at(n) = (reps-maxAssignments)*penalization;
    }
    else if(minAssignments > reps) {
      penalizations.at(n) = (minAssignments-reps)*penalization;
    }
  }
  log("penalizeFreeDays",1);
  for (int i = 0; i < penalizations.size(); ++i) {
    log(penalizations.at(i));
  }
  log("\n",1);
  return penalizations;
}

int Scheduler::calculatePenalization(bool toFile) {
  std::vector< std::vector<int> > pen = std::vector< std::vector<int> >(nursesCount, std::vector<int>(2, 0));
  std::vector<int> p1 = penalizeShiftPreference();
  std::vector<int> p2 = penalizeConsecutiveSameShifts();
  std::vector<int> p3 = penalizeConsecutiveShifts();
  std::vector<int> p4 = penalizeFreeDays();
  std::vector<int> p5 = penalizeShiftsAssignments();
  int pensCount = 0, pens = 0;
  std::vector<int> pensByNurse = std::vector<int>(nursesCount, 0);
  std::vector<int> pensCountByNurse = std::vector<int>(nursesCount, 0);
  for (int n = 0; n < nursesCount; ++n) {
    if (p1.at(n) > 1) {
      pensCountByNurse.at(n)++;
      pensByNurse.at(n)+=p1.at(n);
    }
    if (p2.at(n) > 1) {
      pensCountByNurse.at(n)++;
      pensByNurse.at(n)+=p2.at(n);
    }
    if (p3.at(n) > 1) {
      pensCountByNurse.at(n)++;
      pensByNurse.at(n)+=p3.at(n);
    }
    if (p4.at(n) > 1) {
      pensCountByNurse.at(n)++;
      pensByNurse.at(n)+=p4.at(n);
    }
    pens+=pensByNurse.at(n);
    pensCount+=pensCountByNurse.at(n);
  }
  for (int i = 0; i < p5.size(); ++i) {
    pens+=p5.at(i);
  }

  if (debug || toFile) {
    log(pensCount);
    if (toFile) *file << pensCount << " ";
    for (int i = 0; i < nursesCount; ++i) {
      log(pensCountByNurse.at(i));
      if (toFile) *file << pensCountByNurse.at(i) << " ";
    }
    log("\n");
    log(pens);
    if (toFile) *file << "\n" << pens << " ";
    for (int i = 0; i < nursesCount; ++i) {
      log(pensByNurse.at(i));
      if (toFile) *file << pensByNurse.at(i) << " ";
    }
    log("", true);
    log(-pens);
    if (toFile) *file << "\n" << -pens << " ";
    for (int i = 0; i < nursesCount; ++i) {
      log(-pensByNurse.at(i));
      if (toFile) *file << -pensByNurse.at(i) << " ";
    }
    log("", true);
    if (toFile) *file << "\n";
  }
  log("", true);
  if (toFile) *file << "\n";
  return pens;
}

bool Scheduler::validateInstance() {
  for (int s = 0; s < shiftsCount; ++s) {
    if (!shifts.at(s).validate()) {
      return false;
    }
  }
  return true;
}

void Scheduler::assignNurseToShift(int nurse, int shift) {
  if (nurses.at(nurse).shifts.at(shift) == 1) return;
  nurses.at(nurse).shifts.at(shift) = 1;
  nurses.at(nurse).shiftsCount++;
  shifts.at(shift).nursesCount++;
}

void Scheduler::removeNurseFromShift(int nurse, int shift) {
  if (nurses.at(nurse).shifts.at(shift) == 0) return;
  nurses.at(nurse).shifts.at(shift) = 0;
  nurses.at(nurse).shiftsCount--;
  shifts.at(shift).nursesCount--;
}

void Scheduler::toggleShift(int nurse, int shift) {
  if (nurses.at(nurse).shifts.at(shift) == 1) {
    removeNurseFromShift(nurse, shift);
  }
  else {
    assignNurseToShift(nurse, shift);
  }
}

void Scheduler::assignNursesFromInstance(Instance instance) {
  nurses = instance.nurses;
  for (int i = 0; i < shifts.size(); ++i) {
    shifts.at(i).nursesCount = 0;
  }
  for (int n = 0; n < nurses.size(); ++n) {
    for (int s = 0; s < nurses.at(n).shifts.size(); ++s) {
      if (nurses.at(n).shifts.at(s) == 1) {
        shifts.at(s).nursesCount++;
      }
    }
  }
}

void Scheduler::assignNursesFromInstance() {
  Instance ins = instances.at(instances.size()-1);
  assignNursesFromInstance(ins);
}

void Scheduler::assignInstanceFromNurses(int iterations) {
  int score = calculatePenalization(false);
  instances.at(instances.size()-1) = Instance(nurses, score, iterations);
}

bool Scheduler::initWithFiles(std::string format, std::string instance) {
  std::ifstream formatFile(format);
  std::ifstream instanceFile(instance);
  if(!formatFile.good()) {
    log("Invalid format file", true);
    return false;
  }
  if(!instanceFile.good()) {
    log("Invalid instance file", true);
    return false;
  }

  log("parsing instanceFile", true);
  formatFile >> this->daysCount >> this->shiftsTypes;
  formatFile >> this->minAssignments >> this->maxAssignments;
  formatFile >> this->minConShifts >> this->maxConShifts;
  this->shiftsCount = daysCount*shiftsTypes;

  log("creating shifts", true);
  shifts = std::vector<Shift>(shiftsCount, Shift(0,0,0,0,0,0));
  for(int i = 0; i < shiftsTypes; i++){
    int p1, p2, p3, p4;
    formatFile >> p1 >> p2 >> p3 >> p4;
    for(int j = 0; j < daysCount; j++){
      int id = j*shiftsTypes + i;
      shifts.at(id) = Shift(id, i, p1, p2, p3, p4);
    }
  }
  formatFile.close();

  log("parsing instanceFile", true);
  int nDays, nShiftsCount;

  instanceFile >> nursesCount >> nDays >> nShiftsCount;
  if (nDays != daysCount) {
    log("Incompatible instance file (days)", true);
    return false;
  }
  if (nShiftsCount != shiftsTypes) {
    log("Incompatible instance file (shifts)", true);
    return false;
  }

  log("setting shifts coverage", true);
  for(int j = 0; j < shiftsCount; j++) {
    int v;
    instanceFile >> v;
    shifts.at(j).setCoverage(v);
    log(v);
  }
  log("",1);

  log("creating nurses", true);
  for (int k = 0; k < nursesCount; ++k) {
    Nurse nurse = Nurse(k, shiftsCount);
    nurses.push_back(nurse);
  }

  log("setting nurses preferences", true);
  for (int k = 0; k < nursesCount; ++k) {
    std::vector<int> pref = std::vector<int>();
    int v;
    for(int i = 0; i < shiftsCount; i++) {
      instanceFile >> v;
      pref.push_back(v);
      log(v);
    }
    nurses.at(k).setPreferences(pref);
    log("",1);
  }
  log("",1);
  instanceFile.close();
  return true;
}

void Scheduler::randomInstance() {
  instances.push_back(Instance(shifts, nurses));
  assignNursesFromInstance();
}

void Scheduler::localSearch(int iterations, int retries) {
  log("Start!",1);
  for (int i = 0; i < retries; ++i) {
    log("Try n:");log(i+1,1);
    randomInstance();
    int j = 0;
    // debug = false;
    currentScore = calculatePenalization(false);
    // debug = true;
    bool improvent = true;
    for (j = 0; j < iterations; ++j) {
      bool improve = false;
      int fN = (rand() % nursesCount);
      for (int n = fN; n < nursesCount+fN; ++n) {
        int k = n;
        if (k >= nursesCount) k=k-nursesCount;
        int fS = (rand() % shiftsCount);
        for (int s = fS; s < shiftsCount+fS; ++s) {
          int l = s;
          if (l >= shiftsCount) l=l-shiftsCount;
          toggleShift(k, l);
          if(validateInstance()) {
            // debug = false;
            int newScore = calculatePenalization(false);
            // debug = true;
            if (currentScore > newScore){
              currentScore = newScore;
              improve = true;
              improvent = false;
              break;
            }
          }
          toggleShift(k, l);
        }
        if(improve) break;
      }
      if(!improve) break;
    }
    std::cout << "try " << i << " score: " << currentScore << std::endl;
    if(!validateInstance()) log("Invalid!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",1);
    assignInstanceFromNurses(j);
    printCurrentInstance(false);
  }
  int bestInstance = instances.size()-1;
  int bestScore = currentScore;
  log("",1);
  for (int i = 0; i < instances.size(); ++i) {
    log("Instance: ");log(i);log("score: ");log(instances.at(i).score,1);
    if (instances.at(i).score <= bestScore) {
      bestScore = instances.at(i).score;
      bestInstance = i;
      log("Instance: ");log(i);log("best yet!",1);
    }
  }
  assignNursesFromInstance(instances.at(bestInstance));
  assignInstanceFromNurses(0);
  printCurrentInstance(true);
  calculatePenalization(true);
  if(validateInstance()) log("valida!",1);
  log("FIN",1);
}
