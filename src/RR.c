#include "../lib/RR.h"

static queue_object* RR_queue;
//You can add more global variables

process* RR_tick (process* running_process){
    if (running_process == NULL) {
        // If there is no running process, select the next process from the queue
        running_process = queue_poll(RR_queue);
    } else if (running_process->time_left == 0) {
        // If the running process has completed its time slice (quantum), move it to the end of the queue
        queue_add(running_process, RR_queue);
        running_process = queue_poll(RR_queue);
    }

    if (running_process != NULL) {
        // Decrement the remaining time of the running process
        running_process->time_left--;
    }

    return running_process;
}

int RR_startup(int quantum){
    RR_queue = new_queue();
    if (RR_queue == NULL) {
        return 1; // Error: Failed to create the queue
    }

    RR_quantum = quantum;

    return 0;
}


process* RR_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Add the arriving process to the end of the queue
        queue_add(arriving_process, RR_queue);
    }

    return running_process;
}


void RR_finish(){
    free_queue(RR_queue);
}
