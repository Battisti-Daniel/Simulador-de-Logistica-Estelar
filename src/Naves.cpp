#include "../include/Naves.h"

#include <cstring>
#include <iostream>


Node *createNode(char name[]) {

    Node *node = (Node*)malloc(sizeof(Node));
    node->next = NULL;
    node->prev = NULL;
    strcpy(node->name, name);

    return node;

}

Ships *createShips() {

    Ships *ships = (Ships*)malloc(sizeof(Ships));

    ships->head = NULL;
    ships->tail = NULL;
    ships->count = 0;

    return ships;

}

bool insertShip(Ships *ships, char name[], bool tail) {

    Node *node = createNode(name);

    if (ships->head == NULL) {
        ships->head = node;
        ships->tail = node;
    }else {
        if (tail == true) {
            node->prev = ships->tail;
            ships->tail->next = node;
            ships->tail = node;
        }else {
            node->next = ships->head;
            ships->head->prev = node;
            ships->head = node;
        }
    }
    ships->count += 1;
    return true;

}

char* deleteShip(Ships *ships, bool tail) {

    if (ships->head == NULL) {
        return NULL;
    }

    char *content = (char*)malloc(50);
    Node * aux;

    if (ships->count == 1) {

        aux = ships->head;
        strcpy(content, aux->name);
        ships->head = NULL;
        ships->tail = NULL;
        free(aux);
        aux = NULL;
        ships->count -= 1;
        return content;

    }

    if (tail == true) {

        aux = ships->tail;
        strcpy(content, aux->name);
        ships->tail = ships->tail->prev;
        ships->tail->next = NULL;
        free(aux);
        aux = NULL;
    }else {
        aux = ships->head;
        strcpy(content, aux->name);
        ships->head = ships->head->next;
        ships->head->prev = NULL;
        free(aux);
        aux = NULL;
    }
    ships->count -= 1;
    return content;

}

char* deleteShip(Ships *ships, char name[]) {

    if (ships->head == NULL) {
        return NULL;
    }

    Node *current = ships->head;

    while (current != NULL) {

        if (strcmp(current->name, name) == 0) {

            if (current->prev == NULL) {
                return deleteShip(ships, false);
            }
            if (current->next == NULL) {
                return deleteShip(ships, true);
            }

            Node *aux = current;
            char *content = (char*)malloc(50);
            strcpy(content, aux->name);

            current->prev->next = current->next;
            current->next->prev = current->prev;

            free(aux);
            aux = NULL;

            ships->count -= 1;
            return content;


        }
        current = current->next;
    }
    return NULL;

}

char* deleteShip(Ships *ships, int index) {

    if (ships->head == NULL || index <= 0 || index > ships->count) {
        return NULL;
    }

    if (index == 1) {
        return deleteShip(ships, false);
    }
    if (index == ships->count) {
        return deleteShip(ships, true);
    }

    Node *current = ships->head;

    for (int i = 1; i < index; i++) {

        current = current->next;

    }

    char *content = (char*)malloc(50);
    strcpy(content, current->name);

    current->prev->next = current->next;
    current->next->prev = current->prev;

    free(current);
    current = NULL;
    ships->count -= 1;
    return content;


}
