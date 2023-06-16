#include "../lib/PRIONP.h"
#include <stdio.h>

static queue_object* PRIONP_queue;
//You can add more global variables here

process* PRIONP_tick (process* running_process){
    // Check if the running process is completed
    if (running_process != NULL && running_process->time_left == 0) {
        free(running_process);
        running_process = NULL;
    }

    // Get the highest priority process from the queue
    process* highest_priority_process = (process*)queue_peek(PRIONP_queue);

    // Check if there is a process in the queue
    if (highest_priority_process != NULL) {
        // Check if there is no running process or the new process has a higher priority
        if (running_process == NULL || highest_priority_process->priority < running_process->priority) {
            // Remove the process from the queue
            queue_poll(PRIONP_queue);
            // Set the new running process
            running_process = highest_priority_process;
        }
    }

    // Decrement the remaining time of the running process
    if (running_process != NULL)
        running_process->time_left--;

    return running_process;
}

int PRIONP_startup(){
    // Create the priority queue
    PRIONP_queue = new_queue();
    if (PRIONP_queue == NULL)
        return 1;

    return 0;
}

process* PRIONP_new_arrival(process* arriving_process, process* running_process){
    // Add the arriving process to the queue
    if (arriving_process != NULL)
        queue_add(arriving_process, PRIONP_queue);

    return running_process;
}

void PRIONP_finish(){
    free_queue(PRIONP_queue);
}
