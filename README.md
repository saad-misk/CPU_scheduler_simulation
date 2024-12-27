# Process Scheduling Simulation

This project implements CPU scheduling algorithms using C++ and Python. The algorithms simulate the scheduling of processes in an operating system, calculate performance metrics, and visualize process execution using a Gantt chart.

## Features

### Supported Scheduling Algorithms:
- **First-Come, First-Served (FCFS)**: Non-preemptive scheduling based on process arrival time.
- **Shortest Remaining Time First (SRTF)**: Preemptive scheduling based on the remaining burst time.
- **Round Robin (RR)**: Time-sharing scheduling using a fixed quantum time.

### Metrics Calculated:
- **Average Waiting Time (AWT)**: Average time a process spends waiting in the ready queue.
- **Average Turnaround Time (TAT)**: Average time from process arrival to completion.
- **CPU Utilization**: Percentage of CPU time spent executing processes.

### Gantt Chart Visualization:
- Visual representation of process scheduling using Python's matplotlib.

## File Descriptions

### 1. C++ Files
- **main.cpp**: The entry point of the program that:
  - Reads process data and quantum time from `processes.txt`.
  - Executes scheduling algorithms.
  - Outputs performance metrics and process execution details.

- **Scheduling_algorithms.h** and **Scheduling_algorithms.cpp**: Defines and implements the three scheduling algorithms:
  - FCFS
  - SRTF
  - Round Robin

- **processes.txt**: Input file containing the list of processes and their attributes:
  ```
  start
  arrival_time,burst_time
  ...
  end
  quantum_time
  ```
### 2. Python File
- **main.py**:  
  - Parses the output of the C++ program.  
  - Generates Gantt charts for each scheduling algorithm using matplotlib.  
  - Displays metrics (AWT, TAT, CPU Utilization) along with the visualization.  

## How to Run  

1. **Compile the C++ Code**:
   ```
   g++ main.cpp Scheduling_algorithms.cpp -o scheduling
   ```
1. **Run the C++ Program**:
   ```
   ./scheduling | python main.py
   ```
  This directly pipes the output of the C++ program to the Python script, displaying Gantt charts and metrics.
