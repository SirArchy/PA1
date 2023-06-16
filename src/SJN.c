#include "../lib/SJN.h"

static queue_object* SJN_queue;
//You can add more global variables here
static process* running_process = NULL;

process* SJN_tick (process* running_process){
    return running_process;
}

int SJN_startup(){
    // Erstelle eine leere Warteschlange für den SJN-Algorithmus
    SJN_queue = new_queue();
    if (SJN_queue == NULL) {
        return 1; // Fehler beim Erstellen der Warteschlange
    }
    
    running_process = NULL; // Kein aktiver Prozess zu Beginn
    
    return 0; // Alles erfolgreich initialisiert
}

process* SJN_new_arrival(process* arriving_process, process* running_process){
    if (running_process == NULL) {
        // Es gibt keinen aktiven Prozess, der neu ankommende Prozess wird gestartet
        running_process = arriving_process;
    } else {
        // Es gibt einen aktiven Prozess
        if (arriving_process->time_left < running_process->time_left) {
            // Der neu ankommende Prozess hat eine kürzere verbleibende Zeit als der aktive Prozess
            // Der aktive Prozess wird in die Warteschlange verschoben und der neue Prozess wird gestartet
            queue_add(running_process, SJN_queue);
            running_process = arriving_process;
        } else {
            // Der neu ankommende Prozess hat eine längere oder gleiche verbleibende Zeit wie der aktive Prozess
            // Der neue Prozess wird in die Warteschlange verschoben
            queue_add(arriving_process, SJN_queue);
        }
    }
    
    return running_process;
}

void SJN_finish(){
    // Leere die Warteschlange und gib den Speicher frei
    free_queue(SJN_queue);
}
