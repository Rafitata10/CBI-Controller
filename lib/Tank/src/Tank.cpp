// Rafael Ramírez Salas
// Ingeniería de Computadores, Universidad de Málaga
// Trabajo de Fin de Grado 2024: Fail Tolerant DualNano

#include <Tank.h>

Tank tank;

// Constructor.
Tank::Tank() : highFloater(0), lowFloater(0), minTemperature(0), maxTemperature(0), volume(0), temperature(0) {
    ;
}

// Initialize the tank with default values.
TankData Tank::getInit(void){
    tankData.highFloater = 1;
    tankData.lowFloater = 1;
    tankData.maxTemperature = 1;
    tankData.minTemperature = 1;
    tankData.revolutions = 1;
    return tankData;
}

// Initialize the tank with default values.
TankData2 Tank::getInit2(void){
    tankData2.volume = 5;
    tankData2.temperature = 5;
    return tankData2;
}

// Define the function unPackSRTankData.
void Tank::unPackSRTankData(unsigned int popurri){
    tankData.highFloater    = (popurri >> 4) & 0x1;
    tankData.lowFloater     = (popurri >> 3) & 0x1;
    tankData.maxTemperature = (popurri >> 2) & 0x1;
    tankData.minTemperature = (popurri >> 1) & 0x1;
    tankData.revolutions    =  popurri       & 0x1;
}

// Screen print of the tank data.
void Tank::imprimirSRTank(){
    Serial.print("SRTank: ");
    Serial.print(tankData.highFloater);
    Serial.print(", ");
    Serial.print(tankData.lowFloater);
    Serial.print(", ");
    Serial.print(tankData.minTemperature);
    Serial.print(", ");
    Serial.print(tankData.maxTemperature);
    Serial.print(", ");
    Serial.print(tankData.revolutions);
    Serial.print(", ");
    Serial.print(tankData2.volume);
    Serial.print(", ");
    Serial.println(tankData2.temperature);
}

// path: lib/Tank/Tank.cpp