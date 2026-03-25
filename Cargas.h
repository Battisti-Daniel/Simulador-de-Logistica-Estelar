//
// Created by danie on 25/03/2026.
//

#ifndef TRABALH_M1_CARGAS_H
#define TRABALH_M1_CARGAS_H

#include <iostream>

#define SIZE_CHAR 50

struct Node {

    char load[50];
    Node *next;

};

struct Loads {

    Node *head;
    Node *tail;
    int counter;

};

Loads *createLoads();

Node *createNode(char name[50]);

bool insertLoad(Loads *loads, char name[50]);
char removeLoad(Loads *loads, char name[50]);
char removeLoad(Loads *loads, int index);

#endif //TRABALH_M1_CARGAS_H