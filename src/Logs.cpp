#include "../include/Logs.h"

#include <cmath>
#include <cstring>

Logs *createLog() {

    Logs *logs = (Logs *) malloc(sizeof(Logs));
    logs->head = NULL;
    logs->counter = 0;

    return logs;
}

bool pushLog(Logs *logs, char name[]) {

    Node *node = createNode(name);
    strcpy(node->name, name);

    if (logs->head == NULL) {
        logs->head = node;
    }else {
        node->prev = logs->head;
        logs->head = node;
    }

    logs->counter += 1;
    return true;

}

char *popLog(Logs *logs) {

    if (logs->head == NULL) {
        return NULL;
    }

    Node *node = logs->head;
    char *content = (char *)malloc(50);
    strcpy(content, node->name);

    if (logs->head->prev != NULL) {
        logs->head = logs->head->prev;
        logs->head->next = NULL;
    }else {
        logs->head = NULL;
    }

    free(node);
    node = NULL;
    logs->counter -= 1;

    return content;

}
