#include "../include/Hangar.h"

#include <cstring>


Hangar *createHangar() {

    Hangar *hangar = (Hangar *)malloc(sizeof(Hangar));
    hangar->head = NULL;
    hangar->tail = NULL;
    hangar->counter = 0;
    return hangar;

}

bool enqueueHangar(Hangar *hangar, char name[]) {

    Node *node = createNode(name);

    if (hangar->head == NULL) {
        hangar->head = node;
        hangar->tail = node;
    }else {
        node->prev = hangar->tail;
        hangar->tail->next = node;
        hangar->tail = node;
    }
    hangar->counter += 1;
    return true;

}

char *dequeueHangar(Hangar *hangar) {

    if (hangar->head == NULL) {
        return NULL;
    }

    Node *node = hangar->head;
    char *content = (char *)malloc(50);
    strcpy(content, node->name);

    hangar->head = node->next;
    if (hangar->head != NULL) {
        hangar->head->prev = NULL;
    }else {
        hangar->tail = NULL;
    }
    free(node);
    hangar->counter -= 1;

    return content;

}
