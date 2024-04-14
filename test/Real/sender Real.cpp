#include <Arduino.h>
#include <SPI.h>

void setup(){
  Serial.begin(115200);

  pinMode(SS, OUTPUT); // Configurar el pin SS como salida.

  SPI.begin(); // Inicializar SPI.
  // SPI.setClockDivider(SPI_CLOCK_DIV32); // Divide el reloj por 32.
}

void transferirChar(char comando){
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // Configurar la transferencia SPI.
  digitalWrite(SS, LOW); // Habilitar el esclavo.
  SPI.transfer(comando); // Enviar comando.
  digitalWrite(SS, HIGH); // Deshabilitar el esclavo.
  SPI.endTransaction(); // Finalizar la transferencia SPI.
}

void loop(){
  transferirChar('A'); // Enviar comando para encender el LED.
  delay(1500); // Esperar 1.5 segundos.
  transferirChar('Z'); // Enviar comando para apagar el LED.
  delay(1500); // Esperar 1.5 segundos.
}
