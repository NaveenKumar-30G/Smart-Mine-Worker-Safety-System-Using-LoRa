# Smart Mine Worker Safety and Monitoring System Using LoRa

## Overview

The Smart Mine Worker Safety and Monitoring System is designed to support continuous monitoring of mine workers by combining health monitoring, environmental sensing, wireless communication and emergency alert mechanisms.

The system is organized into three main units: a chest unit, a helmet unit and a receiver unit. The chest unit monitors worker health and motion parameters, while the helmet unit monitors environmental conditions and combines the received worker information with its own sensor readings. The processed information is then transmitted to a monitoring station through LoRa communication.

The system also includes a computer-based monitoring dashboard for observing the received information and maintaining records.

## Key Features

- Heart rate and SpO2 monitoring
- Body temperature monitoring
- Fall detection using motion sensing
- Gas detection using the MQ-2 sensor
- Flame detection
- Ambient temperature monitoring
- Panic button for emergency activation
- Local buzzer, RGB LED and vibration alerts
- ESP-NOW communication between chest and helmet units
- LoRa-based communication between helmet and receiver
- ESP8266-based monitoring receiver
- Computer-based monitoring dashboard
- Serial data monitoring and logging

## System Architecture

The system consists of three primary hardware units.

```text
                    SMART MINE WORKER SAFETY SYSTEM

        ┌─────────────────┐
        │   CHEST UNIT    │
        │     ESP32       │
        │                 │
        │ MAX30102        │
        │ MPU6050         │
        │ DS18B20         │
        │ Panic Button    │
        └────────┬────────┘
                 │
              ESP-NOW
                 │
                 ▼
        ┌─────────────────┐
        │   HELMET UNIT   │
        │     ESP32       │
        │                 │
        │ MQ-2            │
        │ Flame Sensor    │
        │ DS18B20         │
        │ Buzzer          │
        │ RGB LED         │
        │ Vibration Motor │
        │ Panic Button    │
        │ SX1278 LoRa     │
        └────────┬────────┘
                 │
                LoRa
                 │
                 ▼
        ┌─────────────────┐
        │  RECEIVER UNIT  │
        │    ESP8266      │
        │                 │
        │ SX1278 LoRa     │
        │ Vibration Sensor│
        │ Buzzer          │
        │ RGB LED         │
        │ Panic Button    │
        └────────┬────────┘
                 │
              Serial / USB
                 │
                 ▼
        ┌─────────────────┐
        │    DASHBOARD    │
        │   PC / Laptop   │
        │                 │
        │ Real-time Data  │
        │ Alerts          │
        │ Data Logging    │
        └─────────────────┘
A detailed architecture diagram is available in diagrams/architecture_color.png.

System Units
1. Chest Unit — Health Monitoring

The chest unit is based on an ESP32 microcontroller and is responsible for monitoring worker health and motion parameters.

The unit includes:

MAX30102 — Heart rate and SpO2
DS18B20 — Body temperature
MPU6050 — Motion and fall detection
Panic button
ESP-NOW communication

The collected information is processed and transferred to the helmet unit using ESP-NOW.

2. Helmet Unit — Environmental Monitoring

The helmet unit is based on an ESP32 microcontroller and monitors environmental conditions around the worker.

The unit includes:

MQ-2 gas sensor
Flame sensor
DS18B20 temperature sensor
Buzzer
RGB LED
Vibration motor
Panic button
SX1278 LoRa module

The helmet unit receives health and motion information from the chest unit, combines it with the environmental readings and transmits the monitoring data to the receiver through LoRa.

3. Receiver Unit — Monitoring and Alert

The receiver unit is based on an ESP8266 microcontroller.

It receives information from the helmet unit through the SX1278 LoRa module and processes the received monitoring data.

The receiver unit includes:

ESP8266
SX1278 LoRa module
Vibration sensor
Buzzer
RGB LED
Panic button

The received information can also be forwarded through the serial interface for computer-based monitoring.

Communication
ESP-NOW

ESP-NOW is used for communication between the chest unit and helmet unit.

The chest unit transfers the following information:

Heart rate
SpO2
Body temperature
Fall status
LoRa

LoRa is used for communication between the helmet unit and receiver unit.

The helmet unit transmits combined monitoring information including:

Gas level
Flame status
Ambient temperature
Body temperature
Heart rate
SpO2
Fall status
Panic status
Hardware

The major hardware components used in the system include:

Component	Application
ESP32	Chest and helmet processing
ESP8266	Receiver and monitoring station
MAX30102	Heart rate and SpO2
MPU6050	Motion and fall detection
DS18B20	Temperature measurement
MQ-2	Gas detection
Flame Sensor	Flame detection
SX1278	LoRa communication
Vibration Sensor	Receiver-side vibration detection
Buzzer	Audible alert
RGB LED	Visual indication
Vibration Motor	Physical alert
Panic Button	Emergency activation
18650 Battery	Portable power supply

Detailed hardware information is available in docs/hardware.md.

Software

The system software is developed using Arduino-compatible C/C++ firmware and Python-based dashboard software.

Development Tools
Arduino IDE
C/C++
Python
Streamlit
PySerial

Arduino IDE is used for programming, compiling, uploading and testing the ESP32 and ESP8266 firmware.

The software documentation is available in docs/software.md.

Firmware

The firmware is separated according to the three system units.

firmware/
├── chest/
│   └── chest.ino
├── helmet/
│   └── helmet.ino
└── receiver/
    └── receiver.ino
Monitoring Dashboard

A Python-based monitoring dashboard is included for observing the information received from the receiver unit.

The dashboard is developed using Streamlit and communicates with the receiver through the computer's serial interface.

The dashboard provides functions for:

Real-time monitoring
Sensor information display
Alert indication
System status monitoring
Data logging
Historical observation

Dashboard files:

dashboard/
├── dashboard.py
└── requirements.txt
System Operation

The overall operation of the system follows these stages:

The chest unit initializes the health and motion sensors.
Worker health and motion parameters are collected.
The chest unit processes the collected information.
Health and motion data are transferred to the helmet unit through ESP-NOW.
The helmet unit collects environmental information.
The helmet unit combines the received worker information with its environmental readings.
Abnormal conditions are checked locally.
Local warning devices are activated when required.
The combined monitoring information is transmitted through LoRa.
The receiver unit receives and processes the LoRa packet.
Emergency conditions are indicated through the receiver's alert devices.
The received information can be forwarded to the computer through the serial interface.
The monitoring dashboard displays and records the received information.
Alerts and Emergency Conditions

The system is designed to identify conditions such as:

Excessive gas concentration
Flame detection
Abnormal temperature
Fall detection
Panic button activation
Receiver-side vibration detection

Depending on the detected condition, the system can provide audible, visual and vibration-based alerts.

System Documentation

Additional technical information is available in the following sections:

docs/system-overview.md — System overview
docs/hardware.md — Hardware components
docs/software.md — Software and development
diagrams/README.md — System diagrams
firmware/README.md — Firmware structure
System Diagrams

The repository contains the main diagrams used to describe the system architecture and operation.

Available diagrams include:

System architecture
Chest unit block diagram
Helmet unit block diagram
Receiver unit block diagram
Communication flow
Complete system flowchart

The diagrams are available in the diagrams/ directory.

Project Photographs

Photographs of the developed hardware units are available in the images/ directory.

The collection includes photographs of:

Chest unit
Helmet unit
Helmet front and bottom views
Receiver unit
Receiver hardware
Repository Structure
Smart-Mine-Worker-Safety-System-Using-LoRa/
│
├── dashboard/
│   ├── dashboard.py
│   └── requirements.txt
│
├── diagrams/
│   ├── architecture_color.png
│   ├── chest_block_diagram.png
│   ├── communication_flow.png
│   ├── flowchart_mine.png
│   ├── helmet_block_diagram.png
│   ├── receiver_block_diagram.png
│   └── README.md
│
├── docs/
│   ├── hardware.md
│   ├── software.md
│   └── system-overview.md
│
├── firmware/
│   ├── chest/
│   │   └── chest.ino
│   ├── helmet/
│   │   └── helmet.ino
│   ├── receiver/
│   │   └── receiver.ino
│   └── README.md
│
├── images/
│   ├── chest_device.jpeg
│   ├── chest_unit.jpeg
│   ├── helmet_Bottom_view.jpeg
│   ├── helmet_Front_view.jpeg
│   ├── receiver_unit.jpeg
│   ├── receiver_unit_2.jpeg
│   └── project-photos.md
│
└── README.md
Project Status

The repository contains the firmware, system documentation, diagrams, hardware photographs and monitoring dashboard associated with the Smart Mine Worker Safety and Monitoring System.

Future Scope

Possible future development areas include:

Improved sensor calibration
More robust wireless communication
Extended monitoring range
Improved dashboard functionality
Cloud-based data storage
Additional environmental sensors
Improved power management
Enhanced fall-detection algorithms
Centralized monitoring for multiple workers

Author
Naveen Kumar G
B.Tech — Electronics and Communication Engineering
