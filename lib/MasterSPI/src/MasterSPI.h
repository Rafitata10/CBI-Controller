/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#ifndef _MASTERSPI_H
#define _MASTERSPI_H

#include <Arduino.h>
#include <SPI.h>

class MasterSPI {
    public:
        MasterSPI(uint8_t mosi, uint8_t miso, uint8_t sck);
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
                this->transfer(*p++); // Usa el transfer de SoftSPI.
            }
            return i;
        }

        template <typename T> unsigned int ReadAnything(T& value){
            byte *p = (byte*) &value;
            unsigned int i;
            for(i = 0; i < sizeof value; i++){
                *p++ = this->transfer(0); // Usa el transfer de SoftSPI.
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