/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#ifndef _MASTERSPI_H
#define _MASTERSPI_H

#include <Arduino.h>

// Define los pines para SoftSPI.
#define PIN_MOSI 7 // 11
#define PIN_MISO 8 // 12
#define PIN_SCK  9 // 13
#define PIN_SS  10
// Define los pines de sincronización.
#define PIN_SYNC_3 3
#define PIN_SYNC_4 4
// Define el pin para el botón de iniciar.
#define PIN_START 6
//Define el pin de sincronización.
#define PIN_RESET 11
// Define los posibles divisores de reloj.
#define MASTERSPI_CLOCK_DIV4 0x00
#define MASTERSPI_CLOCK_DIV16 0x01
#define MASTERSPI_CLOCK_DIV64 0x02
#define MASTERSPI_CLOCK_DIV128 0x03
#define MASTERSPI_CLOCK_DIV2 0x04
#define MASTERSPI_CLOCK_DIV8 0x05
#define MASTERSPI_CLOCK_DIV32 0x06

class MasterSPI {
    public:
        MasterSPI(uint8_t mosi, uint8_t miso, uint8_t sck);
        void initPins();
        void begin();
        void end();
        void setBitOrder(uint8_t);
        void setDataMode(uint8_t);
        void setClockDivider(uint32_t);
        bool receiveBit();
        uint8_t receiveByte();
        uint8_t transfer(uint8_t);
		uint16_t transfer16(uint16_t data);

        template <typename T> unsigned int WriteAnything(const T& value){
            const byte *p = (const byte*) &value;
            unsigned int i;
            for(i = 0; i < sizeof value; i++){
                this->transfer(*p++); // Usa el transfer de MasterSPI.
            }
            return i;
        }

        template <typename T> unsigned int ReadAnything(T& value){
            byte *p = (byte*) &value;
            unsigned int i;
            for(i = 0; i < sizeof value; i++){
                *p++ = this->transfer(0); // Usa el transfer de MasterSPI.
            }
            return i;
        }

    private:
        void wait(uint_fast8_t del);

    private:
        uint8_t _miso;
        uint8_t _mosi;
        uint8_t _sck;
        uint8_t _delay;
        uint8_t _cke;
        uint8_t _ckp;
        uint8_t _order;
};
#endif

// Path: lib/MasterSPI/MasterSPI.h