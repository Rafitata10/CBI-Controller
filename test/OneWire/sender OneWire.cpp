#include <Arduino.h>
#include <OneWire.h>

// Instancia de OneWire en el pin 5
OneWire oneWire(5);

void setup() {
  // No es necesario configurar el pin como salida porque OneWire lo hace.
}

void loop() {
  // Envía un pulso. Esto es solo un ejemplo y no representa un uso típico de OneWire.
  oneWire.reset(); // Esto genera un pulso de reinicio en el bus.
  delay(1000); // Espera 1 segundo
}

