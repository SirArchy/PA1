#include "../lib/HRRN.h"

static queue_object* HRRN_queue;
//You can add more global variables and structs here

process* HRRN_tick (process* running_process){
    if (running_process == NULL || running_process->time_left == 0) {
        running_process = queue_poll(HRRN_queue);
    }
    if (running_process != NULL) {
        running_process->time_left--;
    }

    return running_process;
}

int HRRN_startup(){
    HRRN_queue = new_queue();
    if (HRRN_queue == NULL) {
        return 1;
    }
    return 0;
}

process* HRRN_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Calculate response ratio for the arriving process
        float rr = (float)(arriving_process->waiting_time + arriving_process->burst_time) / (float)arriving_process->burst_time;

        // Create a new HRRN_process to hold the process and its response ratio
        HRRN_process* new_hrrn_process = (HRRN_process*)malloc(sizeof(HRRN_process));
        new_hrrn_process->this_process = arriving_process;
        new_hrrn_process->waiting_time = 0;
        new_hrrn_process->rr = rr;

        // Add the HRRN_process to the queue
        queue_add(new_hrrn_process, HRRN_queue);
    }

    return running_process;
}

void HRRN_finish(){
    // Free the HRRN_process objects in the queue
    queue_object* current = HRRN_queue->next;
    while (current != NULL) {
        HRRN_process* hrrn_process = (HRRN_process*)current->object;
        free(hrrn_process);
        current = current->next;
    }

    // Free the queue
    free_queue(HRRN_queue);
}
