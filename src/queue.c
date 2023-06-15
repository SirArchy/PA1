#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void* new_object, queue_object* queue) {
    if (queue == NULL) {
        return 1; // Queue doesn't exist
    }

    queue_object* new_queue_obj = (queue_object*)malloc(sizeof(queue_object));
    if (new_queue_obj == NULL) {
        return 1; // Memory allocation failed
    }

    new_queue_obj->object = new_object;
    new_queue_obj->next = NULL;

    if (queue->next == NULL) {
        queue->next = new_queue_obj;
    } else {
        queue_object* temp = queue->next;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_queue_obj;
    }

    return 0; // Successfully added to the queue
}

void* queue_poll(queue_object* queue) {
    if (queue == NULL || queue->next == NULL) {
        return NULL; // Queue is empty
    }

    queue_object* removed_obj = queue->next;
    queue->next = removed_obj->next;
    void* object = removed_obj->object;
    free(removed_obj);
    return object;
}

queue_object* new_queue() {
    queue_object* queue = (queue_object*)malloc(sizeof(queue_object));
    if (queue != NULL) {
        queue->object = NULL;
        queue->next = NULL;
    }
    return queue;
}

void free_queue(queue_object* queue) {
    if (queue != NULL) {
        queue_object* current = queue->next;
        while (current != NULL) {
            queue_object* next = current->next;
            free(current);
            current = next;
        }
        free(queue);
    }
}

void* queue_peek(queue_object* queue) {
    if (queue == NULL || queue->next == NULL) {
        return NULL; // Queue is empty
    }
    return queue->next->object;
}
