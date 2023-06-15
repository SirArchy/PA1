#include "../lib/SJN.h"

static queue_object* SJN_queue;
//You can add more global variables here

process* SJN_tick (process* running_process){
    if (running_process == NULL) {
        // Kein Prozess wurde zuvor ausgeführt
        if (SJN_queue != NULL) {
            // Warteschlange ist nicht leer, wähle den Prozess mit der kürzesten Ausführungszeit aus
            running_process = (process*)queue_poll(SJN_queue);
        }
    } else {
        // Ein Prozess wurde zuvor ausgeführt
        if (SJN_queue != NULL) {
            // Warteschlange ist nicht leer, vergleiche die Ausführungszeit des laufenden Prozesses mit dem ersten Prozess in der Warteschlange
            process* next_process = (process*)queue_peek(SJN_queue);
            if (next_process != NULL && next_process->time_left < running_process->time_left) {
                // Der Prozess in der Warteschlange hat eine kürzere Ausführungszeit, wechsle zum nächsten Prozess
                running_process = (process*)queue_poll(SJN_queue);
                // Füge den aktuellen Prozess wieder in die Warteschlange ein
                queue_add(running_process, SJN_queue);
            }
        }
    }

    return running_process;
}

int SJN_startup(){
    // Erstelle die Warteschlange
    SJN_queue = new_queue();
    if (SJN_queue == NULL) {
        return 1;
    }

    return 0;
}

process* SJN_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process != NULL) {
        // Füge den neu ankommenden Prozess in die Warteschlange ein
        queue_add(arriving_process, SJN_queue);
    }

    // Überprüfe, ob ein Prozesswechsel erforderlich ist
    return SJN_tick(running_process);
}

void SJN_finish(){
    // Lösche die Warteschlange
    free_queue(SJN_queue);
    SJN_queue = NULL;
}
