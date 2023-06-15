#include "../lib/LCFSPR.h"

static queue_object* LCFSPR_queue;
//You can add more global variables here

int LCFSPR_startup(){
    LCFSPR_queue = new_queue();
    if (LCFSPR_queue == NULL) {
        return 1;
    }
    return 0;
}

process* LCFSPR_tick (process* running_process){
    if (running_process == NULL || running_process->time_left == 0) {
        running_process = queue_poll(LCFSPR_queue);
    }
    if (running_process != NULL) {
        running_process->time_left--;
    }
    return running_process;
}


process* LCFSPR_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Add the arriving process to the queue
        queue_add(arriving_process, LCFSPR_queue);
    }
    // Check if there is a running process
    if (running_process == NULL) {
        // Get the process with the highest priority from the queue
        running_process = queue_poll(LCFSPR_queue);
    }
    return running_process;
}


void LCFSPR_finish(){
    free_queue(LCFSPR_queue);
}
