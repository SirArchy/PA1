#include "../lib/SJN.h"

static queue_object* SJN_queue;
//You can add more global variables here

process* SJN_tick (process* running_process){
    if (running_process == NULL) {
        // If there is no running process, select the next process from the queue
        running_process = queue_poll(SJN_queue);
    }

    return running_process;
}

int SJN_startup(){
    SJN_queue = new_queue();
    if (SJN_queue == NULL) {
        return 1; // Error: Failed to create the queue
    }

    return 0;
}

process* SJN_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Add the arriving process to the queue based on its burst time
        process* current_process = SJN_queue->head;
        process* previous_process = NULL;

        while (current_process != NULL && arriving_process->burst_time > current_process->burst_time) {
            previous_process = current_process;
            current_process = current_process->next;
        }

        if (previous_process == NULL) {
            // The arriving process has the shortest burst time, so it becomes the new head of the queue
            SJN_queue->head = arriving_process;
        } else {
            // Insert the arriving process between previous_process and current_process
            previous_process->next = arriving_process;
        }

        arriving_process->next = current_process;
    }

    return running_process;
}

void SJN_finish(){
    free_queue(SJN_queue);
}
