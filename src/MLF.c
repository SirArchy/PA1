#include "../lib/MLF.h"
#include <stdio.h>

static queue_object** MLF_queues;
//You can add more global variables here
static int current_level;
static int MAX_LEVELS = 4;

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

process* MLF_tick (process* running_process){
    // Check if there is a running process
    if (running_process == NULL) {
        // Get the highest priority process from the highest non-empty queue
        for (int i = current_level; i >= 0; i--) {
            if (queue_peek(MLF_queues[i]) != NULL) {
                running_process = (process*)queue_poll(MLF_queues[i]);
                current_level = i;
                break;
            }
        }
    } else {
        // Decrement the remaining time of the running process
        running_process->time_left--;

        // Check if the time quantum for the current level has expired
        unsigned int time_quantum = factorial(current_level + 1);
        if (running_process->time_left > 0 && running_process->time_left % time_quantum == 0) {
            // Move the process to the next lower level
            if (current_level < MAX_LEVELS - 1) {
                current_level++;
                queue_add(running_process, MLF_queues[current_level]);
                running_process = NULL;
            }
        }

        // Check if the running process has completed its execution
        if (running_process->time_left == 0) {
            free(running_process);
            running_process = NULL;
        }
    }

    return running_process;
}
/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int MLF_startup(){
    // Initialize the MLF queues
    MLF_queues = (queue_object**)malloc(sizeof(queue_object*) * MAX_LEVELS);
    for (int i = 0; i < MAX_LEVELS; i++) {
        MLF_queues[i] = new_queue();
    }

    current_level = 0;

    return 0;
}

process* MLF_new_arrival(process* arriving_process, process* running_process) {
    // Check if there is a running process
    if (running_process == NULL) {
        running_process = arriving_process;
    } else {
        // Add the arriving process to the lowest level queue
        queue_add(arriving_process, MLF_queues[0]);
    }

    return running_process;
}

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void MLF_finish() {
    // Free the MLF queues
    for (int i = 0; i < MAX_LEVELS; i++) {
        free_queue(MLF_queues[i]);
    }
    free(MLF_queues);
}
