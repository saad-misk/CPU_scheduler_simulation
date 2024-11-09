import matplotlib.pyplot as plt
import matplotlib.patches as patches
import sys

process_color = "#81c3d7"

def plot_gantt_chart(ax, processes, start_times, durations, title, metrics):
    # Total time for the Gantt chart (end time of the last process)
    total_duration = max([s + d for s, d in zip(start_times, durations)])
    
    ax.set_ylim(0, 1)
    ax.set_xlim(-1, total_duration + 1)  # Add margins to the left and right
    ax.axis('off')  # Turn off the axis to remove the outer part
    
    # Display the title above the metrics with more vertical space
    ax.text(0, 1.8, title, ha='left', va='center', fontsize=12, weight='bold', color="black", transform=ax.transAxes)
   
    # Draw the Gantt chart
    for process, start, duration in zip(processes, start_times, durations):
        color = "#dc2f02" if process == "Idle" else process_color
        rect = patches.Rectangle(
            (start, 0.25), duration, 0.5, 
            edgecolor='black', facecolor=color
        )
        ax.add_patch(rect)
        ax.text(start + duration / 2, 0.5, process, ha='center', va='center', fontsize=10, color="black", weight="bold")
        
        # Display start and end times closer to the rectangles
        ax.text(start, 0.8, f"{start}", ha='center', va='center', fontsize=8)  # Start time
        ax.text(start + duration, 0.8, f"{start + duration}", ha='center', va='center', fontsize=8)  # End time

    # Display the passed metrics
    avg_waiting_time, avg_turnaround_time, cpu_utilization = metrics
    ax.text(0, 1.5, f"Avg Waiting Time: {avg_waiting_time:.2f}", ha='left', va='center', fontsize=10, transform=ax.transAxes)
    ax.text(0, 1.3, f"Avg Turnaround Time: {avg_turnaround_time:.2f}", ha='left', va='center', fontsize=10, transform=ax.transAxes)
    ax.text(0, 1.1, f"CPU Utilization: {cpu_utilization:.2f}%", ha='left', va='center', fontsize=10, transform=ax.transAxes)


# Global lists for each scheduler
processes1, start_times1, durations1, metrics1 = [], [], [], []
processes2, start_times2, durations2, metrics2 = [], [], [], []
processes3, start_times3, durations3, metrics3 = [], [], [], []

# Mapping to global variables for easier access in the function
global_vars = {
    1: (processes1, start_times1, durations1, metrics1),
    2: (processes2, start_times2, durations2, metrics2),
    3: (processes3, start_times3, durations3, metrics3),
}

def parse_and_store_data():
    # Read the input piped data
    input_data = sys.stdin.read()

    lines = input_data.strip().split("break")
    for idx, line in enumerate(lines):
        # Split each block into input lines
        sections = line.strip().split("\n")
        
        # Metrics are the first line of each block
        metrics = list(map(float, sections[0].split(", ")))
        scheduler_id = idx + 1  # To match scheduler id with 1, 2, 3
        
        # Set metrics
        global_vars[scheduler_id][3].extend(metrics)
        
        # Parse the processes, start times, and durations
        process_entries = sections[1].split("|")
        for entry in process_entries:
            entry = entry.strip()
            if entry:  # Skip empty entries
                parts = entry.split(":")
                if len(parts) == 2:  # Ensure there's a process and time part
                    process_number = parts[0].strip()
                    
                    # Check if process_number is -1 (idle state)
                    if process_number == "-1":
                        process_id = "Idle"  # Represent idle time as "Idle"
                    else:
                        process_id = f"P{process_number}"  # Otherwise, use P with process number
                    
                    times = parts[1].strip().split()
                    if len(times) == 2:  # Ensure there are exactly two time values
                        start_time, end_time = map(int, times)
                        duration = end_time - start_time
                        
                        # Append parsed values to respective global lists
                        global_vars[scheduler_id][0].append(process_id)
                        global_vars[scheduler_id][1].append(start_time)
                        global_vars[scheduler_id][2].append(duration)


def main():
    parse_and_store_data()

    fig, axs = plt.subplots(3, 1, figsize=(10, 10))

    # Plot each scheduler's Gantt chart with precomputed metrics
    plot_gantt_chart(axs[0], processes1, start_times1, durations1, "FCFS", metrics1)
    plot_gantt_chart(axs[1], processes2, start_times2, durations2, "SRT", metrics2)
    plot_gantt_chart(axs[2], processes3, start_times3, durations3, "RR", metrics3)

    # Adjust layout to add space between subplots
    plt.subplots_adjust(left=0.1, right=0.9, top=0.8, bottom=0.1, hspace=1.6)
    plt.show()

if __name__ == "__main__":
    main()
