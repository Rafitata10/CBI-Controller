/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Tank.h>

#define SCREEN_WIDTH 128 // Ancho en píxeles.
#define SCREEN_HEIGHT 64 // Altura en píxeles.
// Define el pin de reset.
#define RESET_LED 2

class Screen {
public:
    Screen();
    void initialize();
    void showStep(unsigned int step);
private:
    Adafruit_SSD1306 display;
};

#endif

// Path: lib/Screen/src/Screen.h