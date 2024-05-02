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

// Configura los pines.
void MasterSPI::initPins(){
    pinMode(PIN_SS, OUTPUT); // Configura el pin SS como salida.
    pinMode(PIN_START, INPUT); // Configura el pin START como entrada.
    pinMode(PIN_RESET, INPUT); // Configura el pin RESET como entrada.
    pinMode(PIN_SYNC_3, OUTPUT); // Configura el pin 3 como salida.
    pinMode(PIN_SYNC_4, INPUT); // Configura el pin 4 como entrada.
}

// Inicializa la comunicación SPI del master.
void MasterSPI::begin(){
    // Inicialización del puerto serie.
    Serial.begin(9600);

    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    pinMode(_sck, OUTPUT);

    setClockDivider(MASTERSPI_CLOCK_DIV64); // División del reloj entre 64.
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

// Establece el divisor de reloj.
void MasterSPI::setClockDivider(uint32_t div){
    switch(div){
        case MASTERSPI_CLOCK_DIV2:
            _delay = 2;
            break;
        case MASTERSPI_CLOCK_DIV4:
            _delay = 4;
            break;
        case MASTERSPI_CLOCK_DIV8:
            _delay = 8;
            break;
        case MASTERSPI_CLOCK_DIV16:
            _delay = 16;
            break;
        case MASTERSPI_CLOCK_DIV32:
            _delay = 32;
            break;
        case MASTERSPI_CLOCK_DIV64:
            _delay = 64;
            break;
        case MASTERSPI_CLOCK_DIV128:
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