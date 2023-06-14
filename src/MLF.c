#include "../lib/MLF.h"
#include <stdio.h>

static queue_object** MLF_queues;
//You can add more global variables here
static int num_queues;

process* MLF_tick (process* running_process){
    // Check if there is a running process
    if (running_process != NULL) {
        // Reduce the time left for the running process
        running_process->time_left--;

        // Check if the time slice for the current queue has expired
        if (running_process->time_left <= 0) {
            // Demote the process to a lower priority queue
            int current_queue = running_process->priority;
            int new_queue = (current_queue + 1) % num_queues;

            // Move the process to the new queue
            queue_add(running_process, MLF_queues[new_queue]);
            running_process = NULL;
        }
    }

    // Find the highest priority queue with a process
    int highest_priority_queue = -1;
    for (int i = 0; i < num_queues; i++) {
        if (MLF_queues[i]->next != NULL) {
            highest_priority_queue = i;
            break;
        }
    }

    // If there is a higher priority process, preempt the current running process
    if (highest_priority_queue != -1 && (running_process == NULL || running_process->priority > highest_priority_queue)) {
        running_process = queue_poll(MLF_queues[highest_priority_queue]);
    }

    return running_process;
}

/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int MLF_startup(){
    // TODO: Set the number of queues and allocate memory for the queue pointers
    num_queues = 3;  // Example: 3 priority queues
    MLF_queues = (queue_object**)malloc(num_queues * sizeof(queue_object*));

    // TODO: Initialize each queue
    for (int i = 0; i < num_queues; i++) {
        MLF_queues[i] = new_queue();
        if (MLF_queues[i] == NULL) {
            // TODO: Cleanup and return an error code if there is an allocation failure
            for (int j = 0; j < i; j++) {
                free_queue(MLF_queues[j]);
            }
            free(MLF_queues);
            return 1;
        }
    }

    return 0;
}

process* MLF_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Add the arriving process to the highest priority queue
        queue_add(arriving_process, MLF_queues[0]);
    }

    return running_process;
}

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void MLF_finish(){
    // TODO: Free the memory allocated for each queue
    for (int i = 0; i < num_queues; i++) {
        free_queue(MLF_queues[i]);
    }

    // TODO: Free the memory allocated for the queue pointers
    free(MLF_queues);
}
