# Second Chance Page Replacement Algorithm

## Overview
This C++ program (`lab10_1.cpp`) implements the Second Chance page replacement algorithm and analyzes its performance on a randomly generated page trace. The program takes two command-line arguments `n` and `p`, representing the length of the page trace and the range of page numbers, respectively.

## Requirements
- C++ compiler
- Gnuplot (for plotting)

## Usage
Compile the program using the following command:
```bash
g++ lab10_1.cpp -o lab10_1
```

```bash
./lab10_1 <n> <p>
```
Note: Ensure that n is greater than or equal to 16, and p is greater than or equal to 8.

## Output
The program generates a file named page_faults_data.dat, which contains the number of page faults for different frame sizes. Additionally, it creates a Gnuplot script named plot_script.gp to visualize the data.

## Visualization
To visualize the results, execute the following command after running the program:

```bash
gnuplot plot_script.gp
```
This will open a Gnuplot window displaying a plot of page faults vs. the number of frames.

## File Descriptions
- lab10_1.cpp: C++ source code file.
- page_faults_data.dat: Output file containing data points for page faults.
- plot_script.gp: Gnuplot script for generating the plot.
