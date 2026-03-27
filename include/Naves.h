//
// Created by danie on 26/03/2026.
//

#ifndef TRABALH_M1_NAVES_H
#define TRABALH_M1_NAVES_H

#include <iostream>

struct Node {

    char name[50];
    Node *next;
    Node *prev;

};

struct Ships {

    int count;
    Node *head;
    Node *tail;

};

Ships *createShips();
Node *createNode(char name[]);
bool insertShip(Ships *ships, char name[], bool tail);
char* deleteShip(Ships *ships, bool tail);
char* deleteShip(Ships *ships, char name[]);
char* deleteShip(Ships *ships, int index);


#endif //TRABALH_M1_NAVES_H