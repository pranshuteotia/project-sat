# project-sat
Introducing a Plug-N-Play SAT Solver.

## Requirements
cmake - 3.16 or higher.

## Building the project.
```
cmake .
make
```

## Running the project
```
project-sat heuristic mode /path/to/cnf-file
```
### Example
```
project-sat dlis undo-stack benchmarks/data.in
```
The above command will run the DPLL solver which utilizes the `dlis` heuristic, and the `undo-stack` for keeping track of modifications on the `data.in` file in the `benchmarks` directory.

## Available Options
Currently project-sat supports two heuristics and three rendtitions of the DPLL algorithm.

### Heuristics
* DLIS
* VSIDS

### Implementation Type
* undo-stack: Utilizes the stack data structure to keep track of changes.
* no-stack: All the modifications are stored in the stack frame.
* copy-constructor: Keeps track of changes by creating a new solver object with the appropriate changes.
