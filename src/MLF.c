#include "../lib/MLF.h"
#include <stdio.h>

static queue_object** MLF_queues;
//You can add more global variables here
static int num_queues;
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

process* MLF_tick (process* running_process){
    if (running_process == NULL || running_process->time_left == 0) {
        // Find the highest priority queue with a process
        int i;
        for (i = 0; i < num_queues; i++) {
            if (MLF_queues[i]->next != NULL) {
                running_process = (process*)queue_poll(MLF_queues[i]);
                break;
            }
        }
    }
    
    if (running_process != NULL) {
        running_process->time_left--;
    }

    return running_process;
}
/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int MLF_startup(){
    num_queues = 4;
    MLF_queues = (queue_object**)malloc(num_queues * sizeof(queue_object*));
    if (MLF_queues == NULL) {
        return 1;
    }

    int i;
    for (i = 0; i < num_queues; i++) {
        MLF_queues[i] = new_queue();
        if (MLF_queues[i] == NULL) {
            return 1;
        }
    }

    return 0;
}

process* MLF_new_arrival(process* arriving_process, process* running_process) {
    if (arriving_process != NULL) {
        // Calculate the queue index based on the factorial of its priority
        int queue_index = factorial(arriving_process->priority);

        // Add the arriving process to the appropriate queue
        queue_add(arriving_process, MLF_queues[queue_index]);
    }

    return running_process;
}

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void MLF_finish() {
    int i;
    for (i = 0; i < num_queues; i++) {
        free_queue(MLF_queues[i]);
    }
    free(MLF_queues);
}
