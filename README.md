


# Arduino AT Mega 2560 Project: External I/O Devices and I²C Communication

## Overview
This project was developed as the final assignment for the **Peripherals and Interfaces** course during my CS degree. The goal was to gain hands-on experience with external I/O devices and their communication with the microcontroller through an I²C Bus interface.

> **Note:** This project was tested using [Proteus](https://www.labcenter.com/), a simulation tool for electronic circuits.
## Project Description

In this assignment, we had to build an alarm clock from scratch. The provided description is as follows:

> The practical activity to be carried out consists of designing an alarm clock based on the ATmega 2560 microcontroller, which will serve as our embedded system. The basic functionalities of the alarm clock are based on the DS3232 real-time clock chip, which, once programmed, will be responsible for maintaining the system's date and time and managing up to 2 alarms, among other functions. Once the device has been designed and programmed, it should function like any standard household alarm clock.

> For the practical implementation, a base Proteus project will be provided with all the necessary components to implement the alarm clock functionality. [Figure 1](#figure-1) shows a block diagram of the device to be implemented, which basically consists of:
> 1. Liquid-Crystal Display (LCD) with 4 lines and 20 characters/line (serial bus).
> 2. DS3232 Real-Time Clock (i2c bus).
> 3. 4x3 keypad (parallel interconnection) and speaker or buzzer.

> Obviously, the device supports various designs, such as using buttons instead of a keypad, which would be a solution more in line with devices found in the market. However, for academic reasons, we have chosen to use a 4x3 keypad so that the student acquires the ability to control and manage a keypad in real-time, at least to switch between operating modes.

> The basic operation involves initializing the alarm clock (setting time, date, and possible alarms), after which the device will continuously display time, date, alarms, and the chip's temperature. At any time, adjustments can be made by entering the procedure: modification of date, time, and/or alarms.

> The system could be enhanced by adding some other functionality, such as operating in stopwatch mode, which is proposed as a potential improvement.

## Project Goals
-   Apply theoretical knowledge about the input/output system of a computer system.
-   Understand the practical aspects of the operation of certain basic components in external devices.
-   Integrate and connect basic components to make a device that meets a specific functionality.
-   Analyze, design, and implement control software for the proper operation of the device.
-   Verify and debug for proper functioning of the hardware-software integration. Redesign if necessary.

## Technical Description
#### I²C Interface
- **Memory Chip:** 24LC64 [2 Modules]
- **RTC Chip:** DS3232

#### Pins
- 4x3 Numpad
- Speaker
- 4 Buttons [UP, DOWN, LEFT, RIGHT, CENTER]
- 7 Segment display

#### Serial
- **4x20 LCD**: MILFORD 11357
---


##### Figure 1
> ![Project Schema](/doc/img/implementation-diagram.png)

## Installation and Usage
The installation should be pretty straight-forward, add all the files to the project and load it




