# Hardware Description

## 1. ESP32 Microcontroller

ESP32 is used as the main controller for the chest unit and helmet unit.

In the chest unit, the ESP32 collects health and motion-related information from the connected sensors and transfers the processed data using ESP-NOW.

In the helmet unit, the ESP32 collects environmental information, receives data from the chest unit and prepares the combined information for transmission through LoRa.

## 2. ESP8266 Microcontroller

The ESP8266 is used in the receiver unit as the monitoring-side controller.

It receives the monitoring packet from the helmet unit through the LoRa communication module and processes the received information to identify alert conditions.

## 3. MAX30102

The MAX30102 sensor is used in the chest unit for monitoring heart rate and SpO2.

The sensor provides the health-related readings that are processed by the ESP32 before being transferred to the helmet unit.

## 4. MPU6050

The MPU6050 is used for motion monitoring and fall detection.

It contains an accelerometer and gyroscope and provides motion information to the chest-unit ESP32.

## 5. DS18B20

DS18B20 temperature sensors are used in the system for temperature monitoring.

One sensor is used in the chest unit for body temperature monitoring, while another is used in the helmet unit for environmental temperature monitoring.

## 6. MQ-2 Gas Sensor

The MQ-2 gas sensor is installed on the helmet unit.

It is used to monitor the surrounding gas level and identify conditions in which the measured gas level crosses the defined threshold.

## 7. Flame Sensor

The flame sensor is installed on the helmet unit to detect the presence of a flame or fire-related condition.

The sensor output is processed by the ESP32 and included in the monitoring data.

## 8. SX1278 LoRa Module

The SX1278 LoRa module provides the long-range wireless communication link between the helmet unit and the receiver unit.

The helmet unit uses the module to transmit monitoring information to the receiver station.

## 9. Buzzer

Buzzers are used as audible alert devices.

The helmet unit can activate its buzzer when an abnormal condition is detected, while the receiver unit also provides an audible warning for received or local alert conditions.

## 10. RGB LED

RGB LEDs provide visual indication of the system status.

The LEDs can indicate normal operation and alert conditions at the helmet and receiver units.

## 11. Vibration Motor

A vibration motor is included in the helmet unit to provide a physical warning to the worker.

It can be activated when a panic condition or abnormal monitoring condition is detected.

## 12. Panic Button

The panic button provides a manual emergency input.

The worker can use the button on the helmet unit to indicate an emergency condition. A panic input is also provided at the receiver side for monitoring-station operation.

## 13. Vibration Sensor

A vibration sensor is included in the receiver unit.

It provides an additional local input that can be used to identify a vibration-related alert condition.

## 14. 18650 Battery

Rechargeable 18650 batteries are used to provide portable power for the worker-side units.

The battery-powered arrangement allows the chest and helmet units to operate without being connected directly to a fixed power source.

## 15. Power Module and Voltage Monitoring

Power modules are used with the battery-powered units to provide the required power arrangement.

A voltage divider is used for battery-voltage monitoring where required.

## Hardware Summary

| Unit | Main Components |
|------|-----------------|
| Chest Unit | ESP32, MAX30102, MPU6050, DS18B20 |
| Helmet Unit | ESP32, MQ-2, Flame Sensor, DS18B20, SX1278, Buzzer, RGB LED, Vibration Motor, Panic Button |
| Receiver Unit | ESP8266, SX1278, Vibration Sensor, Buzzer, RGB LED, Panic Button |
| Power | 18650 Battery, Power Module, Voltage Divider |

## Communication Hardware

The system uses two different wireless communication methods:

- **ESP-NOW** for communication between the chest and helmet units.
- **LoRa using SX1278** for long-range communication between the helmet and receiver units.
