#include <Arduino.h>

void setup(){
  // Inicia la comunicación serial a 4800 baudios
  Serial.begin(4800);
}

void loop(){
  // Envía el comando para encender el LED
  Serial.println("LED encendido");
  Serial.write('H'); // H de "High"
  delay(1500); // Espera un segundo y medio
  
  // Envía el comando para apagar el LED
  Serial.println("LED apagado");
  Serial.write('L'); // L de "Low"
  delay(1500); // Espera un segundo y medio
}

