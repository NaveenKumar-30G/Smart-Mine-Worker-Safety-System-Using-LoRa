# Software and Development

## Arduino IDE

Arduino IDE is used as the primary development environment for programming and testing the ESP32 and ESP8266 microcontrollers used in the system.

The firmware for the chest unit, helmet unit and receiver unit is developed using Arduino-compatible C/C++ code and uploaded to the respective microcontrollers through the Arduino IDE.

The Arduino IDE is also used for:

- Writing and modifying firmware
- Compiling the source code
- Uploading programs to the microcontrollers
- Monitoring sensor readings through the Serial Monitor
- Testing wireless communication
- Debugging hardware and firmware issues

## Firmware

The project contains separate firmware programs for the three main units:

```text
firmware/
├── helmet/
│   └── helmet.ino
├── chest/
│   └── chest.ino
└── receiver/
    └── receiver.ino
The chest and helmet units use ESP32 microcontrollers, while the receiver unit uses an ESP8266 microcontroller.

2. Chest Unit Firmware

The chest unit firmware is developed for the ESP32 microcontroller.

The software is responsible for collecting the worker's health and motion information from the connected sensors.

The main operations are:

Initialize the ESP32 and connected sensors.
Read heart rate and SpO2 using the MAX30102 sensor.
Measure body temperature using the DS18B20 sensor.
Read motion information from the MPU6050 sensor.
Process the motion information for fall detection.
Process the collected sensor readings.
Transfer the processed information to the helmet unit using ESP-NOW.

The chest unit focuses mainly on health and motion monitoring.

3. Helmet Unit Firmware

The helmet unit firmware is developed for the ESP32 microcontroller.

The helmet unit monitors environmental conditions and also receives information from the chest unit.

The main operations are:

Initialize the ESP32 and connected sensors.
Read gas concentration using the MQ-2 gas sensor.
Detect flame conditions using the flame sensor.
Measure temperature using the DS18B20 sensor.
Receive health and motion information from the chest unit using ESP-NOW.
Process the received and locally measured data.
Check the system parameters for abnormal conditions.
Activate the buzzer, RGB LED and vibration motor when an alert condition is detected.
Prepare the monitoring data for transmission.
Transmit the collected information to the receiver using the SX1278 LoRa module.

The helmet unit therefore acts as the main field unit that combines environmental information with the worker's health and motion information.

4. Receiver Unit Firmware

The receiver unit firmware is developed for the ESP8266 microcontroller.

The receiver acts as the monitoring station for information transmitted from the helmet unit.

The main operations are:

Initialize the ESP8266 and LoRa receiver.
Receive monitoring data from the helmet unit through the SX1278 LoRa module.
Read the local vibration sensor.
Monitor the panic button.
Process the received monitoring information.
Check for abnormal conditions and emergency inputs.
Activate the buzzer and RGB LED when an alert condition is detected.
Send the received information through the serial interface for monitoring and logging.
5. ESP-NOW Communication

ESP-NOW is used for wireless communication between the chest unit and the helmet unit.

The chest unit collects the worker's health and motion information and transfers the processed information to the helmet unit.

The information transferred through ESP-NOW includes:

Heart rate
SpO2
Body temperature
Fall status

This communication allows the chest unit to transfer the worker monitoring information to the helmet unit without requiring a conventional Wi-Fi network.

6. LoRa Communication

LoRa communication is used between the helmet unit and the receiver unit.

The system uses the SX1278 LoRa module for long-range wireless data transmission.

The helmet unit combines the environmental information with the information received from the chest unit and transmits the monitoring data to the receiver.

The transmitted information includes:

Gas level
Flame status
Ambient temperature
Body temperature
Heart rate
SpO2
Fall status
Panic status

The receiver processes the received packet and checks the information for abnormal conditions.

7. Sensor Data Processing

The firmware continuously reads the connected sensors and processes the obtained values.

The chest unit processes:

Heart rate
SpO2
Body temperature
Motion and fall information

The helmet unit processes:

Gas level
Flame detection
Ambient temperature
Panic input

The processed information is combined with the communication data before being transmitted to the receiver.

8. Alert and Emergency Processing

The system provides local alert indications when an abnormal or emergency condition is detected.

The firmware checks conditions such as:

High gas concentration
Flame detection
Abnormal temperature
Fall detection
Panic button activation
Vibration detection at the receiver

When an alert condition is detected, the corresponding warning devices can be activated.

The warning devices used in the system include:

Buzzer
RGB LED
Vibration motor

This provides both audible and physical indications to improve awareness of emergency conditions.

9. Serial Monitoring

The Serial Monitor provided by Arduino IDE is used during development and testing.

Serial communication is useful for observing:

Sensor readings
System initialization
ESP-NOW communication status
LoRa transmission status
LoRa reception status
Alert conditions
Received monitoring packets

Serial output also assists in checking the behaviour of the system during hardware testing and firmware development.

10. Monitoring Dashboard

A Python-based monitoring dashboard is included in the project.

The dashboard is developed using Streamlit and uses serial communication to receive information from the receiver unit.

The dashboard is intended to provide a simple interface for monitoring the system information received from the field units.

The main functions include:

Real-time monitoring
Display of sensor information
Alert indication
System status monitoring
Data logging
Historical observation of recorded information

The dashboard files are organized as follows:

dashboard/
├── dashboard.py
└── requirements.txt
11. Python Software

Python is used for developing the monitoring dashboard.

The dashboard uses the following software components:

Streamlit

Streamlit is used to create the monitoring dashboard interface and display the received system information in a user-friendly format.

PySerial

PySerial is used for serial communication between the computer and the receiver unit.

The receiver sends the received monitoring information through the serial interface, which can then be read and processed by the dashboard.

12. Software Components
Software / Technology	Purpose
Arduino IDE	Firmware development, compilation and uploading
C/C++	ESP32 and ESP8266 firmware development
ESP-NOW	Wireless communication between chest and helmet units
LoRa	Long-range communication between helmet and receiver
Python	Monitoring dashboard development
Streamlit	Dashboard interface
PySerial	Serial communication between receiver and computer
13. Development Workflow

The software development process follows the sequence below:

Sensor and module initialization
Sensor data acquisition
Data processing
Abnormal-condition checking
ESP-NOW communication between chest and helmet units
Data aggregation at the helmet unit
LoRa transmission from helmet to receiver
Data reception and processing at the receiver
Serial transmission to the computer
Dashboard-based monitoring and data logging

The software architecture is designed to support continuous monitoring of the worker's health, motion and surrounding environmental conditions.
