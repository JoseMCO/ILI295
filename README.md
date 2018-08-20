# Nurse Scheduling Problem
Project for Artificial Intelligence (ILI-295)

José Castro Opazo
201273514-9

## To compile
make

## To run
./nsp case.gen instance.nsp output [max-iterations [retries]]
### Example
./nsp ./NSPLib/Cases/2.gen ./NSPLib/N25/1428.nsp results/out.txt 10000 100

## Output
The output  written to the file will have the following format

S_1,1 S_2,1 S_3,1 ... S_m,1 
S_1,2 S_2,2 S_3,2 ... S_m,2 
  .     .     .         .
  .     .     .         .
  .     .     .         .
S_1,n S_2,n S_3,n ... S_m,n

nRB rn_1 rn_2 rn_3 ... rn_n
P   pn_1 pn_2 pn_3 ... pn_n  
FO  sn_1 sn_2 sn_3 ... sn_n  

Where S_i,j is 1 if the nurse j is assigned to de shift i, 0 otherwise

### Example
1 0 1 1 0 0 0 0 1 
1 1 0 1 1 0 1 1 0 
0 0 1 0 0 1 0 0 0 
0 0 0 1 0 1 1 0 0 
1 1 0 1 1 0 1 1 1 

10 2 2 2 2 2 
402 85 83 35 75 24 
-402 -85 -83 -35 -75 -24 