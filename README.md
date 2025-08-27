# City Reconstruction Simulation

This project implements a C++ simulation for modeling the reconstruction of a city following a disaster. It was developed as part of Assignment 1 in the SPL course at Ben-Gurion University.

## Overview

The simulation reads a configuration file that defines:
- Settlements and their populations
- Facilities and their types
- Reconstruction plans and actions
- Selection policies used to guide the reconstruction process

The simulator runs step-by-step, executing predefined actions and applying selection policies to rebuild the city efficiently.

## Compilation

To build the project, run the following command from the root of the repository:

```bash
make
```

## Execution

After compilation, run the simulation using:

```bash
./bin/simulation <path_to_configuration_file>
```

## Requirements

- C++11 or later
- `make` utility
- Unix-based environment (tested inside a Docker container)

## Author

**Stav Balaish**  
B.Sc. Computer Science, Ben-Gurion University  
📧 stavbalaish2000@gmail.com