//
// Created by danie on 26/03/2026.
//

#ifndef TRABALH_M1_MONITORAMENTO_H
#define TRABALH_M1_MONITORAMENTO_H

#include<iostream>

#include "Naves.h"


struct Monitoring {

    int count;
    Node *head;
    Node *tail;

};

Monitoring *createMonitoring();
bool insertCamera(Monitoring *monitoring, char name[]);
bool removeCamera(Monitoring *monitoring, bool tail);
bool removeCamera(Monitoring *monitoring, int index);
bool removeCamera(Monitoring *monitoring, char name[]);

#endif //TRABALH_M1_MONITORAMENTO_H