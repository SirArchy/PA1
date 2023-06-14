#include "../lib/PRIONP.h"
#include <stdio.h>

static queue_object* PRIONP_queue;
//You can add more global variables here

process* PRIONP_tick (process* running_process){
    // In the PrioNP algorithm, the running process cannot be preempted
    // So we simply return the running process
    return running_process;
}

int PRIONP_startup(){
    PRIONP_queue = new_queue();
    if (PRIONP_queue == NULL) {
        return 1; // Error: Failed to create the queue
    }
    return 0;
}

process* PRIONP_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Add the arriving process to the queue based on its priority
        queue_add(arriving_process, PRIONP_queue);
    }

    if (running_process == NULL && PRIONP_queue->next != NULL) {
        // If there is no running process and the queue is not empty,
        // select the highest priority process as the new running process
        running_process = queue_poll(PRIONP_queue);
    }

    return running_process;
}

void PRIONP_finish(){
    free_queue(PRIONP_queue);
}
