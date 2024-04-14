#include <Arduino.h>

#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6
#define PIN_7 7
#define PIN_8 8
#define PIN_9 9
#define PIN_10 10
#define PIN_11 11
#define PIN_12 12
#define PIN_13 13

int aux;

void setup(){
    aux = 0;
    pinMode(PIN_0, OUTPUT);
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PIN_3, OUTPUT);
    pinMode(PIN_4, OUTPUT);
    pinMode(PIN_5, OUTPUT);
    pinMode(PIN_6, OUTPUT);

    pinMode(PIN_7, INPUT);
    pinMode(PIN_8, INPUT);
    pinMode(PIN_9, INPUT);
    pinMode(PIN_10, INPUT);
    pinMode(PIN_11, INPUT);
    pinMode(PIN_12, INPUT);
    pinMode(PIN_13, INPUT);
}

void loop(){
    if(aux % 2 == 0){
        digitalWrite(PIN_0, HIGH);
        digitalWrite(PIN_1, HIGH);
        digitalWrite(PIN_2, HIGH);
        digitalWrite(PIN_3, HIGH);
        digitalWrite(PIN_4, HIGH);
        digitalWrite(PIN_5, HIGH);
        digitalWrite(PIN_6, HIGH);
    } else {
        digitalWrite(PIN_0, LOW);
        digitalWrite(PIN_1, LOW);
        digitalWrite(PIN_2, LOW);
        digitalWrite(PIN_3, LOW);
        digitalWrite(PIN_4, LOW);
        digitalWrite(PIN_5, LOW);
        digitalWrite(PIN_6, LOW);
    }
    aux++;
    delay(2000);
}
