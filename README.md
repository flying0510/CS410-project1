# CS410 Project 1:  Solve MSA Problem In Three Algorithms

​	MSA represents multiple sequence alignment and MSA problem is widely used in many areas such as biology and chemistry.

​	In this project, I implement three algorithms including DP(dynamic programming), A* search and GA(genetic algorithm).

## Environment

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	ubuntu18.04 &nbsp;&nbsp;&nbsp;&nbsp;gcc 7.5.0

## Compile and Run

​			 g++ -O2 dp.cpp -o dp &nbsp;&nbsp;   g++ -O2 Astar.cpp -o Astar&nbsp;&nbsp;  g++ -O2 GA.cpp -o GA

## Structure
```
.
├── README.md
├── code
│   ├── Astar.cpp						# A* search Algorithm
│   ├── GA.cpp							# Genetic Algorithm
│   └── dp.cpp							# Dynamic programming
├── data
│   ├── MSA2_query.txt					# Query set for pairwise alignment
│   ├── MSA3_query.txt					# Query set for three-sequence alignment
│   ├── MSA_database.txt				# Database including 100 sequences
│   ├── MSA_database_mini.txt			# A mini Database for test
│   └── MSA_query.txt					# Complete Query set
├── logfiles							# record each test's result for
│   ├── log_GA_2.txt					  pairwise and three-sequence alignment
│   ├── log_GA_3.txt					  using such log files
│   ├── log_astar_2.txt
│   ├── log_astar_3.txt
│   ├── log_dp_2.txt
│   └── log_dp_3.txt
├── output								# record final output results
│   ├── output_GA.txt					# output of GA.cpp
│   ├── output_astar.txt				# output of Astar.cpp
│   └── output_dp.txt					# output of dp.cpp
└── report.pdf							# Report for this project
```
