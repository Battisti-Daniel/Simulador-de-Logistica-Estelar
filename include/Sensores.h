//
// Created by danie on 24/03/2026.
//

#ifndef TRABALH_M1_SENSORES_H
#define TRABALH_M1_SENSORES_H

#include <iostream>

#define MAX_SIZE 5

struct Sensors {

    std::string sensor[MAX_SIZE] = {};
    int counter = 0;
};

Sensors *createSensors();

bool insertSensor(Sensors *sensors, std::string name);

std::string removeSensor(Sensors *sensors, std::string name);
std::string removeSensor(Sensors *sensors, int index);

#endif //TRABALH_M1_SENSORES_H