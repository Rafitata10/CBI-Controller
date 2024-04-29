/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

#include <MasterSPI.h>

MasterSPI::MasterSPI(uint8_t mosi, uint8_t miso, uint8_t sck){
    _mosi = mosi;
    _miso = miso;
    _sck = sck;
    _delay = 2;
    _cke = 0;
    _ckp = 0;
    _order = MSBFIRST;
}

// Inicializa la comunicación SPI del master.
void MasterSPI::begin(){
    // Inicializa el puerto serie.
    Serial.begin(9600);

    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    pinMode(_sck, OUTPUT);

    setClockDivider(SPI_CLOCK_DIV64); // Divide el reloj entre 64.
}

// Finaliza la comunicación SPI del master.
void MasterSPI::end(){
    pinMode(_mosi, INPUT);
    pinMode(_miso, INPUT);
    pinMode(_sck, INPUT);
}

// Establece el orden de los bits.
void MasterSPI::setBitOrder(uint8_t order){
    _order = order & 1;
}

// Establece el modo de los datos.
void MasterSPI::setDataMode(uint8_t mode){
    switch(mode){
        case SPI_MODE0:
            _ckp = 0;
            _cke = 0;
            break;
        case SPI_MODE1:
            _ckp = 0;
            _cke = 1;
            break;
        case SPI_MODE2:
            _ckp = 1;
            _cke = 0;
            break;
        case SPI_MODE3:
            _ckp = 1;
            _cke = 1;
            break;
    }
    digitalWrite(_sck, _ckp ? HIGH : LOW);
}

// Establece el divisor de reloj.
void MasterSPI::setClockDivider(uint32_t div){
    switch(div){
        case SPI_CLOCK_DIV2:
            _delay = 2;
            break;
        case SPI_CLOCK_DIV4:
            _delay = 4;
            break;
        case SPI_CLOCK_DIV8:
            _delay = 8;
            break;
        case SPI_CLOCK_DIV16:
            _delay = 16;
            break;
        case SPI_CLOCK_DIV32:
            _delay = 32;
            break;
        case SPI_CLOCK_DIV64:
            _delay = 64;
            break;
        case SPI_CLOCK_DIV128:
            _delay = 128;
            break;
        default:
            _delay = 128;
            break;
    }
}

// Espera un tiempo determinado.
void MasterSPI::wait(uint_fast8_t del){
    for(uint_fast8_t i = 0; i < del; i++){
        asm volatile("nop");
    }
}

/*
// Transfiere un byte. (Versión Original)
uint8_t MasterSPI::transfer(uint8_t val){
    uint8_t out = 0;
    if(_order == MSBFIRST){
        uint8_t v2 = 
            ((val & 0x01) << 7) |
            ((val & 0x02) << 5) |
            ((val & 0x04) << 3) |
            ((val & 0x08) << 1) |
            ((val & 0x10) >> 1) |
            ((val & 0x20) >> 3) |
            ((val & 0x40) >> 5) |
            ((val & 0x80) >> 7);
        val = v2;
    }

    uint8_t del = _delay >> 1;

     * CPOL := 0, CPHA := 0 => INIT = 0, PRE = Z|0, MID = 1, POST =  0
     * CPOL := 1, CPHA := 0 => INIT = 1, PRE = Z|1, MID = 0, POST =  1
     * CPOL := 0, CPHA := 1 => INIT = 0, PRE =  1 , MID = 0, POST = Z|0
     * CPOL := 1, CPHA := 1 => INIT = 1, PRE =  0 , MID = 1, POST = Z|1

    int sck = (_ckp) ? HIGH : LOW;

    for(uint8_t bit = 0u; bit < 8u; bit++){
        if(_cke){
            sck ^= 1;
            digitalWrite(_sck, sck);            
            wait(del);
        }

        // ... Write bit
        digitalWrite(_mosi, ((val & (1 << bit)) ? HIGH : LOW));

        wait(del);

        sck ^= 1u; digitalWrite(_sck, sck);

        uint8_t bval = 0;
        // ... Read bit
        {
            bval = digitalRead(_miso);

            if(_order == MSBFIRST){
                out << 1;
                out |= bval;
            } else {
                out >> 1;
                out |= bval << 7;
            }
        }*/
        /*
        if(digitalRead(_miso) == HIGH){
            // Ajusta la posición del bit correctamente para un bucle ascendente
            out |= (1 << (7 - bit));
        }

        wait(del);

        if(!_cke){
            sck ^= 1u;
            digitalWrite(_sck, sck);
        }
    }
    return out;
}*/

/*
// Transfiere un byte. (Versión con depuración)
uint8_t MasterSPI::transfer(uint8_t valToSend){
    uint8_t received = 0;
    String debugBitsSent = ""; // Para almacenar los bits enviados.
    //String debugBitsReceived = ""; // DEBUG-Para almacenar los bits recibidos.
    
    for(int i = 7; i >= 0; i--){
        // Ajusta el estado de SCK según CPOL.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);
        
        // Envía un bit: MSB primero.
        bool bitToSend = (valToSend & (1 << i)) ? HIGH : LOW;
        digitalWrite(_mosi, bitToSend);
        debugBitsSent += bitToSend ? "1" : "0"; // Almacena el bit enviado para depuración.
        
        // Ajusta el estado de SCK para leer el bit.
        digitalWrite(_sck, _ckp);
        
        // Espera de nuevo antes de leer el bit.
        wait(_delay);
        
        // Lee el bit recibido y ajusta el valor de 'received'.
        if(digitalRead(_miso) == HIGH){
            received |= (1 << i);
        }
        // DEBUG-Para almacenar los bits recibidos.
        //    debugBitsReceived += "1";
        //} else {
        //    debugBitsReceived += "0";
        //}

        wait(_delay);
        // Restablece el estado de SCK al final de la transferencia de este bit.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);
    }
    Serial.println(debugBitsSent); // DEBUG-Imprime los bits enviados para depuración.
    //Serial.println(debugBitsReceived);  // DEBUG-Imprime los bits recibidos para depuración.

    return received; // Devuelve el byte recibido.
}
*/

// Recibe un solo bit.
bool MasterSPI::receiveBit(){
    bool receivedBit = false;

    // Ajusta el estado de SCK según CPOL para comenzar la transferencia.
    digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);

    // Cambia el estado de SCK para leer el bit del esclavo.
    digitalWrite(_sck, _ckp);

    // Lee el bit enviado por el esclavo.
    receivedBit = digitalRead(_miso) == HIGH;

    // Restablece el estado de SCK al final de la transferencia de este bit.
    digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);

    return receivedBit; // Devuelve el bit recibido del slave.
}

// Recibe un byte.
uint8_t MasterSPI::receiveByte(){
    uint8_t receivedByte = 0;

    for(int i = 7; i >= 0; i--){
        // Ajusta el estado de SCK según CPOL para leer el bit.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);

        // Ajusta el estado de SCK para capturar el bit enviado por el esclavo.
        digitalWrite(_sck, _ckp);

        wait(2); // Espera un poco antes de leer el bit.

        // Lee el bit enviado por el esclavo.
        if(digitalRead(_miso) == HIGH){
            receivedByte |= (1 << i);
        }

        // Restablece el estado de SCK al final de la transferencia de este bit.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);
    }

    return receivedByte; // Devuelve el byte recibido.
}


// Transfiere y recibe un byte.
uint8_t MasterSPI::transfer(uint8_t valToSend){
    uint8_t received = 0;
    
    for(int i = 7; i >= 0; i--){
        // Ajusta el estado de SCK según CPOL.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);

        // Envía un bit: MSB primero.
        digitalWrite(_mosi, (valToSend & (1 << i)) ? HIGH : LOW);
        
        // Ajusta el estado de SCK para leer el bit.
        digitalWrite(_sck, _ckp);
                
        // Espera un poco antes de leer para dar tiempo al esclavo de preparar el bit.
        wait(_delay);

        // Lee el bit recibido y ajusta el valor de 'received'.
        if(digitalRead(_miso) == HIGH){
            received |= (1 << i);
        }

        // Restablece el estado de SCK al final de la transferencia de este bit.
        digitalWrite(_sck, (_ckp == HIGH) ? LOW : HIGH);
    }
    return received; // Devuelve el byte recibido.
}

// Transfiere un dato de 16 bits como dos bytes.
uint16_t MasterSPI::transfer16(uint16_t data){
	union {
		uint16_t val;
		struct {
			uint8_t lsb;
			uint8_t msb;
		};
	} in, out;
  
	in.val = data;

	if(_order == MSBFIRST){
		out.msb = transfer(in.msb);
		out.lsb = transfer(in.lsb);
	} else {
		out.lsb = transfer(in.lsb);
		out.msb = transfer(in.msb);
	}
	return out.val;
}

// Path: lib/MasterSPI/MasterSPI.cpp