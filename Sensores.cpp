#include "Sensores.h"

#include <iostream>


Sensors *createSensors() {

    Sensors *sensors = new Sensors();

    std::cout << "Sensores Inicializados" << std::endl;

    return sensors;

}

bool insertSensor(Sensors *sensors, std::string name) {

    if (sensors->counter == MAX_SIZE) {
        return false;
    }

    std::string *aux = sensors->sensor;

    *(aux + sensors->counter) = name;

    sensors->counter += 1;

    return true;
}

std::string removeSensor(Sensors *sensors, std::string name) {

    if (sensors->counter == 0) {
        return "";
    }

    for (int i = 0; i < sensors->counter; i++) {
        if (sensors->sensor[i] == name) {
            std::string aux = sensors->sensor[i];

            if (sensors->counter == 1) {
                sensors->sensor[i] = "";
                sensors->counter -= 1;
                return aux;
            }

            for (int j = i; j < sensors->counter - 1; j++) {
                sensors->sensor[j] = sensors->sensor[j + 1];
            }

            sensors->counter -= 1;
            sensors->sensor[sensors->counter] = "";
            return aux;

        }
    }
    return "";
}

std::string removeSensor(Sensors *sensors, int index) {


    if (index > sensors->counter || index <= 0) {
        return "";
    }

    std::string aux = sensors->sensor[index - 1];

    sensors->counter -= 1;

    for (int i = index - 1; i < sensors->counter; i++) {

        sensors->sensor[i] = sensors->sensor[i + 1];

    }

    sensors->sensor[sensors->counter] = "";
    return aux;

}


