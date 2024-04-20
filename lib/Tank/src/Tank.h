// Rafael Ramírez Salas
// Ingeniería de Computadores, Universidad de Málaga
// Trabajo de Fin de Grado 2024: Fail Tolerant DualNano

#ifndef Tank_h
#define Tank_h

#include <Arduino.h>

// Commands.
#define _OPEN_INLET    0x40 // Open inlet valve
#define _CLOSE_INLET   0x41 // Close inlet valve
#define _OPEN_INLET2   0x50 // Open inlet valve (for second time)
#define _CLOSE_INLET2  0x51 // Close inlet valve (for second time)
#define _OPEN_OUTLET   0x42 // Open outlet valve
#define _CLOSE_OUTLET  0x43 // Close outlet valve
#define _TURN_ON_HEAT  0x44 // Turn on heater
#define _TURN_OFF_HEAT 0x45 // Turn off heater
#define _TURN_ON_COOL  0x46 // Turn on cooler
#define _TURN_OFF_COOL 0x47 // Turn off cooler
#define _TURN_ON_STIR  0x48 // Turn on stirrer
#define _TURN_OFF_STIR 0x49 // Turn off stirrer
#define _STAY_CHILL    0x52 // Stay Chill
#define _AUXILIAR      0x59 // Auxiliar
#define _ABORT         0x69 // ABORT
// Steps:
#define _STEP0 0x00 // Llenar tanque hasta los 1000 litros con una solución ya mezclada de agua con sosa caústica.
#define _STEP1 0x01 // Calentar la solución a una temperatura de 40ºC.
#define _STEP2 0x02 // Rellenar con aceites hasta los 2000 litros.
#define _STEP3 0x03 // Mezclar con el agitador el contenido del tanque, (se removerá hasta que se alcancen los 60ºC).
#define _STEP4 0x04 // Enfriar la mezcla hasta los 35ºC ya que al tratarse de una reacción exotérmica, se calentará.
#define _STEP5 0x05 // Abrir la válvula de vaciado del tanque para procesar el resultado en una maquinaria que dividirá y empaquetará el jabón.
#define _STEP6 0x06 // Finish. (Wait for a button push).
#define _STEP7 0x07 // ERROR State.
// Check steps:
#define _CHECK_STEP0 0x08  // Check STEP0.
#define _CHECK_STEP1 0x09  // Check STEP1.
#define _CHECK_STEP2 0x010 // Check STEP2.
#define _CHECK_STEP3 0x011 // Check STEP3.
#define _CHECK_STEP4 0x012 // Check STEP4.
#define _CHECK_STEP5 0x013 // Check STEP5.

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