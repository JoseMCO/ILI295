import os
from random import randint

problems = 10
nurses = 25
retries = 100
name = "score_per_tries"

gen = {
  25: [1,8],
  50: [1,8],
  75: [1,8],
  30: [9,16],
  60: [9,16],
}

os.system("'' > results/{2}-N{0}-{1}.txt".format(nurses,retries,name)) 
for case in range(1,17):
  for x in range(20):
    x = randint(1, 7000)
    print("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> results/N{0}-{3}-{4}.txt".format(nurses,case,x,retries,name))
    os.system("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> results/N{0}-{3}-{4}.txt".format(nurses,case,x,retries,name)) 

# for x in range(problems):
#   case = randint(gen[nurses][0],gen[nurses][1])
#   nsp = randint(1, 7000)
#   print("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> results/{4}-N{0}-{1}-{2}-score.txt".format(nurses,case,nsp,retries,name))
#   os.system("echo \"Score\\tRetries\" > results/{4}-N{0}-{1}-{2}-score.txt".format(nurses,case,nsp,retries,name)) 
#   os.system("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> results/{4}-N{0}-{1}-{2}-score.txt".format(nurses,case,nsp,retries,name)) 
