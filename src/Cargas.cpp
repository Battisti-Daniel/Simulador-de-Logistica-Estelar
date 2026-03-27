#include "../include/Cargas.h"

#include <cstring>
#include <iostream>

#include "../include/Naves.h"

Loads *createLoads() {

    Loads *load = (Loads*)malloc(sizeof(Loads));
    load->counter = 0;
    load->head = NULL;
    load->tail = NULL;

    return load;

}

bool insertLoad(Loads *loads, char name[50]) {

    Node *node = createNode(name);

    if (loads->head == NULL) {
        loads->head = node;
        loads->tail = node;
    }else {
        node->next = loads->head;
        loads->head = node;
    }
    loads->counter += 1;
    return true;

}

char* removeLoad(Loads *loads, char name[50]) {

    if (loads->head == NULL) {
        return NULL;
    }

    Node *current = loads->head;
    Node *previous = NULL;

    while (current != NULL) {

            if (strcmp(name, current->name) == 0) {
                Node *remover = current;
                char *content = (char*)malloc(50);
                strcpy(content, current->name);

                if (current == loads->tail) {
                    loads->tail = previous;
                }

                if (previous == NULL) {
                    loads->head = current->next;
                }else {
                    previous->next = current->next;
                }
                free(remover);
                remover = NULL;
                loads->counter -= 1;
                return content;
            }

        previous = current;
        current = current->next;

    }
    return NULL;

}

char* removeLoad(Loads *loads, int index) {

    if (loads->head == NULL || index <= 0 || index > loads->counter) {
        return NULL;
    }

    Node *current = loads->head;
    Node *previous = NULL;

    for (int i = 1; i <= loads->counter; i++) {

        if (i == index) {
            Node *remover = current;
            char *content = (char*)malloc(50);
            strcpy(content, current->name);

            if (current == loads->tail) {
                loads->tail = previous;
            }

            if (previous == NULL) {
                loads->head = current->next;
            }else {
                previous->next = current->next;
            }
            free(remover);
            remover = NULL;
            loads->counter -= 1;
            return content;

        }

        previous = current;
        current = current->next;

    }

    return NULL;


}
