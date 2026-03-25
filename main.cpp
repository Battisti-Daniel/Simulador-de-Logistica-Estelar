#include <cstring>
#include <iostream>

#include "Sensores.h"
#include "Cargas.h"

int main() {


    Sensors *sensors = createSensors();
    Loads *loads = createLoads();

    std::string names[6] = {"Sensor A", "Sensor B", "Sensor C", "Sensor D", "Sensor E", "Sensor F"};

    std::string namesToRemove[2] = {names[0]};

    for (std::string name: namesToRemove) {
        std::string removed = removeSensor(sensors, name);
        if (removed.empty()) {
            std::cout << "Nenhum sensor encontrado, tente novamente" << std::endl;
        }else {
            std::cout << "Sensor: " << removed << " ,removido com sucesso!" << std::endl;
        }

    }

    for (std::string name: names) {
            if (insertSensor(sensors, name)) {
                std::cout << "Sensor adicionado com sucesso!" << std::endl;
            }else {
                std::cout << "Nao ha mais espaco para adicionar sensores, tente remover antes de adicionar." << std::endl;
            }
    }

    insertLoad(loads,"Load 1");
    insertLoad(loads,"Load 2");
    insertLoad(loads,"Load 3");
    insertLoad(loads,"Load 4");
    insertLoad(loads,"Load 5");

    removeLoad(loads,1);
    removeLoad(loads,5);
    removeLoad(loads,4);


    return 0;
}