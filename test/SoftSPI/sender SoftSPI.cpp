#include <Arduino.h>
#include "SoftSPI.h"

// Define los pines para SoftSPI.
#define PIN_MOSI 7 // 11
#define PIN_MISO 8 // 12
#define PIN_SCK  9 // 13
#define PIN_SS  10

// Instanciación de la clase SoftSPI.
SoftSPI spi(PIN_MOSI, PIN_MISO, PIN_SCK);

void setup(){
  Serial.begin(9600);

  pinMode(PIN_SS, OUTPUT); // Configura el pin SS como salida.

  spi.begin(); // Inicializa SoftSPI.
  spi.setClockDivider(SPI_CLOCK_DIV32); // Divide el reloj por 32.
}

void transferirChar(char comando){
  Serial.println("Enviando comando: " + String(comando));
  digitalWrite(PIN_SS, LOW); // Habilita el esclavo.
  spi.transfer(comando); // Envia comando.
  digitalWrite(PIN_SS, HIGH); // Deshabilita el esclavo.
}

void loop(){
  transferirChar('A'); // Enviar comando para encender el LED.
  delay(1500); // Esperar 1.5 segundos.
  transferirChar('Z'); // Enviar comando para apagar el LED.
  delay(1500); // Esperar 1.5 segundos.
}
