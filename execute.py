import os
from random import randint

nurses = 25
retries = 20
os.system("'' > N{0}-{1}-randomst.txt".format(nurses,retries)) 
for case in range(1,17):
  for x in range(20):
    x = randint(1, 7000)
    print("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> N{0}-{3}-randomst.txt".format(nurses,case,x,retries))
    os.system("./nsp ./NSPLib/Cases/{1}.gen ./NSPLib/N{0}/{2}.nsp 10000 {3} >> N{0}-{3}-randomst.txt".format(nurses,case,x,retries)) 