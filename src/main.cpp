#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "scheduler.h"

int main(int args, char **argv) {
  if(args < 4){
    std::cout << "Usage:";
    std::cout << "\t ./nsp case.gen instance.nsp output [iterations [retries]] \n";
    return 1;
  }

  std::ofstream file;
  file.open(argv[3]);
  file << "";

  Scheduler scheduler = Scheduler(false, &file);
  if(!scheduler.initWithFiles(argv[1], argv[2])){
    file.close();
    return 1;
  };

  int iterations = 10000;
  int tries = 100;
  if (argv[4]) {
    iterations = strtol(argv[4], NULL, 10);
    if (argv[5]) {
      tries = strtol(argv[5], NULL, 10);
    }
  }

  scheduler.localSearch(iterations, tries);
  file.close();
  return 0;
}
