#include "../include/Monitoramento.h"

#include <cstring>
#include <iostream>

#include "../include/Naves.h"

Monitoring *createMonitoring() {

    Monitoring *m = (Monitoring *)malloc(sizeof(Monitoring));
    m->head = NULL;
    m->tail = NULL;
    m->count = 0;
    return m;

}

bool insertCamera(Monitoring *monitoring, char name[]) {

    Node *node = createNode(name);

    if (monitoring->head == NULL) {
        monitoring->head = node;
        monitoring->tail = node;
        node->next = node;
        node->prev = node;
    }else {
        node->prev = monitoring->tail;
        node->next = monitoring->head;
        monitoring->tail->next = node;
        monitoring->head->prev = node;
        monitoring->tail = node;
    }
    monitoring->count += 1;
    return true;

}

bool removeCamera(Monitoring *monitoring, bool tail) {

    if (monitoring->count == 0 || monitoring->head == NULL) {
        return false;
    }

    Node *node;

    if (monitoring->count == 1) {
        free(monitoring->head);
        monitoring->head = NULL;
        monitoring->tail = NULL;
        monitoring->count = 0;
        return true;
    }

    if (tail) {
        node = monitoring->tail;
        monitoring->tail = node->prev;
        monitoring->tail->next = monitoring->head;
        monitoring->head->prev = monitoring->tail;
        free(node);
        node = NULL;
    } else {
        node = monitoring->head;
        monitoring->head = node->next;
        monitoring->head->prev = monitoring->tail;
        monitoring->tail->next = monitoring->head;
        free(node);
        node = NULL;
    }

    monitoring->count -= 1;
    return true;
}

bool removeCamera(Monitoring *monitoring, int index) {

    if (monitoring->head == NULL || index <= 0 || index > monitoring->count) {
        return false;
    }

    if (index == 1) {
        return removeCamera(monitoring, false);
    }
    if (index == monitoring->count) {
        return removeCamera(monitoring, true);
    }

    Node *node = monitoring->head;

    for (int i = 1; i < index; i++) {

        node = node->next;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    node = NULL;
    monitoring->count -= 1;

    return true;

}

bool removeCamera(Monitoring *monitoring, char name[]) {

    if (monitoring->head == NULL) {
        return NULL;
    }

    Node *current = monitoring->head;

    for (int i = 0; i < monitoring->count; i++) {

        if (strcmp(current->name, name) == 0) {

            if (current == monitoring->head) {
                return removeCamera(monitoring, false);
            }
            if (current == monitoring->tail) {
                return removeCamera(monitoring, true);
            }
            Node *aux = current;

            current->prev->next = current->next;
            current->next->prev = current->prev;

            free(aux);
            aux = NULL;

            monitoring->count -= 1;
            return true;

        }
        current = current->next;
    }
    return false;

}