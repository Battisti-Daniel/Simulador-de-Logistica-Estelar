//
// Created by danie on 26/03/2026.
//

#ifndef TRABALH_M1_HANGAR_H
#define TRABALH_M1_HANGAR_H

#include "Naves.h"

struct Hangar {

    Node *head;
    Node *tail;
    int counter;

};

Hangar *createHangar();
bool enqueueHangar(Hangar *hangar, char name[]);
char *dequeueHangar(Hangar *hangar);

#endif //TRABALH_M1_HANGAR_H