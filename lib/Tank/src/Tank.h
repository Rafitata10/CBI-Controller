// Rafael Ramírez Salas
// Ingeniería de Computadores, Universidad de Málaga
// Trabajo de Fin de Grado 2024: Fail Tolerant DualNano

#ifndef Tank_h
#define Tank_h

#include <Arduino.h>

// Commands.
#define _OPEN_INLET    40 // Open inlet valve
#define _CLOSE_INLET   41 // Close inlet valve
#define _OPEN_INLET2   50 // Open inlet valve (for second time)
#define _CLOSE_INLET2  51 // Close inlet valve (for second time)
#define _OPEN_OUTLET   42 // Open outlet valve
#define _CLOSE_OUTLET  43 // Close outlet valve
#define _TURN_ON_HEAT  44 // Turn on heater
#define _TURN_OFF_HEAT 45 // Turn off heater
#define _TURN_ON_COOL  46 // Turn on cooler
#define _TURN_OFF_COOL 47 // Turn off cooler
#define _TURN_ON_STIR  48 // Turn on stirrer
#define _TURN_OFF_STIR 49 // Turn off stirrer
#define _STAY_CHILL    52 // Stay Chill
#define _AUXILIAR      59 // Auxiliar
#define _ABORT         69 // ABORT
// Steps:
#define _STEP0 0 // Llenar tanque hasta los 1500 litros con una solución ya mezclada de agua con sosa caústica.
#define _STEP1 1 // Calentar la solución a una temperatura de 35ºC.
#define _STEP2 2 // Rellenar con aceites hasta los 3000 litros.
#define _STEP3 3 // Mezclar con el agitador el contenido del tanque, (se removerá hasta que se alcancen los 50ºC).
#define _STEP4 4 // Enfriar la mezcla hasta los 25ºC ya que al tratarse de una reacción exotérmica, se calentará.
#define _STEP5 5 // Abrir la válvula de vaciado del tanque para procesar el resultado en una maquinaria que dividirá y empaquetará el jabón.
#define _STEP6 6 // Finish. (Wait for a button push).
#define _STEP7 7 // ERROR State.
#define _RESETING 33 // Reseting.
// Check steps:
#define _CHECK_STEP0 8  // Check STEP0.
#define _CHECK_STEP1 9  // Check STEP1.
#define _CHECK_STEP2 10 // Check STEP2.
#define _CHECK_STEP3 11 // Check STEP3.
#define _CHECK_STEP4 12 // Check STEP4.
#define _CHECK_STEP5 13 // Check STEP5.

struct TankData {
    unsigned char highFloater;
    unsigned char lowFloater;
    unsigned char minTemperature;
    unsigned char maxTemperature;
    unsigned char revolutions;
}; extern TankData tankData;

struct TankData2 {
    unsigned int popurri;
    unsigned int volume;
    unsigned int temperature;
}; extern TankData2 tankData2;

class Tank {
public:
    // Constructor.
    Tank();
    // Functions.
    TankData getInit(void);
    TankData2 getInit2(void);
    void unPackSRTankData(unsigned int popurri);
    void imprimirSRTank();
    void resetTank();

public:
    // Digital outputs.
    bool highFloater;
    bool lowFloater;
    bool minTemperature;
    bool maxTemperature;
    bool revolutions;
    // Analog outputs.
    unsigned int volume;
    unsigned int temperature;

private:
    // Auxiliar values.
    unsigned char valueH, valueL;
    unsigned int maxVolume = 3000, minVolume = 0, maxTemp = 150, 
    minTemp = 1, maxTempAlarm = 125, minTempAlarm = 10, stirTime = 1000;


}; extern Tank tank; // Declaración externa para uso global.


#endif

// Path: lib/Tank/Tank.h