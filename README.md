# N-Puzzle
Solving games with path finding

### Building
`make`  
:warning: This project only compiles with clang++ >= 3.6 (#cutting_edge_technos)  
:warning: Compiling in non-debug mode can take a long time

### Usage
```
Available options:
  -h [ --help ]         Show this help message
  -i [ --initial ] arg  Input file for the initial state
  -g [ --goal ] arg     Input file for the goal state
  --heuristics arg      Heuristics used for solving the puzzle
  --strategy arg        Search strategy (uniform or greedy)
  -v [ --variant ] arg  A* variant (astar or idastar)
  --random-goal         Generates a goal state other than a snail
  -s [ --size ] arg     Size of randomly generated states
  --show-moves          Display the states resulting from the path finding
  --random-seed arg     Seed for generating random states
```

##### Example
`./n-puzzle -s 4 --heuristics linear manhattan`  
should output something along the lines of:
```
==============

Initial state:

|15 |7  |8  |4  |
|14 |11 |13 |10 |
|9  |3  |1  |6  |
|2  |0  |12 |5  |


Final state:

|1  |2  |3  |4  |
|12 |13 |14 |5  |
|11 |0  |15 |6  |
|10 |9  |8  |7  |

==============

A* variant:      astar
Search strategy: uniform
Heuristics:      Linear conflict + Manhattan distance
Admissible:      Yes
Random seed:     10 911 951 466 754 511 061

==============

Number of states selected:          1 277 156
Maximum number of states in memory: 2 407 941
Number of moves required:           57

==============

Computation time: 6 945 332 950 ns

==============
```
