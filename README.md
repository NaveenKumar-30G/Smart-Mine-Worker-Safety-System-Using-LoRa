# Smart Mine Worker Safety and Monitoring System Using LoRa

An IoT-based wearable safety and monitoring system designed to monitor
mine workers' health conditions and surrounding environmental conditions
using ESP32, ESP-NOW and LoRa communication.

------------------------------------------------------------------------

## Overview

Mining environments expose workers to several risks, including harmful
gas leakage, fire, high temperature, falls and health-related
emergencies. A major difficulty in underground environments is that
these conditions can develop quickly while communication with workers
may be limited.

This project combines environmental monitoring and worker health
monitoring into a single wearable system.

The system is divided into three main units:

-   Chest Unit
-   Helmet Unit
-   Receiver Unit

The chest unit monitors the worker's physical condition. The helmet unit
monitors the surrounding environment and also acts as the main
communication unit. The receiver collects the transmitted information
and provides a centralized monitoring interface.

ESP-NOW is used for communication between the chest and helmet units,
while LoRa is used for long-range communication between the helmet and
receiver.

------------------------------------------------------------------------

## Project Objectives

-   Monitor worker health parameters continuously.
-   Detect harmful gas conditions around the worker.
-   Detect flame or fire conditions.
-   Monitor environmental temperature.
-   Detect sudden movement and possible falls.
-   Provide local warning through buzzer, LED and vibration.
-   Provide a panic alert mechanism for emergency situations.
-   Transfer health information from the chest unit to the helmet using
    ESP-NOW.
-   Transfer combined worker and environmental data from the helmet to
    the receiver using LoRa.
-   Display received information through a real-time monitoring
    dashboard.

------------------------------------------------------------------------

## System Architecture

The overall communication path is:

    Chest Unit
        |
        | ESP-NOW
        v
    Helmet Unit
        |
        | LoRa 433 MHz
        v
    Receiver Unit
        |
        | Serial Communication
        v
    Monitoring Dashboard

The chest unit collects worker health information and sends it to the
helmet unit. The helmet combines this information with its environmental
sensor readings. The combined packet is transmitted through LoRa to the
receiver.

The receiver processes the packet, activates local warning devices when
required and sends the received information to the monitoring dashboard.

------------------------------------------------------------------------

## Hardware Architecture

### 1. Chest Unit

The chest unit is responsible for monitoring the worker's physical
condition.

  Component   Purpose
  ----------- --------------------------------
  ESP32       Main controller
  MAX30102    Heart rate and SpO2 monitoring
  MPU6050     Motion and fall detection
  DS18B20     Body temperature monitoring

The collected health information is transmitted to the helmet unit using
ESP-NOW.

### 2. Helmet Unit

The helmet unit monitors the environment around the worker and combines
the environmental information with the health information received from
the chest unit.

  Component            Purpose
  -------------------- ---------------------------
  ESP32                Main controller
  MQ-2                 Gas and smoke detection
  Flame Sensor         Fire/flame detection
  DS18B20              Environmental temperature
  SX1278 LoRa          Long-range communication
  Buzzer               Audible warning
  RGB/LED indicators   Visual warning
  Vibration Motor      Physical warning
  Panic Button         Manual emergency alert

The helmet controller checks the monitored parameters and generates
local warnings when an unsafe condition is detected.

### 3. Receiver Unit

The receiver unit acts as the monitoring station for the system.

  Component      Purpose
  -------------- ---------------------------
  ESP8266        Receiver controller
  SX1278 LoRa    Receives data from helmet
  SW-420         Vibration detection
  Panic Button   Emergency input
  Buzzer         Audible warning
  RGB LED        Status indication

The receiver obtains the combined data packet from the helmet through
LoRa and forwards the information to the monitoring dashboard through
serial communication.

------------------------------------------------------------------------

## Communication

### Chest to Helmet

ESP-NOW is used between the chest unit and helmet unit.

The chest unit sends:

-   Body temperature
-   Heart rate
-   SpO2
-   Fall detection status

### Helmet to Receiver

LoRa communication is used between the helmet and receiver.

The helmet combines the chest data with its own environmental readings
and sends the resulting packet through the SX1278 LoRa module.

The LoRa configuration used in the prototype includes:

-   Frequency: 433 MHz
-   Spreading Factor: 7
-   Bandwidth: 125 kHz
-   Coding Rate: 4/5
-   Sync Word: 0x12

------------------------------------------------------------------------

## Data Flow

1.  The chest unit initializes the health monitoring sensors.
2.  Worker health parameters are collected.
3.  The chest unit sends health data to the helmet using ESP-NOW.
4.  The helmet reads gas, flame and environmental temperature.
5.  The helmet combines health and environmental information.
6.  The controller checks the monitored parameters.
7.  Local warning devices are activated when an unsafe condition is
    detected.
8.  The combined data is transmitted through LoRa.
9.  The receiver receives and processes the LoRa packet.
10. RSSI information is obtained from the received LoRa packet.
11. The receiver activates warning indicators when required.
12. The received information is sent to the monitoring dashboard.

------------------------------------------------------------------------

## Monitoring Parameters

  ------------------------------------------------------------------------
  Parameter              Sensor/Source                  Purpose
  ---------------------- ------------------------------ ------------------
  Gas Level              MQ-2                           Detect gas/smoke
                                                        conditions

  Flame                  Flame Sensor                   Detect fire

  Environmental          DS18B20                        Monitor
  Temperature                                           surrounding
                                                        temperature

  Body Temperature       DS18B20                        Monitor worker
                                                        temperature

  Heart Rate             MAX30102                       Monitor worker
                                                        heart rate

  SpO2                   MAX30102                       Monitor blood
                                                        oxygen level

  Fall                   MPU6050                        Detect sudden
                                                        movement/fall

  Vibration              SW-420                         Detect
                                                        receiver-side
                                                        vibration

  Panic                  Panic Button                   Manual emergency
                                                        alert

  RSSI                   LoRa                           Monitor received
                                                        signal strength
  ------------------------------------------------------------------------

------------------------------------------------------------------------

## Alert Conditions

The prototype uses threshold-based monitoring.

The main conditions considered by the system include:

-   High gas level
-   High temperature
-   Flame detection
-   Abnormal heart rate
-   Low SpO2
-   Fall detection
-   Panic button activation
-   Receiver-side vibration

When an unsafe condition is detected, the system can provide warning
through the buzzer, LED indicators and vibration motor.

The monitoring dashboard also represents normal and emergency states
using visual status indicators.

------------------------------------------------------------------------

## Data Packet

The helmet sends the combined information to the receiver in a compact
text packet.

Example format:

    ID:01,GAS:768,FLAME:SAFE,TEMP:33.56,BODYTEMP:36.5,HR:78,SPO2:98,FALL:NO,PANIC:NO

The receiver adds local information such as vibration, button state,
alert status and LoRa RSSI before sending the information to the
dashboard.

------------------------------------------------------------------------

## Monitoring Dashboard

A Streamlit-based Python dashboard is used for monitoring the received
information.

The dashboard provides sections for:

### Worker Health

-   Heart rate
-   SpO2
-   Body temperature
-   Fall status

### Environmental Conditions

-   Gas level
-   Flame status
-   Environmental temperature

### Emergency Information

-   Panic status
-   Receiver vibration
-   Receiver button status
-   Overall alert status

### Communication

-   LoRa RSSI
-   Signal condition

The dashboard communicates with the receiver through a serial
connection.

------------------------------------------------------------------------

## Software

### Firmware

-   Arduino IDE
-   C/C++
-   ESP32 Arduino framework
-   ESP8266 Arduino framework

### Communication

-   ESP-NOW
-   LoRa

### Dashboard

-   Python
-   Streamlit
-   PySerial

------------------------------------------------------------------------

## Repository Structure

    Smart-Mine-Worker-Safety-System-Using-LoRa/
    |
    +-- firmware/
    |   +-- helmet/
    |   |   +-- helmet.ino
    |   |
    |   +-- chest/
    |   |   +-- chest.ino
    |   |
    |   +-- receiver/
    |       +-- receiver.ino
    |
    +-- dashboard/
    |   +-- dashboard.py
    |   +-- requirements.txt
    |
    +-- diagrams/
    |   +-- system_architecture.png
    |   +-- helmet_block_diagram.png
    |   +-- chest_block_diagram.png
    |   +-- system_flowchart.png
    |
    +-- images/
    |   +-- helmet_inside.jpg
    |   +-- helmet_outside.jpg
    |   +-- chest_unit.jpg
    |   +-- receiver_unit.jpg
    |
    +-- documentation/
    |   +-- project_overview.md
    |
    +-- README.md

------------------------------------------------------------------------

## Installation

### Arduino Firmware

Install the required board packages and libraries through the Arduino
IDE.

The firmware uses libraries/modules for:

-   LoRa
-   ESP-NOW
-   OneWire
-   DallasTemperature
-   MAX3010x sensor
-   MPU6050

The exact library requirements are available in the corresponding
firmware source files.

### Dashboard Setup

Install the required Python packages:

    pip install -r requirements.txt

Run the dashboard using:

    streamlit run dashboard.py

Connect the receiver to the computer through USB and select the
corresponding serial port from the dashboard.

------------------------------------------------------------------------

## Prototype Testing

The project was developed as an academic prototype and the individual
sections were tested during implementation.

Testing covered:

-   MQ-2 gas/smoke response
-   Flame detection
-   Temperature monitoring
-   MAX30102 heart rate and SpO2 monitoring
-   MPU6050 movement/fall detection
-   ESP-NOW communication
-   LoRa communication
-   Local alert generation
-   Receiver-side panic communication
-   Dashboard data reception

The project documentation reports successful operation of the main
sensing, communication and alert functions during the prototype testing
stage.

------------------------------------------------------------------------

## Observed Limitations

### MQ-2 Warm-up

The MQ-2 sensor requires a warm-up period before its readings become
stable.

### LoRa Signal Reduction

LoRa signal strength can decrease when the signal passes through thick
walls or heavy obstacles.

### Motion Effects on MAX30102

Strong worker movement can affect the stability of heart rate
measurements.

### Single Worker Monitoring

The current implementation is primarily designed around a single worker.
Supporting a larger number of workers would require additional software
and communication handling.

### Battery Dependency

Continuous operation depends on adequate battery charging and
maintenance.

------------------------------------------------------------------------

## Future Improvements

-   GPS-based worker location tracking
-   Multi-worker monitoring
-   Cloud-based data storage
-   Mobile application support
-   AI-based danger prediction
-   Solar charging
-   Additional environmental sensors
-   Improved mine-specific gas sensing and calibration
-   More robust fall-detection algorithms
-   Larger-scale LoRa network support

------------------------------------------------------------------------

## Applications

The system concept can be applied to:

-   Underground mining
-   Coal and metal mining
-   Tunnel construction
-   Industrial safety monitoring
-   Fire-risk environments
-   Worker health monitoring
-   Hazardous gas monitoring

------------------------------------------------------------------------

## Project Highlights

-   Wearable worker safety monitoring
-   Simultaneous health and environmental monitoring
-   ESP-NOW communication between wearable units
-   Long-range LoRa communication
-   Local emergency alerts
-   Two-way panic communication
-   Centralized monitoring dashboard
-   Low-cost embedded hardware

------------------------------------------------------------------------

## Sustainable Development Goal

The project is aligned with the safety and well-being objectives of the
United Nations Sustainable Development Goals, particularly:

-   SDG 3 -- Good Health and Well-Being
-   SDG 8 -- Decent Work and Economic Growth

The system focuses on improving safety conditions for workers operating
in hazardous environments through continuous monitoring and emergency
alert mechanisms.

------------------------------------------------------------------------

## Project Status

**Academic Prototype / Major Project**

The repository contains the firmware, dashboard software, diagrams and
supporting documentation used for the project prototype.

Hardware-specific configuration, sensor calibration and communication
settings may need to be adjusted when reproducing the system on
different devices.

------------------------------------------------------------------------

## License

This project is intended for academic and educational purposes.

If you plan to reuse or modify the project, please provide appropriate
credit to the original project team.
