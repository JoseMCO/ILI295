#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "scheduler.h"

int main(int args, char **argv) {
  if(args < 4){
    std::cout << "Usage:";
    std::cout << "\t ./nsp case.gen instance.nsp [iterations [retries]] \n";
    return 1;
  }
  Scheduler scheduler = Scheduler(false);
  if(!scheduler.initWithFiles(argv[1], argv[2])){
    return 1;
  };

  int iterations = 10000;
  int tries = 1;
  if (argv[3]) {
    iterations = strtol(argv[3], NULL, 10);
    if (argv[4]) {
      tries = strtol(argv[4], NULL, 10);
    }
  }

  scheduler.localSearch(iterations, tries);
  return 0;
}
