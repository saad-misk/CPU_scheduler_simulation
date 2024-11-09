#include <iostream>
#include <vector>
#include <string>
#include <fstream>
// #include <sstream>
#include "Scheduling_algorithms.h"

using namespace std;

pair<vector<Process>, int> readProcessesFromFile(const string& filename) {
    vector<Process> processes;
    ifstream file(filename);
    string line;

    getline(file, line);
    int cnt = 1;
    while (getline(file, line)) {

        if( line == "end" ) break;

        Process p;
        string temp;
        
        int comma_index = line.find(',');
        string arrival = line.substr(0, comma_index);
        string burst = line.substr(comma_index+1, line.size()-(comma_index+1));
        // cout << stoi(arrival) << " " << burst << "\n";
        p.arrival = stoi(arrival);
        p.burst = stoi(burst);
        // Initialize other fields
        p.id = cnt++;
        p.remaining = p.burst;
        p.completion = 0;
        p.turnaround = 0;
        p.waiting = 0;
        
        processes.push_back(p);
    }
    getline(file, line);
    int quantum = stoi(line);

    file.close();
    return {processes, quantum};
}

double avg_waiting_time(vector<Process>& v){
    int sum = 0;
    for(Process p : v){
        sum += p.waiting;
    }
    return sum / (double)v.size();
}
double avg_turnaround_time(vector<Process>& v){
    int sum = 0;
    for(Process p : v){
        sum += p.turnaround;
    }
    return sum / (double)v.size();
}
double cpu_utilization(vector<Process>& v){
    int sum_idle = 0;
    int n = v.size();
    for(Process p : v){
        if( p.id == -1 ) sum_idle += (p.completion - p.arrival);
    }
    double total_time = v[n-1].completion;
    double sum_util = total_time - sum_idle;
    return (sum_util / total_time) * 100; // return it as a percentage
}

void print_output(vector<Process> processes){

    vector<Process> v = FCFS(processes);
    cout << avg_waiting_time(v) << ", " << avg_turnaround_time(v) << ", " << cpu_utilization(v) << "\n";
    for(int i = 0; i < (int)v.size(); i++){
        cout << v[i].id << ": " << v[i].arrival << " " << v[i].completion << "   |   ";
    }
    cout << "\nbreak\n";

    v = SRTF(processes);
    cout << avg_waiting_time(v) << ", " << avg_turnaround_time(v) << ", " << cpu_utilization(v) << "\n";
    for(int i = 0; i < (int)v.size(); i++){
        cout << v[i].id << ": " << v[i].arrival << " " << v[i].completion << "   |   ";
    }
    cout << "\nbreak\n";

    v = RoundRobin(processes, 4);
    v[0].id = -1;
    cout << avg_waiting_time(v) << ", " << avg_turnaround_time(v) << ", " << cpu_utilization(v) << "\n";
    for(int i = 0; i < (int)v.size(); i++){
        cout << v[i].id << ": " << v[i].arrival << " " << v[i].completion << "   |   ";
    }

}

void print_detailed_processes(vector<Process> processes){

    for(auto i : processes){
        cout << "ProcessID: " << i.id << ", arrival: " << i.arrival << ", burst: " << i.burst << 
        ", remaining: " << i.remaining << ", finished: " << i.completion << ", waiting: " << 
        i.waiting << ", turnaround: " << i.turnaround << "\n";
    }

}

int main() {

    pair<vector<Process>, int> p = readProcessesFromFile("./processes.txt");

    vector<Process> processes = p.first;
    int quantum = p.second;
    //print_detailed_processes(processes);
    print_output(processes);

    return 0;
}
