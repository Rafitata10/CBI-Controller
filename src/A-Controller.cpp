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
#include <StateMachine.h>

// Instancia de la clase SoftSPI.
MasterSPI masterSPI(PIN_MOSI, PIN_MISO, PIN_SCK);
// Instancia de la clase SRTank.
TankData tankData = tank.getInit();
TankData2 tankData2 = tank.getInit2();
// Instancia de la clase Screen.
Screen myScreen;
// Instancia de la clase StateMachine.
StateMachine myStateMachine;

// Variables para manejar la comunicación.
unsigned int respuesta = 0;
// Flag para saber si se ha actualizado el struct.
unsigned char actualizado = 0;
// Comando a enviar al comparator.
unsigned char comando = _OPEN_INLET;
// Variables para el estado del tanque.
unsigned int step = _STEP6;
unsigned int prevStep = _STEP6;
// Variable para el número de resets.
unsigned int numResets = 0;

// Configuración inicial.
void setup(){
    // Configura los pines.
    masterSPI.initPins();

    // Display setup.
    myScreen.initialize();

    // Inicializa SoftSPI.
    masterSPI.begin();
}

// Sync both controllers.
void syncController(){
    digitalWrite(PIN_SYNC_3, HIGH); // Avise al otro gemelo que está listo.
    unsigned long previousMillis = millis();
    while(digitalRead(PIN_SYNC_4) == LOW){ // Espera a que llegue el otro gemelo.
        // Comprobar si no debo esperar más para no bloquear el sistema.
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis >= _INVERVAL){
            break;
        }
    }
}

// Transfer the command to the slave.
unsigned char transferir(unsigned char comando){
    unsigned char resp = 0;

    digitalWrite(PIN_SS, LOW); // Habilita el esclavo.

    resp = masterSPI.transfer(comando); // Envia el comando para el tanque.

    // Si se recibe el comando esperado, se recibe el struct del esclavo.
    if(resp > 0){
        masterSPI.ReadAnything(tankData2); // Lee el struct del esclavo.
        actualizado = 1;
    }

    digitalWrite(PIN_SS, HIGH); // Deshabilita el esclavo.
    digitalWrite(PIN_SYNC_3, LOW); // Apaga el pin 3.

    return resp; // Devuelve la respuesta.
}

// Main loop.
void loop(){
    delay(750); // Espera 0.75 segundos.

    syncController(); // Sincroniza los dos controladores.
    
    respuesta = transferir(comando); // Envía el comando.
    if(respuesta <= 0 || respuesta >= 70){
        Serial.println(F("Connecting..."));
    }

    myStateMachine.checkResponse(step, respuesta, myScreen, numResets);

    // Selección del siguiente comando.
    myStateMachine.handleState(step, comando, tankData, tankData2, respuesta, MASTER_1, numResets);

    // Muestro el estado del tanque.
    if(actualizado && respuesta < 70){
        tank.unPackSRTankData(tankData2.popurri);
        tank.imprimirSRTank();
        actualizado = 0;
    }
    respuesta = 0;

    if(digitalRead(PIN_RESET) == HIGH){
        step = _RESETING;
    } else {
        digitalWrite(RESET_LED, LOW);
    }
    
    if(step != prevStep){
        // Show the current step on the screen.
        myScreen.showStep(step);
        prevStep = step;
    }

    delay(750); // Espera 0.75 segundos.
}

// Path: src/Controller.cpp