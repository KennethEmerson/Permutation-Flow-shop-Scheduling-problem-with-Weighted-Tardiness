# Permutation Flow-Shop Scheduling Problem With Weighted Tardiness 

This repo contains a Local Search algorithm written with C++ as part of a project for the "Heuristic Optimisation " course at the Université Libre De Bruxelles.
The setup of the algorithm was inspired by the book of Hoos, H.H. and Stützle, T. [[1]](#1). The dataset used was based on [[2]](#2).

## Introduction

The permutation flow-shop scheduling problem with weighted tardiness (PFSP-WT) is one of the archetypical optimization problems used to determine the effectiveness of specific solving algorithms. Solving this flowshop scheduling problem can, in general, be defined as finding an optimal or near-optimal sequence of job allocations to one or more machines in order to minimize the weighted tardiness. Each of these jobs consists of one or more operations that each need to be performed on a specific machine, keeping in mind that a machine can only perform one operation at a time.

Furthermore, the permutation flow-shop scheduling problem comes with some additional constraints. First of all, the sequence in which the jobs pass the machines can not change between machines. The buffers between machines, which are considered infinite in size, thus work according to a First In, First Out principle. 

The weighted tardiness is used as a cost function that needs to be minimized. Any job that gets finalized passed its due date will create a penalty based on the size of the delay and the priority given to the specific job. The optimal solution for a given PFSP-WT instance will therefore result in a minimal overall weighted tardiness over the given set of jobs.

## Working Principle

The algorithms presented in this repository, vary on three axes, namely the way in which an initial starting solution is formulated (random uniform permutation and the Simplified RZ heuristic or SRZH), the way subsequent neighbourhoods are defined (TRANSPOSE, EXCHANGE and INSERT), and finally, the selection mechanism used to select a better solution in this neighbourhood (FIRST vs BEST). The implementation also includes a *Variable neighbourhood Descent (VND)* mechanism (TRANSPOSE-EXCHANGE-INSERT or TEI and TRANSPOSE-INSERT-EXCHANGE or TIE) which will use a sequence of neighbourhood construction mechanisms. if the first neighbourhood construction method in the sequence does not result in a superior solution, the algorithm will not terminate as with the algorithms described above, but a second neighbourhood will be constructed based on the next method in the given sequence. Only when all construction methods in the sequence are exhausted and no better solution can be found, will the algorithm terminate.

In order to prevent the iterative improvement algorithm from getting stuck in a given local minimum, two extensions, namely *Simulated Annealing (SA)* and *Iterated Local Search (ILS)*, are also included in the existing codebase.


## Compiling the program

In order to compile and run the required algorithms, C++17 and g++ (GCC) 12.2.0 need to be preinstalled. A makefile is provided in the root of the codebase which allows you to compile the C++ source code.

The source code compilation can be executed by simply executing the following command in a terminal while being in the root folder of this repository:

```shell
make
```
By executing this command, various messages will occur in the terminal, which should look like this:
```shell
compiling main.cpp
compiling pfspinstance.cpp
compiling configuration.cpp
compiling configparser.cpp
compiling initsolution.cpp
compiling neighbourhood.cpp
compiling helpers.cpp
compiling solver.cpp
compiling fileio.cpp
compiling runners.cpp
Linking all object files for flowshop
```
By doing this, all required binary files will be stored in the "bin" directory and a new "flowshop" file will be created in the root of this repository.

Once this is done, you can run the program by using the instructions in the next paragraph.

---
## Running an optimization on one file

In order to run the program (after compilation), you can simply execute the command "./flowshop" followed by the filepath of the datafile containing the PFSP-WT instance. for instance:

```shell
./flowshop ./data/DD_Ta051.txt
```
will run one testrun on the data contained in the file with filepath: "./data/DD_Ta051.txt". The testrun will be executed with the default parameters.

If you want to change the parameters, you can use the following flags and command line parameters:
 

description                                 | flag  | possible values behind the flag   | default                 | 
--------------------------------------------|------ |-----------------------------------|-------------------------|
the initial solution to use                 |-i     | RANDOM, SRZH                      | RANDOM                  | 
the way the neighbourhood is chosen         |-n     | TRANSPOSE,EXCHANGE,INSERT,TEI,TIE | TRANSPOSE               |
the pivoting rule to use                    |-p     | FIRST, BEST, ANNEAL               | FIRST                   |
the memoization memory to use               |-m     | NONE,HASH,MAP                     | NONE                    |
the annealing decay to use                  |-d     | a float between 0 and 1           | 0.992                   |
the number of pertubations to use with ILS  |-r     | any integer value                 | 0                       |
the seed to use                             |-s     | any integer value                 | a randomly chosen value |

for example:
```shell
./flowshop ./data/DD_Ta051.txt -p FIRST -n EXCHANGE -i SRZH
```
will launch one testrun on the data in the file "./data/DD_Ta051.txt". The solver will use the FIRST pivoting rule, the EXCHANGE neighbourhood method and SRZH as the initial solution. 
As you can see, the order of the flags with their respective values does not matter as long as the first parameter behind "./flowshop" is a path to a datafile and every flag is followed by a valid value (separated by just one space).

The program will display various messages in the terminal, regarding the configuration used, the loading of the data and finally the results of the test. These results will also be stored in a separate log file in the root folder of this repository.

---
## Running one testrun with Simulated Annealing

To run one test with simulated annealing, the -p flag should be set to ANNEAL. Only then will a user selected annealing decay be taken into consideration. 
If you only set a custom annealing decay without setting the -p flag to ANNEAL, the decay will be ignored and no simulated annealing will be triggered.
Please also be aware that a single test run can take some time to compute.

---
## Running one testrun with Iterated Local Search

To activate the Iterated Local Search, the -r flag should be set to an integer value bigger than two. By setting the command line parameter to a value bigger than 2, the Iterated Local Search will automatically be used by the Algorithm.
Please also be aware that a single test run can take some time to compute.

---
## Running a complete testrun on multiple files

In order to run a complete testrun on multiple files, you once again start by typing the "./flowshop" command in the terminal, however this time it needs to be followed by a path to a directory containing one or more datafiles.

for instance:

```shell
./flowshop ./data
```
this will start the testing script and will process all files contained in the [data](./data) directory.

Some additional command line parameters can be used to alter the testing script, These do however need to be in the correct order behind the folder path.

| pos behind folder path | command line parameter | description  |
-------------------------|------------------------|----------------------------------------------------------------------------------------------------------|
| 1                      | NONVND, TEI, TIE, ANNEAL, PERTUB       | the test to run, "NONVND is the standard test doing 5 testruns on all combinations and files. ANNEAL and PERTUB will start the annealing and ILS testrun respectively with the hyper parameter values as set in the constants.cpp file |
| 2                      | NONE, HASH, MAP         | the memoization memory to use                        |

For instance:
```shell
./flowshop ./data TEI
```
will perform the testrun with TEI as the VND.

```shell
./flowshop ./data TIE HASH
```
on the other hand will do the same test but this time using TIE and HASH as the memoization technique.

```shell
./flowshop ./data ANNEAL
```
will perform the testrun with the simulated annealing extension.

```shell
./flowshop ./data PERTUB
```
will perform the testrun with the ILS extension.

---
## Note on memoization

The solver can make use of two different memoization techniques which could potentially speed up the process.
(although tests showed it does not). By using the "HASH command line parameter, a HASH table is used to store Weighted Tardiness-values for all solutions calculated by the solver. The "MAP" command line parameter will allow the solver to use a red-black binary tree to store the WT-values.

---
## performing a dummy test

In order to test whether certain parts of the code work as planned, a flowshoptest program can be compiled, this uses dummy data from the folder "dummy_data" which mimics the actual data. 

to compile and run this testscript, use the following commands from the root of this repository:

```shell
make flowshoptest
```
```shell
./flowshoptest
```

---

## running in a Docker container

If so desired, the code can also be run in a designated Docker container which will automaticcally compile the code. 
You can then use the Docker desktop terminal to run the above described commands from within the container

The docker image can be created by means of the following command:
```shell
docker build -f Dockerfile -t heuropt:2.0 --target production .
```
Furthermore, an additional [devcontainer.json](./.devcontainer/devcontainer.json) file is included which allows you to test the code inside a development container in conjunction with Microsoft Visual Studio Code.  

## References

<a id="1">[1]</a> Hoos, H.H. and Stützle, T. (2004), *Stochastic Local Search: Foundations and Applications*  Elsevier Science (ISBN:9780080498249)   
<a id="2">[2]</a> E. Taillard (1993)
*Benchmarks for basic scheduling problems*,
European Journal of Operational Research,
Volume 64, Issue 2,
https://doi.org/10.1016/0377-2217(93)90182-M. (dataset: http://mistic.heig-vd.ch/taillard/)