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
#define PIN_SYNC 13

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
// Variables to check the correctness of the tank operations.
unsigned int counter = 0;
unsigned int prevVolume = 0;
unsigned int prevTemp = 0;
// Intervalo de tiempo para esperar al otro controlador.
const unsigned long interval = 1500;

// Configuración inicial.
void setup(){
    // Inicializa el puerto serie.
    Serial.begin(9600);
    // Configura los pines.
    pinMode(PIN_SS, OUTPUT); // Configura el pin SS como salida.
    pinMode(PIN_START, INPUT); // Configura el pin START como entrada.
    pinMode(PIN_SYNC, INPUT); // Configura el pin SYNC como entrada.

    // Inicializa SoftSPI.
    masterSPI.begin(); // Inicializa SoftSPI.
    masterSPI.setClockDivider(SPI_CLOCK_DIV64); // Divide el reloj entre 64.

    // GAP Between the two controllers.
    delay(25); // Espera 25 ms.
}

// Sync both controllers.
void syncController(){
    unsigned long previousMillis = millis();
    while(digitalRead(PIN_SYNC) == LOW){ // Espera a que llegue el otro gemelo.
        // Comprobar si no debo esperar más para no bloquear el sistema.
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis >= interval){
            break;
        }
    }
}

// Transfer the command to the slave.
unsigned char transferir(unsigned char comando){
    unsigned char resp = 0;

    digitalWrite(PIN_SS, LOW); // Habilita el esclavo.

    syncController(); // Sincroniza los dos controladores.

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
    delay(750); // Espera 0.75 segundos.
    respuesta = transferir(comando); // Envía el comando.
    if(respuesta > 0 && respuesta < 70){
        Serial.print("Respuesta: ");
        Serial.println(respuesta);        
    } else {
        Serial.println("Connecting...");
    }
    
    // Máquina de estados de los pasos del proceso realizado en el tanque.
    switch(step){
        case _STEP0: // Llenar tanque hasta los 1500 litros con una solución ya mezclada de agua con sosa caústica.
            Serial.println("I am STEP0");
            if(tankData2.volume >= 1250){

                step = _CHECK_STEP0;
                comando = _CLOSE_INLET;
            } else {
                comando = _OPEN_INLET;
            }
            break;
        case _CHECK_STEP0: // Checking STEP0.
            Serial.println("I am checking STEP0");
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int volume = tankData2.volume;
                if(volume >= 1485 && volume <= 1515 && prevVolume == volume && tankData.lowFloater){
                    step = _STEP1;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP0");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else if(counter == 1){
                prevVolume = tankData2.volume;
                counter++;
            } else {
                counter++;
            }
            break;
        case _STEP1: // Calentar la solución a una temperatura de 40ºC.
            Serial.println("I am STEP1");
            if(tankData2.temperature >= 39){
                step = _CHECK_STEP1;
                comando = _TURN_OFF_HEAT;
            } else {
                comando = _TURN_ON_HEAT;
            }
            break;
        case _CHECK_STEP1: // Checking STEP1.
            Serial.println("I am checking STEP1");
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int temp = tankData2.temperature;
                if(temp >= 39 && temp <= prevTemp){
                    step = _STEP2;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP1");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else if(counter == 1){
                prevTemp = tankData2.temperature;
                counter++;
            } else {
                counter++;
            }
            break;
        case _STEP2: // Rellenar con aceites hasta los 3000 litros.
            Serial.println("I am STEP2");
            if(tankData2.volume > 2750){
                step = _CHECK_STEP2;
                comando = _CLOSE_INLET2;
            } else {
                comando = _OPEN_INLET2;
            }
            break;
        case _CHECK_STEP2: // Checking STEP2.
            Serial.println("I am checking STEP2");
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int volume = tankData2.volume;
                if(volume == 3000 && volume == prevVolume && tankData.lowFloater && tankData.highFloater){
                    step = _STEP3;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP2");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else if(counter == 1){
                prevVolume = tankData2.volume;
                counter++;
            } else {
                counter++;
            }
            break;
        case _STEP3: // Mezclar con el agitador el contenido del tanque, (se removerá hasta que se alcancen los 60ºC).
            Serial.println("I am STEP3");
            if(tankData2.temperature == 59){
                comando = _TURN_OFF_STIR;
            } else if(tankData2.temperature >= 60){
                step = _CHECK_STEP3;
            } else {
                comando = _TURN_ON_STIR;
            }
            break;
        case _CHECK_STEP3: // Checking STEP3.
            Serial.println("I am checking STEP3");
            comando = _STAY_CHILL;
            if(counter == 1){
                if(tankData2.temperature >= 59 && (!tankData.revolutions)){
                    step = _STEP4;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP3");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else {
                counter++;
            }
            break;
        case _STEP4: // Enfriar la mezcla hasta los 25ºC ya que al tratarse de una reacción exotérmica, se calentará.
            Serial.println("I am STEP4");
            if(tankData2.temperature <= 25){
                step = _CHECK_STEP4;
                comando = _TURN_OFF_COOL;
            } else {
                comando = _TURN_ON_COOL;
            }
            break;
        case _CHECK_STEP4: // Checking STEP4.
            Serial.println("I am checking STEP4");
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int temp = tankData2.temperature;
                if(temp <= 25 && prevTemp == temp && (!tankData.minTemperature)){
                    step = _STEP5;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP4");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else if(counter == 1){
                prevTemp = tankData2.temperature;
                counter++;
            } else {
                counter++;
            }
            break;
        case _STEP5: // Abrir la válvula de vaciado del tanque para procesar el resultado en una maquinaria que dividirá y empaquetará el jabón.
            Serial.println("I am STEP5");
            if(tankData2.volume < 250){
                step = _CHECK_STEP5;
                comando = _CLOSE_OUTLET;
            } else {
                comando = _OPEN_OUTLET;
            }
            break;
        case _CHECK_STEP5: // Checking STEP5.
            Serial.println("I am checking STEP5");
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int volume = tankData2.volume;
                if(volume == 0 && (!tankData.lowFloater)){ // Faltaría comprobar que la válvula está cerrada.
                    step = _STEP6;
                    comando = _STAY_CHILL;
                    // Serial.println("Correct STEP5");
                } else {
                    step = _STEP7;
                    comando = _ABORT;
                }
                counter = 0;
            } else {
                counter++;
            }
            break;
        case _STEP6: // Finish.
            Serial.println("\nWaiting for a button to be pressed...");
            step = _STEP0;
            while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón para iniciar el sistema.
            comando = _STAY_CHILL;
            respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
            // GAP Between the two controllers.
            delay(25); // Espera 25 ms.
            break;
        case _STEP7: // ERROR - STATE.
            Serial.println("I am waiting for an operator to fix the error...");
            step = _STEP0;
            comando = _ABORT;
            respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
            tank.resetTank();
            while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón que indica que se ha solucionado el problema.
            // GAP Between the two controllers.
            delay(25); // Espera 25 ms.
            break;
        default: // ERROR-Abortar.
            Serial.println("Invalid STEP - Abort Now");
            comando = _ABORT;
            respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
            break;
    }

    // Muestro el estado del tanque.
    if(actualizado && respuesta < 70){
        tank.unPackSRTankData(tankData2.popurri);
        tank.imprimirSRTank();
        actualizado = 0;
    }
    respuesta = 0;

    delay(750); // Espera 0.75 segundos.
}

// Path: src/Controller.cpp