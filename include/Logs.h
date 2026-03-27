//
// Created by danie on 26/03/2026.
//

#ifndef TRABALH_M1_LOGS_H
#define TRABALH_M1_LOGS_H

#include "Naves.h"

struct Logs {

    Node *head;
    int counter;

};

Logs *createLog();
bool pushLog(Logs *logs, char name[]);
char *popLog(Logs *logs);

#endif //TRABALH_M1_LOGS_H