/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#include <Screen.h>

Screen::Screen() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire){}

// Initialize the screen.
void Screen::initialize(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){ // Verifica la dirección I2C.
        for(;;); // Bucle infinito si la pantalla no se pudo inicializar.
    }
    display.display();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    showStep(_STEP6);
}

// Show the current step on the screen.
void Screen::showStep(unsigned int step){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(F("Step "));
    if(step < 8){
        display.println(step);
    } else {
        display.println(F("X"));
    }

    display.setCursor(0, 32);
    switch(step){
        case _STEP0:
        case _STEP2: display.println(F("Filling"));    break;
        case _STEP1: display.println(F("Heating"));    break;
        case _STEP3: display.println(F("Mixing"));     break;
        case _STEP4: display.println(F("Cooling"));    break;
        case _STEP5: display.println(F("Emptying"));   break;
        case _STEP6: display.println(F("Waiting...")); break;
        case _STEP7: display.println(F("ERROR!"));     break;

        case _CHECK_STEP0:
        case _CHECK_STEP1:
        case _CHECK_STEP2:
        case _CHECK_STEP3:
        case _CHECK_STEP4:
        case _CHECK_STEP5: display.println(F("Checking!")); break;

        case 33: display.println(F("Reseting")); break;

        default: display.println(F("Unknown")); break;
    }
    display.display();
}

// Path: lib/Screen/src/Screen.cpp