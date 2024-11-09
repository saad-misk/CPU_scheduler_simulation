#include <vector>
#include <algorithm> // For sorting functions
#include <queue>     // For Round Robin queue
#include <climits>   // For INT_MAX

using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
};

// Declarations of scheduling algorithm functions
vector<Process> FCFS(vector<Process> processes);
vector<Process> SRTF(vector<Process> processes);
vector<Process> RoundRobin(vector<Process> processes, int quantum);