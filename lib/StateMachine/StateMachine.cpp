/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#include <StateMachine.h>

StateMachine::StateMachine(){}

void StateMachine::handleState(unsigned int& step, unsigned char& comando, TankData& tankData, TankData2& tankData2, unsigned int& respuesta, unsigned int master, unsigned int& numResets){
    // Máquina de estados de los pasos del proceso realizado en el tanque.
    switch(step){
        case _STEP0: // Llenar tanque hasta los 1500 litros con una solución ya mezclada de agua con sosa caústica.
            Serial.println(F("\nI am STEP0"));
            if(tankData2.volume >= 1250){
                step = _CHECK_STEP0;
                comando = _CLOSE_INLET;
            } else {
                comando = _OPEN_INLET;
            }
            break;
        case _CHECK_STEP0: // Checking STEP0.
            Serial.println(F("\nI am checking STEP0"));
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
        case _STEP1: // Calentar la solución a una temperatura de 35ºC.
            if(master == MASTER_2 && (counter == 2 || counter == 5)){
                Serial.println(F("\nSensor failure..."));
            } else {
                Serial.println(F("\nI am STEP1"));
            }            
            if(tankData2.temperature >= 34){
                step = _CHECK_STEP1;
                comando = _TURN_OFF_HEAT;
                counter = 0;
            } else {
                if(master == MASTER_2 && (counter == 2 || counter == 5)){
                    step = _RESETING;
                    comando = _STAY_CHILL;
                } else {
                   comando = _TURN_ON_HEAT; 
                }
                counter++;
            }
            break;
        case _CHECK_STEP1: // Checking STEP1.
            Serial.println(F("\nI am checking STEP1"));
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int temp = tankData2.temperature;
                if(temp >= 34 && temp <= prevTemp){
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
            Serial.println(F("\nI am STEP2"));
            if(tankData2.volume > 2750){
                step = _CHECK_STEP2;
                comando = _CLOSE_INLET2;
            } else {
                comando = _OPEN_INLET2;
            }
            break;
        case _CHECK_STEP2: // Checking STEP2.
            Serial.println(F("\nI am checking STEP2"));
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
        case _STEP3: // Mezclar con el agitador el contenido del tanque, (se removerá hasta que se alcancen los 50ºC).
            Serial.println(F("\nI am STEP3"));
            if(tankData2.temperature == 49){
                comando = _TURN_OFF_STIR;
            } else if(tankData2.temperature >= 50){
                step = _CHECK_STEP3;
            } else {
                comando = _TURN_ON_STIR;
            }
            break;
        case _CHECK_STEP3: // Checking STEP3.
            Serial.println(F("\nI am checking STEP3"));
            comando = _STAY_CHILL;
            if(counter == 1){
                if(tankData2.temperature >= 49 && (!tankData.revolutions)){
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
            Serial.println(F("\nI am STEP4"));
            if(tankData2.temperature <= 26){
                step = _CHECK_STEP4;
                comando = _TURN_OFF_COOL;
            } else {
                comando = _TURN_ON_COOL;
            }
            break;
        case _CHECK_STEP4: // Checking STEP4.
            Serial.println(F("\nI am checking STEP4"));
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int temp = tankData2.temperature;
                if(temp <= 26 && prevTemp == temp && (!tankData.minTemperature)){
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
            Serial.println(F("\nI am STEP5"));
            if(tankData2.volume < 250){
                step = _CHECK_STEP5;
                comando = _CLOSE_OUTLET;
            } else {
                comando = _OPEN_OUTLET;
            }
            break;
        case _CHECK_STEP5: // Checking STEP5.
            Serial.println(F("\nI am checking STEP5"));
            comando = _STAY_CHILL;
            if(counter == 2){
                unsigned int volume = tankData2.volume;
                if(volume == 0 && (!tankData.lowFloater)){
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
            delay(10);
            if(digitalRead(PIN_RESET) == LOW){
                if(tankData2.volume > 1000){
                    Serial.println(F("\nERROR!"));
                    step = _STEP7;
                    comando = _ABORT;
                    counter = 0;
                } else {
                    Serial.println(F("\nWaiting for a button to be pressed..."));
                    respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
                    numResets = 0;
                    while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón para iniciar el sistema.
                    step = _STEP0;
                    comando = _OPEN_INLET;
                }
            } else {
                comando = _STAY_CHILL;
                respuesta = 100; // Para no mostrar el estado del tanque al inicializar.              
            }
            break;
        case _STEP7: // ERROR - STATE.
            if(counter == 2){
                Serial.println(F("\nWaiting for an operator to fix the error..."));
                tank.resetTank();
                respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
                counter = 0;
                numResets = 0;
                while(digitalRead(PIN_START) == HIGH); // Espera a que se pulse el botón que indica que se ha solucionado el problema.
                step = _STEP0;
                comando = _OPEN_INLET;                
            } else {
                counter++;
                comando = _ABORT;
                respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
            }
            break;
        default: // ERROR-Abortar.
            Serial.println(F("\nI am Reseting"));
            comando = _STAY_CHILL;
            step = _RESETING;
            respuesta = 100; // Para no mostrar el estado del tanque al inicializar.
            break;
    }
}

// Define the function "desconvertir".
unsigned int StateMachine::desconvertir(unsigned int& code){
    unsigned int resp = _STEP0;
    switch(code){
        case _RESET0:
        case _RESET0*2:
            resp = _STEP0; break;
        case _RESET1:
        case _RESET1*2:
            resp = _STEP1; break;
        case _RESET2:
        case _RESET2*2:
            resp = _STEP2; break;
        case _RESET3:
        case _RESET3*2:
            resp = _STEP3; break;
        case _RESET4:
        case _RESET4*2:
            resp = _STEP4; break;
        case _RESET5:
        case _RESET5*2:
            resp = _STEP5; break;
        case _RESET6:
        case _RESET6*2:
            resp = _STEP6; break;
        default:
            break;
    }
    Serial.print(F("Reset to STEP"));
    Serial.println(resp);
    return resp;
}

void StateMachine::checkResponse(unsigned int& step, unsigned int& respuesta, Screen& myScreen, unsigned int& numResets){
    // Se ha enviado un posible comando de reset.
    if(respuesta != _EXPEC_RESP){
        if(numResets >= 2){
            Serial.println(F("\nERROR!"));
            step = _STEP7;
            myScreen.showStep(_STEP7);
        } else {
            switch(respuesta){
                case _RESET0:
                case _RESET1:
                case _RESET2:
                case _RESET3:
                case _RESET4:
                case _RESET5:
                case _RESET6: step = desconvertir(respuesta); myScreen.showStep(_RESETING); break;
                default:
                    break;
            }
            numResets++;           
        }
    } else {
        numResets = 0;
    }
}

// Path: lib/StateMachine/src/StateMachine.h