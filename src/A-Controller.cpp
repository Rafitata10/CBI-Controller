/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#include <Arduino.h>
#include <MasterSPI.h>

// Define los pines para SoftSPI.
#define PIN_MOSI 7 // 11
#define PIN_MISO 8 // 12
#define PIN_SCK  9 // 13
#define PIN_SS  10

struct SRTankData {
    unsigned char highFloater;
    unsigned char lowFloater;
    unsigned char minTemperature;
    unsigned char maxTemperature;
}; SRTankData tankData;

struct SRTankData2 {
    unsigned int popurri;
    unsigned int volume;
    unsigned int temperature;
}; SRTankData2 tankData2;

// Instancia de la clase SoftSPI.
MasterSPI masterSPI(PIN_MOSI, PIN_MISO, PIN_SCK);

unsigned char respuesta = 0;
unsigned char actualizado = 0;
unsigned char comando = 171;

void setup(){
    Serial.begin(9600);
    pinMode(PIN_SS, OUTPUT); // Configura el pin SS como salida.

    masterSPI.begin(); // Inicializa SoftSPI.
    masterSPI.setClockDivider(SPI_CLOCK_DIV64); // Divide el reloj entre 64.
    
    // Inicialización del SRTank.
    tankData.highFloater = 0;
    tankData.lowFloater = 0;
    tankData.maxTemperature = 0;
    tankData.minTemperature = 0;
    tankData2.popurri = 2;
    tankData2.volume = 2;
    tankData2.temperature = 2;
}

void imprimirSRTank(){
    Serial.print("SRTank: ");
    Serial.print(tankData.highFloater);
    Serial.print(", ");
    Serial.print(tankData.lowFloater);
    Serial.print(", ");
    Serial.print(tankData.minTemperature);
    Serial.print(", ");
    Serial.print(tankData.maxTemperature);
    Serial.print(", ");
    Serial.print(tankData2.volume);
    Serial.print(", ");
    Serial.println(tankData2.temperature);
}

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

// Define the function unPackSRTankData.
void unPackSRTankData(unsigned int popurri){
    tankData.highFloater = (popurri >> 3) & 0x1;
    tankData.lowFloater = (popurri >> 2) & 0x1;
    tankData.maxTemperature = (popurri >> 1) & 0x1;
    tankData.minTemperature = popurri & 0x1;
}

void loop(){
    delay(5000); // Espera 5 segundos.
    respuesta = transferir(comando); // Envía el comando.
    Serial.print("Respuesta: ");
    Serial.println(respuesta);
    respuesta = 0;

    if(actualizado){
        unPackSRTankData(tankData2.popurri);
        imprimirSRTank();
        actualizado = 0;
    }

    delay(5000); // Espera 5 segundos.
}

// Path: src/A-controller.cpp