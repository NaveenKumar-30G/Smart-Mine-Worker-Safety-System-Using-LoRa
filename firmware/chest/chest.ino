#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <MPU6050.h>

// =====================================================
// SMART MINE WORKER SAFETY AND MONITORING SYSTEM
// Chest Unit - ESP32
// =====================================================

// ---------------- Sensor Objects ----------------

MAX30105 max30102;
MPU6050 mpu;


// ---------------- Temperature Sensor ----------------

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensor(&oneWire);


// =====================================================
// MAX30102 SETTINGS
// =====================================================

#define BUFFER_LENGTH 100

uint32_t irBuffer[BUFFER_LENGTH];
uint32_t redBuffer[BUFFER_LENGTH];

int32_t spo2;
int8_t validSpO2;

int32_t heartRate;
int8_t validHeartRate;


// =====================================================
// DATA STRUCTURE
// =====================================================

typedef struct {

  float temp;
  int heartRate;
  int spo2;
  bool fall;

} chest_data_t;

chest_data_t chestData;


// =====================================================
// ESP-NOW DESTINATION
// =====================================================

// IMPORTANT:
// Replace the placeholder with the MAC address
// of your HELMET ESP32 before using the hardware.
//
// Do NOT publish your real MAC address on GitHub.

uint8_t helmetAddress[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};


// =====================================================
// READ MAX30102
// =====================================================

void readMAX30102() {

  // Collect samples
  for (
    byte i = 0;
    i < BUFFER_LENGTH;
    i++
  ) {

    while (
      max30102.available() == false
    ) {

      max30102.check();
    }


    redBuffer[i] =
      max30102.getRed();

    irBuffer[i] =
      max30102.getIR();


    max30102.nextSample();
  }


  // Calculate heart rate and SpO2

  maxim_heart_rate_and_oxygen_saturation(

    irBuffer,
    BUFFER_LENGTH,

    redBuffer,

    &spo2,
    &validSpO2,

    &heartRate,
    &validHeartRate
  );


  // ---------------- Heart Rate ----------------

  if (
    validHeartRate &&
    heartRate >= 30 &&
    heartRate <= 220
  ) {

    chestData.heartRate =
      heartRate;

  } else {

    chestData.heartRate =
      0;
  }


  // ---------------- SpO2 ----------------

  if (
    validSpO2 &&
    spo2 >= 70 &&
    spo2 <= 100
  ) {

    chestData.spo2 =
      spo2;

  } else {

    chestData.spo2 =
      0;
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);


  // ===================================================
  // I2C
  // ===================================================

  Wire.begin();


  // ===================================================
  // ESP-NOW
  // ===================================================

  WiFi.mode(
    WIFI_STA
  );

  WiFi.disconnect();

  WiFi.setChannel(1);


  if (
    esp_now_init() != ESP_OK
  ) {

    Serial.println(
      "ESP-NOW initialization failed!"
    );

    while (1) {
      delay(1000);
    }
  }


  // ---------------- Add Helmet Peer ----------------

  esp_now_peer_info_t peerInfo = {};

  memcpy(
    peerInfo.peer_addr,
    helmetAddress,
    6
  );

  peerInfo.channel = 1;

  peerInfo.encrypt = false;

  peerInfo.ifidx =
    WIFI_IF_STA;


  if (
    esp_now_add_peer(
      &peerInfo
    ) != ESP_OK
  ) {

    Serial.println(
      "ESP-NOW peer configuration failed!"
    );

    while (1) {
      delay(1000);
    }
  }


  // ===================================================
  // MAX30102
  // ===================================================

  if (
    !max30102.begin(
      Wire,
      I2C_SPEED_FAST
    )
  ) {

    Serial.println(
      "MAX30102 not found!"
    );

    while (1) {
      delay(1000);
    }
  }


  Serial.println(
    "MAX30102 ready"
  );


  // ---------------- MAX30102 Configuration ----------------

  byte ledBrightness = 60;

  byte sampleAverage = 4;

  byte ledMode = 2;

  byte sampleRate = 100;

  int pulseWidth = 411;

  int adcRange = 4096;


  max30102.setup(

    ledBrightness,

    sampleAverage,

    ledMode,

    sampleRate,

    pulseWidth,

    adcRange
  );


  // ===================================================
  // DS18B20
  // ===================================================

  temperatureSensor.begin();


  // ===================================================
  // MPU6050
  // ===================================================

  mpu.initialize();


  if (
    mpu.testConnection()
  ) {

    Serial.println(
      "MPU6050 ready"
    );

  } else {

    Serial.println(
      "MPU6050 not found!"
    );
  }


  // ===================================================
  // READY
  // ===================================================

  Serial.println();
  Serial.println(
    "======================================"
  );

  Serial.println(
    " SMART MINE SAFETY - CHEST UNIT"
  );

  Serial.println(
    " ESP32 + MAX30102 + MPU6050"
  );

  Serial.println(
    "======================================"
  );

  Serial.println(
    "Chest Unit Ready"
  );
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ===================================================
  // BODY TEMPERATURE
  // ===================================================

  temperatureSensor.requestTemperatures();

  chestData.temp =
    temperatureSensor.getTempCByIndex(0);


  // ===================================================
  // HEART RATE + SpO2
  // ===================================================

  Serial.println(
    "Reading MAX30102..."
  );

  readMAX30102();


  // ===================================================
  // FALL DETECTION
  // ===================================================

  int16_t ax;
  int16_t ay;
  int16_t az;


  mpu.getAcceleration(
    &ax,
    &ay,
    &az
  );


  float magnitude =
    sqrt(

      (float)ax * ax +

      (float)ay * ay +

      (float)az * az
    );


  chestData.fall =
    (
      magnitude > 20000
    );


  // ===================================================
  // SEND DATA TO HELMET
  // ===================================================

  esp_err_t result =
    esp_now_send(

      helmetAddress,

      (uint8_t *)&chestData,

      sizeof(chestData)
    );


  if (
    result == ESP_OK
  ) {

    Serial.println(
      "ESP-NOW Send OK"
    );

  } else {

    Serial.println(
      "ESP-NOW Send Failed"
    );
  }


  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.println();
  Serial.println(
    "========== CHEST DATA =========="
  );


  Serial.print(
    "Body Temperature : "
  );

  Serial.print(
    chestData.temp
  );

  Serial.println(
    " °C"
  );


  Serial.print(
    "Heart Rate       : "
  );

  Serial.print(
    chestData.heartRate
  );

  Serial.println(
    " BPM"
  );


  Serial.print(
    "SpO2             : "
  );

  Serial.print(
    chestData.spo2
  );

  Serial.println(
    " %"
  );


  Serial.print(
    "Fall Detection   : "
  );

  Serial.println(

    chestData.fall
    ? "YES"
    : "NO"
  );


  Serial.println(
    "================================"
  );


  delay(1000);
}
