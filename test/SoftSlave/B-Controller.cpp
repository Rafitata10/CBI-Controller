/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

// Incluye las librerías necesarias.
#include <Arduino.h>
#include <MasterSPI.h>
#include <Tank.h>
#include <Screen.h>

// Define los pines para SoftSPI.
#define PIN_MOSI 7 // 11
#define PIN_MISO 8 // 12
#define PIN_SCK  9 // 13
#define PIN_SS  10
// Define el pin para el botón de iniciar.
#define PIN_START 6
// Define el pin de sincronización.
#define PIN_SYNC 11

// Instancia de la clase SoftSPI.
MasterSPI masterSPI(PIN_MOSI, PIN_MISO, PIN_SCK);
// Instancia de la clase SRTank.
TankData tankData = tank.getInit();
TankData2 tankData2 = tank.getInit2();
// Instancia de la clase Screen.
Screen myScreen;

// Variables para manejar la comunicación.
unsigned char respuesta = 0;
// Flag para saber si se ha actualizado el struct.
unsigned char actualizado = 0;
// Comando a enviar al comparator.
unsigned char comando = _OPEN_INLET;
// Variable para el estado del tanque.
unsigned char step = _STEP6;

// Configuración inicial.
void setup(){
    // Inicializa el puerto serie.
    Serial.begin(9600);
    // Configura los pines.
    pinMode(PIN_SS, OUTPUT); // Configura el pin SS como salida.
    pinMode(PIN_START, INPUT); // Configura el pin START como entrada.

    // Inicializa SoftSPI.
    masterSPI.begin(); // Inicializa SoftSPI.
    masterSPI.setClockDivider(SPI_CLOCK_DIV64); // Divide el reloj entre 64.

    // GAP Between the two controllers.
    delay(50); // Espera 50 ms.
}

// Transfer the command to the slave.
unsigned char transferir(unsigned char comando){
    unsigned char resp = 0;

    digitalWrite(PIN_SS, LOW); // Habilita el esclavo.

    delay(8); // Espera un poco para enviar el comando.
    resp = masterSPI.transfer(comando); // Envia el comando para el tanque.

    // Si se recibe el comando esperado, se recibe el struct del esclavo.
    if(resp > 0){
        masterSPI.ReadAnything(tankData2); // Lee el struct del esclavo.
        actualizado = 1;
    }

    digitalWrite(PIN_SS, HIGH); // Deshabilita el esclavo.

    return resp; // Devuelve la respuesta.
}

// Main loop.
void loop(){
    delay(1000); // Espera 1 segundo.
    respuesta = transferir(comando); // Envía el comando.
    Serial.print("Respuesta: ");
    Serial.println(respuesta);
    respuesta = 0;

    if(actualizado){
        tank.unPackSRTankData(tankData2.popurri);
        tank.imprimirSRTank();
        actualizado = 0;
    }

    // Máquina de estados.
    switch(step){
        case _STEP0: // Llenar tanque hasta los 1500 litros con una solución ya mezclada de agua con sosa caústica.
            Serial.println("I am STEP0");
            if(tankData2.volume >= 1300){
                step = _CHECK_STEP0;
                comando = _CLOSE_INLET;
            } else {
                comando = _OPEN_INLET;
            }
            break;
        case _CHECK_STEP0: // Checking STEP0.
            Serial.println("I am checking STEP0");
            if(tankData2.volume >= 1425 && tankData.lowFloater){ // Faltaría comprobar que la válvula está cerrada.
                step = _STEP1;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP1: // Calentar la solución a una temperatura de 40ºC.
            Serial.println("I am STEP1");
            if(tankData2.temperature >= 40){
                step = _CHECK_STEP1;
                comando = _TURN_OFF_HEAT;
            } else {
                comando = _TURN_ON_HEAT;
            }
            break;
        case _CHECK_STEP1: // Checking STEP1.
            Serial.println("I am checking STEP1");
            if(tankData2.temperature >= 39){ // Faltaría comprobar que el heater está apagado.
                step = _STEP2;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP2: // Rellenar con aceites hasta los 3000 litros.
            Serial.println("I am STEP2");
            if(tankData2.volume >= 2990){
                step = _CHECK_STEP2;
                comando = _CLOSE_INLET2;
            } else {
                comando = _OPEN_INLET2;
            }
            break;
        case _CHECK_STEP2: // Checking STEP2.
            Serial.println("I am checking STEP2");
            if(tankData2.volume >= 3000 && tankData.highFloater){ // Faltaría comprobar que la válvula está cerrada.
                step = _STEP3;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP3: // Mezclar con el agitador el contenido del tanque, (se removerá hasta que se alcancen los 60ºC).
            Serial.println("I am STEP3");
            if(tankData2.temperature == 58){
                comando = _TURN_OFF_STIR;
            } else if(tankData2.temperature >= 59){
                step = _CHECK_STEP3;
            } else {
                comando = _TURN_ON_STIR;
            }
            break;
        case _CHECK_STEP3: // Checking STEP3.
            Serial.println("I am checking STEP3");
            if(tankData2.temperature >= 58 && (!tankData.revolutions)){
                step = _STEP4;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP4: // Enfriar la mezcla hasta los 35ºC ya que al tratarse de una reacción exotérmica, se calentará.
            Serial.println("I am STEP4");
            if(tankData2.temperature <= 35){
                step = _CHECK_STEP4;
                comando = _TURN_OFF_COOL;
            } else {
                comando = _TURN_ON_COOL;
            }
            break;
        case _CHECK_STEP4: // Checking STEP4.
            Serial.println("I am checking STEP4");
            if(tankData2.temperature <= 35 && (!tankData.minTemperature)){ // Faltaría comprobar que el cooler está apagado.
                step = _STEP5;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP5: // Abrir la válvula de vaciado del tanque para procesar el resultado en una maquinaria que dividirá y empaquetará el jabón.
            Serial.println("I am STEP5");
            if(tankData2.volume <= 10){
                step = _CHECK_STEP5;
                comando = _CLOSE_OUTLET;
            } else {
                comando = _OPEN_OUTLET;
            }
            break;
        case _CHECK_STEP5: // Checking STEP5.
            Serial.println("I am checking STEP5");
            if(tankData2.volume == 0 && (!tankData.lowFloater)){ // Faltaría comprobar que la válvula está cerrada.
                step = _STEP6;
                comando = _STAY_CHILL;
            } else {
                step = _STEP7;
                comando = _ABORT;
            }
            break;
        case _STEP6: // Finish.
            Serial.println("\nWaiting for a button push...");
            while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón.
            step = _STEP0;
            comando = _STAY_CHILL;
            // GAP Between the two controllers.
            delay(50); // Espera 50 ms.
            break;
        case _STEP7: // ERROR - STATE.
            Serial.println("I am waiting for an operator to fix the error...");
            while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón que indica que se ha solucionado el problema.
            step = _STEP0;
            comando = _STAY_CHILL;
            // GAP Between the two controllers.
            delay(50); // Espera 50 ms.
            break;
        default: // ERROR-Abortar.
            Serial.println("Invalid STEP - Abort Now");
            comando = _ABORT;
            break;
    }

    delay(1000); // Espera 1 segundo.
}

// Path: src/Controller.cpp