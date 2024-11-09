#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

struct Process {
    int id, arrival, burst, remaining, completion, waiting, turnaround;
};

// Comparator to sort processes by arrival time
bool compareArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

// First-Come, First-Served (FCFS) Scheduling Algorithm
vector<Process> FCFS(vector<Process> processes) {
    vector<Process> result;
    int current_time = 0;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareArrival);

    for (auto& process : processes) {
        if (current_time < process.arrival) {  // If CPU is idle
            result.push_back({-1, current_time, -1, -1, process.arrival, -1, -1});
            current_time = process.arrival;
        }
        process.completion = current_time + process.burst;
        current_time = process.completion;

        process.turnaround = process.completion - process.arrival;
        process.waiting = process.turnaround - process.burst;
        Process p = process;
        result.push_back({p.id, p.completion-p.burst, p.burst, p.remaining, p.completion, p.waiting, p.turnaround});
    }

    return result;
}

// Shortest Remaining Time First (SRTF) Scheduling Algorithm
vector<Process> SRTF(vector<Process> processes) {
    vector<Process> result, merged;
    int current_time = 0, completed = 0;
    int n = processes.size();
    vector<bool> isCompleted(n, false);

    while (completed < n) {
        int idx = -1, minRemainingTime = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrival <= current_time && processes[i].remaining < minRemainingTime) {
                minRemainingTime = processes[i].remaining;
                idx = i;
            }
        }

        if (idx != -1) {
            processes[idx].remaining--;
            current_time++;
            result.push_back({processes[idx].id, current_time - 1, -1, -1, current_time, -1, -1});

            if (processes[idx].remaining == 0) {
                isCompleted[idx] = true;
                completed++;

                processes[idx].completion = current_time;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                
                result.back() = {processes[idx].id, current_time - 1, processes[idx].burst, 0, current_time, processes[idx].waiting, processes[idx].turnaround};
            }
        } else {
            current_time++;
        }
    }

    // Merge consecutive segments for the same process ID
    for (const auto& p : result) {
        if (!merged.empty() && merged.back().id == p.id) {
            merged.back().completion = p.completion;
            merged.back().waiting = p.waiting;
            merged.back().turnaround = p.turnaround;
        } else {
            merged.push_back(p);
        }
    }

    return merged;
}

// Round Robin Scheduling Algorithm
vector<Process> RoundRobin(vector<Process> processes, int quantum) {
    vector<Process> result;
    int current_time = 0, completed = 0;
    int n = processes.size();
    queue<int> readyQueue;
    vector<bool> inQueue(n, false);

    // Sort processes by arrival time and initialize remaining times
    sort(processes.begin(), processes.end(), compareArrival);
    for (auto& process : processes) process.remaining = process.burst;

    for (int i = 0; i < n && processes[i].arrival <= current_time; i++) {
        readyQueue.push(i);
        inQueue[i] = true;
    }

    if( readyQueue.empty() ){
        result.push_back({-1, current_time, -1, -1, processes[0].arrival, -1, -1});
        current_time = processes[0].arrival;
        for (int i = 0; i < n && processes[i].arrival <= current_time; i++) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {

        int idx = readyQueue.front();
        readyQueue.pop();
        inQueue[idx] = false;

        int execTime = min(quantum, processes[idx].remaining);
        processes[idx].remaining -= execTime;
        current_time += execTime;
        result.push_back({processes[idx].id, current_time - execTime, -1, -1, current_time, -1, -1});

        if (processes[idx].remaining == 0) {
            completed++;
            processes[idx].completion = current_time;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            
            result.back() = {processes[idx].id, current_time - execTime, -1, -1, current_time, processes[idx].waiting, processes[idx].turnaround};
        } else {
            readyQueue.push(idx);
            inQueue[idx] = true;
        }

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= current_time && processes[i].remaining > 0 && !inQueue[i]) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (readyQueue.empty() && completed < n) {
            for (int i = 0; i < n; i++) {
                if (processes[i].remaining > 0) {
                    result.push_back({-1, current_time, -1, -1, processes[i].arrival, -1, -1});
                    current_time = processes[i].arrival;
                    readyQueue.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
        }
    }

    return result;
}