#include "../lib/LCFSPR.h"

static queue_object* LCFSPR_queue;
//You can add more global variables here
static process* running_process;

int LCFSPR_startup(){
    LCFSPR_queue = new_queue();
    if (LCFSPR_queue == NULL) {
        return 1;
    }
    return 0;
}

process* LCFSPR_tick (process* running_process){
    // Überprüfen, ob ein laufender Prozess vorhanden ist
    if (running_process != NULL) {
        running_process->time_left--;

        // Überprüfen, ob der laufende Prozess abgeschlossen ist
        if (running_process->time_left == 0) {
            free(running_process);
            running_process = NULL;
        }
    }

    // Den Prozess mit der höchsten Priorität aus der Warteschlange auswählen
    process* highest_priority_process = (process*)queue_peek(LCFSPR_queue);
    if (highest_priority_process != NULL) {
        if (running_process == NULL || highest_priority_process->priority < running_process->priority) {
            // Den Prozess mit der höchsten Priorität als den neuen laufenden Prozess festlegen
            if (running_process != NULL) {
                // Den bisherigen laufenden Prozess wieder in die Warteschlange einfügen
                queue_add(running_process, LCFSPR_queue);
            }
            running_process = (process*)queue_poll(LCFSPR_queue);
        }
    }

    return running_process;
}


process* LCFSPR_new_arrival(process* arriving_process, process* running_process){
    // Eingehenden Prozess zur Warteschlange hinzufügen
    queue_add(arriving_process, LCFSPR_queue);

    // Überprüfen, ob ein laufender Prozess vorhanden ist
    if (running_process == NULL) {
        // Den eingehenden Prozess als den neuen laufenden Prozess festlegen
        running_process = (process*)queue_poll(LCFSPR_queue);
    }

    return running_process;
}


void LCFSPR_finish(){
    // Alle verbleibenden Prozesse in der Warteschlange freigeben
    while (LCFSPR_queue->next != NULL) {
        process* remaining_process = (process*)queue_poll(LCFSPR_queue);
        free(remaining_process);
    }

    // Den laufenden Prozess freigeben, falls vorhanden
    if (running_process != NULL) {
        free(running_process);
        running_process = NULL;
    }

    // Die Warteschlange freigeben
    free_queue(LCFSPR_queue);
}
