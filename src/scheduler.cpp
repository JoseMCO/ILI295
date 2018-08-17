#include "scheduler.h"

Scheduler::Scheduler(bool debug) {
  this->debug = debug;
  this->currentScore = 1;
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

void Scheduler::printShiftsCoverage() {
  if(!debug) return;
  log("", true);
  for (int i = 0; i < days.size(); ++i) {
    Day d = days.at(i);
    for (int j = 0; j < d.shifts.size(); ++j) {
      log(d.shifts.at(j)->coverage);
    }
    log("", true);
  }
}

void Scheduler::printNursesPreferences() {
  if(!debug) return;
  log("", true);
  for (int i = 0; i < nurses.size(); ++i) {
    Nurse n = nurses.at(i);
    for (int j = 0; j < n.preferences.size(); ++j) {
      log(n.preferences.at(j));
    }
    log("", true);
  }
}

void Scheduler::printCurrentInstance() {
  if(!debug) return;
  instances.at(instances.size()-1).print();
}

int Scheduler::calculatePenalization() {
  std::vector< std::vector<int> > pen = std::vector< std::vector<int> >(nursesCount, std::vector<int>(2, 0));
  int totalPenalization = 0;
  int totalPens = 0;
  for (int i = 0; i < nurses.size(); ++i) {
    int p0 = nurses.at(i).penalizeShiftPreference();
    int p1 = nurses.at(i).penalizeConsecutiveSameShifts();
    int p2 = nurses.at(i).penalizeConsecutiveShifts(maxConShifts);
    int p3 = nurses.at(i).penalizeFreeDays(minAssignments, maxAssignments);
    pen.at(i).at(0) = p0+p1+p2+p3;
    pen.at(i).at(1) = (p0>0)+(p1>0)+(p2>0)+(p3>0);
    totalPenalization += pen.at(i).at(0);
    totalPens += pen.at(i).at(1);
  }
  // for (int i = 0; i < shifts.size(); ++i) {
  //   totalPenalization += shifts.at(i).penalizeCoverage();
  // }
  if (debug) {
    log(totalPens);
    for (int i = 0; i < pen.size(); ++i) {
      log(pen.at(i).at(1));
    }
    log("\n");
    log(totalPenalization);
    for (int i = 0; i < pen.size(); ++i) {
      log(pen.at(i).at(0));
    }
    log("", true);
    log(-totalPenalization);
    for (int i = 0; i < pen.size(); ++i) {
      log(-pen.at(i).at(0));
    }
    log("", true);
  }
  log("", true);
  return totalPenalization;
}

bool Scheduler::validateInstance() {
  for (int i = 0; i < shifts.size(); ++i)
  {
    if (!shifts.at(i).validCoverage()) {
      return false;
    }
  }
  return true;
}

void Scheduler::assignNurseToShift(int nurse, int shift) {
  for (int i = 0; i < shifts.at(shift).nursesCount; ++i){
    if (shifts.at(shift).nurses.at(i)->id == nurse) {
      return;
    }
  }
  for (int i = 0; i < nurses.at(nurse).shiftsCount; ++i){
    if (nurses.at(nurse).shifts.at(i)->id == shift) {
      return;
    }
  }
  shifts.at(shift).nurses.push_back(&nurses.at(nurse));
  shifts.at(shift).nursesCount++;

  nurses.at(nurse).shifts.push_back(&shifts.at(shift));
  nurses.at(nurse).shiftsCount++;
}

void Scheduler::removeNurseFromShift(int nurse, int shift) {
  for (int i = 0; i < shifts.at(shift).nursesCount; ++i){
    if (shifts.at(shift).nurses.at(i)->id == nurse) {
      shifts.at(shift).nurses.erase(shifts.at(shift).nurses.begin() + i);
      shifts.at(shift).nursesCount--;
      break;
    }
  }
  for (int i = 0; i < nurses.at(nurse).shiftsCount; ++i){
    if (nurses.at(nurse).shifts.at(i)->id == shift) {
      nurses.at(nurse).shifts.erase(nurses.at(nurse).shifts.begin() + i);
      nurses.at(nurse).shiftsCount--;
      break;
    }
  }
}

void Scheduler::toggleShift(int nurse, int shift) {
  bool found = false;
  for (int i = 0; i < shifts.at(shift).nurses.size(); ++i) {
    if (shifts.at(shift).nurses.at(i)->id == nurse) {
      found = true;
      break;
    }
  }
  if (found){
    removeNurseFromShift(nurse, shift);
  } else {
    assignNurseToShift(nurse, shift);
  }
}

void Scheduler::assignNursesFromInstance(Instance instance) {
  for (int i = 0; i < instance.nurses.size(); ++i) {
    for (int j = 0; j < instance.nurses.at(i).size(); ++j) {
      if (instance.nurses.at(i).at(j) == 1){
        assignNurseToShift(i,j);
      }
      else {
        removeNurseFromShift(i,j);
      }
    }
  }
}

void Scheduler::assignNursesFromInstance() {
  Instance ins = instances.at(instances.size()-1);
  assignNursesFromInstance(ins);
}

void Scheduler::assignInstanceFromNurses(int iterations) {
  int score = calculatePenalization();
  instances.at(instances.size()-1) = Instance(nurses, shiftsCount, score, iterations);
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

  log("creating days", true);
  days = std::vector<Day>(daysCount, Day(0, 0));
  for(int j = 0; j < daysCount; j++){
    days.at(j) = Day(j, shiftsTypes);
    days.at(j).shifts = std::vector<Shift*>(shiftsTypes, NULL);
  }

  log("creating shifts", true);
  shifts = std::vector<Shift>(shiftsCount, Shift(0,0,0,0,0,0,&days.at(0)));
  for(int i = 0; i < shiftsTypes; i++){
    int p1, p2, p3, p4;
    formatFile >> p1 >> p2 >> p3 >> p4;
    for(int j = 0; j < daysCount; j++){
      int id = j*shiftsTypes + i;
      shifts.at(id) = Shift(id, i+1, p1, p2, p3, p4, &days.at(j));
      days.at(j).shifts.at(i) = &(shifts.at(id));
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
  for(int i = 0; i < days.size(); i++) {
    for(int j = 0; j < days.at(i).shifts.size(); j++) {
      int v;
      instanceFile >> v;
      days.at(i).shifts.at(j)->setCoverage(v);
    }
  }

  log("creating nurses", true);
  for (int k = 0; k < nursesCount; ++k) {
    Nurse nurse = Nurse(k);
    nurses.push_back(nurse);
  }

  log("setting nurses preferences", true);
  for (int k = 0; k < nursesCount; ++k) {
    std::vector<int> pref = std::vector<int>();
    for(int i = 0; i < daysCount; i++) {
      for(int j = 0; j < shiftsTypes; j++) {
        int v;
        instanceFile >> v;
        pref.push_back(v);
      }
    }
    nurses.at(k).setPreferences(pref);
  }
  instanceFile.close();
  return true;
}

void Scheduler::randomInstance() {
  instances.push_back(Instance(days, nursesCount));
  assignNursesFromInstance();
}

void Scheduler::localSearch(int iterations, int retries) {
  // std::cout << nursesCount << std::endl << std::endl;
  // std::cout << "score\titer\trel score\trel iter" << std::endl;
  for (int i = 0; i < retries; ++i) {
    randomInstance();
    int j = 0;
    currentScore = calculatePenalization();
    bool improvent = true;
    for (j = 0; j < iterations; ++j) {
      bool improve = false;
      int fN = (rand() % nursesCount);
      for (int n = fN; n < nursesCount+fN; ++n) {
        int k = n;
        if (k>=nursesCount) k=k-nursesCount;
        int fS = (rand() % shiftsCount);
        for (int s = fS; s < shiftsCount+fS; ++s) {
          int l = s;
          if (l>=shiftsCount) l=l-shiftsCount;
          toggleShift(k, l);
          if(validateInstance()) {
            int newScore = calculatePenalization();
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
    std::cout << currentScore << "\t" << j << "\t";
    // std::cout << ((float)currentScore)/nursesCount << "\t" << ((float)j)/nursesCount << "\t";
    // std::cout << (((float)currentScore)/nursesCount)/(((float)j)/nursesCount) << std::endl;
    std::cout << std::endl;
    assignInstanceFromNurses(j);
  }
  int bestInstance = instances.size()-1;
  for (int i = 0; i < instances.size(); ++i) {
    if (instances.at(i).score <= currentScore) {
      currentScore = instances.at(i).score;
      bestInstance = i;
    }
  }
  assignNursesFromInstance(instances.at(bestInstance));
  printCurrentInstance();
  calculatePenalization();
}
