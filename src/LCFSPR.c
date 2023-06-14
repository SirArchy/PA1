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
        // Calculate the longest common burst time for all processes in the queue
        int longest_common_burst_time = arriving_process->burst_time;
        queue_object* current = LCFSPR_queue->next;
        while (current != NULL) {
            process* process_in_queue = (process*)current->object;
            if (process_in_queue->burst_time < longest_common_burst_time) {
                longest_common_burst_time = process_in_queue->burst_time;
            }
            current = current->next;
        }

        // Check if the arriving process has a burst time shorter than the longest common burst time
        if (arriving_process->burst_time < longest_common_burst_time) {
            // Add the arriving process to the front of the queue
            queue_object* new_queue_object = (queue_object*)malloc(sizeof(queue_object));
            new_queue_object->object = arriving_process;
            new_queue_object->next = LCFSPR_queue->next;
            LCFSPR_queue->next = new_queue_object;
        } else {
            // Add the arriving process to the end of the queue
            queue_add(arriving_process, LCFSPR_queue);
        }
    }

    return running_process;
}

void LCFSPR_finish(){
    free_queue(LCFSPR_queue);
}
