#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void* new_object, queue_object* queue) {
    // Allocate memory for a new queue_object
    queue_object* new_queue_object = (queue_object*)malloc(sizeof(queue_object));
    if (new_queue_object == NULL) {
        // Memory allocation failed
        return 1;
    }

    // Set the object and next pointer of the new queue_object
    new_queue_object->object = new_object;
    new_queue_object->next = NULL;

    // If the queue is empty, set the new queue_object as the head of the queue
    if (queue->next == NULL) {
        queue->next = new_queue_object;
    } else {
        // Find the last queue_object in the queue
        queue_object* current = queue->next;
        while (current->next != NULL) {
            current = current->next;
        }

        // Append the new queue_object to the end of the queue
        current->next = new_queue_object;
    }

    return 0;
}

void* queue_poll(queue_object* queue) {
    if (queue->next == NULL) {
        // The queue is empty
        return NULL;
    }

    // Get the oldest item from the queue
    queue_object* oldest = queue->next;
    void* oldest_object = oldest->object;

    // Update the head of the queue
    queue->next = oldest->next;

    // Free the memory of the removed queue_object
    free(oldest);

    return oldest_object;
}

queue_object* new_queue() {
    // Allocate memory for the head queue_object
    queue_object* head = (queue_object*)malloc(sizeof(queue_object));
    if (head == NULL) {
        // Memory allocation failed
        return NULL;
    }

    // Initialize the head queue_object
    head->object = NULL;
    head->next = NULL;

    return head;
}

void free_queue(queue_object* queue) {
    // Free all queue_objects in the queue
    queue_object* current = queue->next;
    while (current != NULL) {
        queue_object* next = current->next;
        free(current);
        current = next;
    }

    // Free the head queue_object
    free(queue);
}

void* queue_peek(queue_object* queue) {
    if (queue->next == NULL) {
        // The queue is empty
        return NULL;
    }

    // Get the oldest item from the queue
    return queue->next->object;
}
