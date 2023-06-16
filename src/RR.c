#include "../lib/RR.h"

static queue_object* RR_queue;
//You can add more global variables
static int quantum;  // Quantum for Round-Robin scheduling


process* RR_tick (process* running_process){
    if (running_process == NULL) {
        // No running process, get the next process from the queue
        running_process = queue_poll(RR_queue);
    } else {
        // Reduce the remaining time of the running process
        running_process->time_left -= quantum;

        if (running_process->time_left <= 0) {
            // The running process has finished
            free(running_process);
            running_process = queue_poll(RR_queue);
        } else {
            // The running process needs to be requeued
            queue_add(running_process, RR_queue);
            running_process = queue_poll(RR_queue);
        }
    }

    return running_process;
}

int RR_startup(int quantum){
    RR_queue = new_queue();

    if (RR_queue == NULL) {
        return 1;  // Error creating the queue
    }

    return 0;
}


process* RR_new_arrival(process* arriving_process, process* running_process){
    // Add the arriving process to the queue
    queue_add(arriving_process, RR_queue);

    return running_process;
}

void RR_finish(){
    // Free the memory used by the queue
    free_queue(RR_queue);
}
