/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>
#include <Tank.h>
#include <Screen.h>

// Define el pin para el botón de iniciar.
#define PIN_START 6
// Define los comandos para los resets.
#define _RESET0 10 // Reset to step 0.
#define _RESET1 11 // Reset to step 1.
#define _RESET2 12 // Reset to step 2.
#define _RESET3 13 // Reset to step 3.
#define _RESET4 14 // Reset to step 4.
#define _RESET5 15 // Reset to step 5.
#define _RESET6 16 // Reset to step 6.
#define _RESET7 17 // Reset to step 7.
// Define la respuesta esperada.
#define _EXPEC_RESP 35
// Define el intervalo de tiempo.
#define _INVERVAL 1500 // 1000
// Define los masters.
#define MASTER_1 1 // Master 1.
#define MASTER_2 2 // Master 2.
// Define el pin de reset.
#define PIN_RESET 11

class StateMachine {
public:
    StateMachine();
    void handleState(unsigned int& step, unsigned char& comando, TankData& tankData, TankData2& tankData2, unsigned int& respuesta, unsigned int master, unsigned int& numResets);
    void checkResponse(unsigned int& step, unsigned int& respuesta, Screen& myScreen, unsigned int& numResets);

public:
    // Variables to check the correctness of the tank operations.
    unsigned int counter = 0;
    unsigned int prevVolume = 0;
    unsigned int prevTemp = 0;

private:
    unsigned int desconvertir(unsigned int& code);
};

#endif

// Path: lib/StateMachine/src/StateMachine.h