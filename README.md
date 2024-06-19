/*
  * Rafael Ramírez Salas
  * Ingeniería de Computadores, Universidad de Málaga
  * Trabajo de Fin de Grado 2024: Fail Tolerant DualNano
*/

## Fail Tolerant DualNano - CBI-Controller

This project is developed by Rafael Ramírez Salas as part of his final year project at the University of Málaga, Computer Engineering department. The project is titled "Fail Tolerant DualNano".

## Overview

The project is a fail-tolerant system implemented on an Arduino platform. It uses several libraries such as MasterSPI, Tank, Screen, and StateMachine to manage the system's operations.
This project develops a controller for a serial remote tank using Arduino. As a redundant system, there are two copies, A-Controller and B-Controller.

## Components

- `MasterSPI`: This is a software SPI (Serial Peripheral Interface) library for Arduino. It is used to establish communication between the Arduino and other SPI devices.

- `Tank`: This library is used to manage the data of the tank system. It provides methods to initialize and update the tank data.

- `Screen`: This library is used to manage the display screen of the system.

- `StateMachine`: This library is used to manage the state of the system. It provides methods to transition between different states based on the system's conditions.

## Setup

To set up the project, you need to have the Arduino IDE installed on your system. Then, download the project files and open them in the Arduino IDE. Make sure to install the necessary libraries mentioned above. You can use VS Code with PlatformIO extension as well.

## Environment Setup

This project is developed for the Arduino Uno platform using the Atmel AVR microcontrollers. The code is written in the Arduino framework.

The project is configured with the PlatformIO IDE. The configuration is specified in the `platformio.ini` file.

## Usage

Upload the code to your Arduino board using the Arduino IDE. The system should start running immediately after the upload is complete. You can use VS Code with PlatformIO extension as well.

## Contributing

Contributions are welcome. Please open an issue to discuss your ideas before making a change.

## License

This project is licensed under the terms of the MIT license.