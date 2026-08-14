# System Overview

## Smart Mine Worker Safety and Monitoring System Using LoRa for Real-Time Application

The Smart Mine Worker Safety and Monitoring System is designed to provide continuous monitoring of mine workers by combining health monitoring, environmental sensing, wireless communication and local alert mechanisms.

The system is divided into three main units:

- Chest Unit
- Helmet Unit
- Receiver Unit

The chest unit is responsible for monitoring worker health and motion-related parameters. The helmet unit monitors environmental conditions around the worker and provides local alerts. The receiver unit acts as the monitoring station and receives information through LoRa for further monitoring and alert indication.

## System Architecture

The system uses two wireless communication methods for different purposes.

### ESP-NOW Communication

The chest unit uses ESP-NOW to transfer the collected health and motion data to the helmet unit.

The chest unit monitors:

- Heart rate
- SpO2
- Body temperature
- Fall detection

The received information is combined with the environmental information collected by the helmet unit.

### LoRa Communication

The helmet unit uses an SX1278 LoRa module to transmit the combined monitoring information to the receiver unit.

LoRa provides a long-range wireless communication link between the worker-side unit and the monitoring station.

## Chest Unit

The chest unit is based on an ESP32 microcontroller.

The unit includes:

- MAX30102 heart rate and SpO2 sensor
- DS18B20 body temperature sensor
- MPU6050 motion sensor
- ESP-NOW communication

The collected data is processed by the ESP32 and transmitted to the helmet unit using ESP-NOW.

## Helmet Unit

The helmet unit is based on an ESP32 microcontroller and acts as the main worker-side environmental monitoring unit.

The helmet monitors:

- Gas level using MQ-2
- Flame condition using a flame sensor
- Temperature using DS18B20
- Panic input using a push button

The helmet also provides local warning mechanisms using:

- Buzzer
- RGB LED
- Vibration motor

The helmet receives health information from the chest unit through ESP-NOW and combines it with its own sensor readings.

The resulting monitoring data is transmitted to the receiver through the SX1278 LoRa module.

## Receiver Unit

The receiver unit acts as the monitoring station.

It uses an ESP8266 microcontroller with an SX1278 LoRa module to receive data from the helmet unit.

The receiver unit includes:

- LoRa communication
- Vibration sensor
- Panic button
- Buzzer
- RGB LED

The received data is analysed for abnormal conditions. When an alert condition is detected, the receiver provides a local warning through the buzzer and LED indicators.

## Alert Conditions

The system is designed to respond to abnormal conditions such as:

- Excessive gas level
- Flame detection
- High temperature
- Fall detection
- Worker panic activation
- Receiver-side panic activation
- Vibration detection at the monitoring station

When an abnormal condition is detected, the corresponding unit activates its available warning mechanisms.

## Data Flow

The overall data flow can be summarized as:

1. Sensors collect worker health, motion and environmental information.
2. The chest ESP32 processes health and motion data.
3. Chest data is transferred to the helmet using ESP-NOW.
4. The helmet ESP32 processes environmental information.
5. Helmet and chest information are combined into a monitoring packet.
6. The SX1278 LoRa module transmits the packet to the receiver.
7. The ESP8266 receiver processes the received information.
8. Alert conditions are identified.
9. Local warning indicators are activated when required.
10. The received information can be passed to the monitoring dashboard for visualization and logging.

## Communication Overview

```text
Chest Unit
   |
   | ESP-NOW
   v
Helmet Unit
   |
   | LoRa
   v
Receiver Unit
   |
   | Serial / USB
   v
Monitoring Dashboard
