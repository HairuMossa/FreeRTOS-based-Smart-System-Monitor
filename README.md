# FreeRTOS Based Smart System Monitor

## Introduction 
This repository contains the firmware for a Real-Time Operating System (RTOS) project based on the STM32F4-Discovery board. 
The system is designed as a dual-state device that monitors its physical orientation using the onboard LIS3DSH 3-axis accelerometer.

The core functionality is built around two mutually exclusive operating modes: IDLE (Standby) and RUNNING (Active). 
Users can switch between these modes using a physical push button or by sending commands ('1' for RUNNING, '0' for IDLE) via a UART terminal.

In the RUNNING state, the system provides real-time visual feedback using three LEDs (Green, Orange, Blue) to indicate alignment with 
gravity along the X, Y, and Z axes. A Red LED always indicates the current system state (blinking for IDLE, solid for RUNNING).

---

## RTOS Architecture 
<img width="1164" height="539" alt="image" src="https://github.com/user-attachments/assets/d2544e33-8af5-47a3-9bfc-f3e3bb52c0bf" />

## Architecuture overview
This Real-Time Operating System (RTOS) architecture is designed to manage system states, handle asynchronous user input, read 
sensor data periodically, and safely output information. The design separates interrupt-driven inputs, processing logic, resource
protection, and output mechanisms into distinct, concurrent tasks and communication channels.

### Inter-Task Communication (ITC)
The system utilizes queues for message passing and a mutex for shared resource protection. 
- **Command Queue (Queue)** - This is the primary input channel. Interrupt Service Routines (ISRs) for the Button and UART RX send 
command signals to this queue. The Manager Task blocks and waits on this queue, processing commands only when new data arrives.
- **Print Queue (Queue)** - Tasks use this queue to safely send formatted strings for output. It decouples the application tasks from
the UART transmission process.
- **System State Mutex (Mutex)** - A binary semaphore used to protect the global system state definition (e.g., IDLE or RUNNING).
The Manager Task writes to the protected state, while the Sensor Task reads it.

### System Tasks
The architecture is built around three main tasks, each with a specific role. 

**1. Manager Task** (Event-driven, wakes only when a new command arrives in the Command Queue)
- Role
    - Reads and processes commands originating from the Button or UART RX interrupts.
    - Updates the System State (protected by the Mutex) between IDLE and RUNNING.
    - Sends status strings (e.g., "Button Pressed," "UART Message Received") to the Print Queue.

**2. Sensor Task** (runs periodically every 500 millisecond)
- Role, Manages the LIS3DSH accelerometer sensor and provides visual feedback.
    - Initializes and configures the LIS3DSH sensor.
    - Reads the System State (protected by the Mutex) to determine its operation mode:
        - If IDLE: Blinks a Red LED every 100 ms.
        - If RUNNING: Turns the Red LED solid, reads the acceleration data, and controls three alignment indicators:
            - X-Axis Alignment (Green Indicator): ON if X-axis reading is between 0.9g and 1.0g (aligned with gravity); OFF otherwise.
            - Y-Axis Alignment (Orange Indicator): ON if Y-axis reading is between 0.9g and 1.0g; OFF otherwise.
            - Z-Axis Alignment (Blue Indicator): ON if Z-axis reading is between 0.9g and 1.0g; OFF otherwise.
    - Prints system state

**3. Print Task** (Event-driven, wakes only when a string arrives in the Print Queue)
- Role
    - Sends the received string data to the UART TX hardware peripheral.
    - It ensures safe, non-concurrent use of the UART peripheral, allowing both the Manager and Sensor Tasks to request print
operations without conflict via the Print Queue.

Please refer to - 

--- 

## Hardware Architecture
<img width="1269" height="653" alt="image" src="https://github.com/user-attachments/assets/00881f5f-53cf-49d0-bfd8-21c1411d2b61" />

## Overview
This project runs on the STM32F4-Discovery board, utilizing its built-in peripherals:
- Microcontroller: STM32F407VGT6.
- Sensor: Onboard LIS3DSH 3-axis accelerometer.
- User I/O: Built-in User Button and four onboard LEDs (Red, Green, Orange, Blue) for status and alignment indication.

### Debug UART Connection
Since the onboard ST-LINK does not route the application UART by default, external jumper wires are required for communication 
using USART2 on the STM32F407 (PA2/PA3) to the Virtual COM Port (VCP) pins on the ST-LINK debugger section.

| STM32F407 Pin (USART2) | Function | Connect to ST-LINK VCP Pin |
|---------|----------|----------|
| PA2  | UART TX (MCU Output)   | ST-LINK RX (Pin 13)   |
| PA3  | UART RX (MCU Input)   | ST-LINK TX (Pin 12)   |
| GND  | GND  | GND   |

--- 



## Key Files

| File | Location | Description |
|------|----------|-------------|
| rtos_system_state.c | Core/Src/ | Manages overall system state and state-machine logic |
| rtos_tasks.c | Core/Src/ | FreeRTOS task implementations (sensor read, manager, print) |
| rtos_logger.c | Core/Src/ | Simple logging utility for events and errors |
| lis3dsh_io.c | Drivers/LIS3DSH/Source/ | SPI I/O and register access for the LIS3DSH accelerometer |

## Screenshots & Photos
<img width="484" height="457" alt="image" src="https://github.com/user-attachments/assets/97c22128-2d61-4bc0-a470-1d6ed92439f4" />

<img width="183" height="521" alt="image" src="https://github.com/user-attachments/assets/783b6043-e852-445c-810e-36fe02f93146" />


<img width="1430" height="894" alt="image" src="https://github.com/user-attachments/assets/1e159e62-bf58-44e4-bf8b-0c64c464f70a" />

![WhatsApp Image 2025-11-26 at 12 54 08_5bc50948](https://github.com/user-attachments/assets/6de00f98-e678-4e22-81d2-9b51c08e8faf)
<img width="1297" height="747" alt="image" src="https://github.com/user-attachments/assets/a6fa9873-ea7e-444b-a3ae-afc16a179405" />

























