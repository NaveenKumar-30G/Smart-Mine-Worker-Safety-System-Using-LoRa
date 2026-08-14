#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <esp_now.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// =====================================================
// SMART MINE WORKER SAFETY AND MONITORING SYSTEM
// Helmet Unit - ESP32 + SX1278 LoRa
// =====================================================

// ---------------- LoRa Pins ----------------
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  2

// ---------------- Sensors ----------------
#define GAS_PIN       34
#define FLAME_PIN     35
#define ONE_WIRE_BUS  4

// ---------------- Outputs ----------------
#define BUZZER_PIN    27
#define RED_LED       32
#define GREEN_LED     15
#define VIBRATION_PIN 26

// ---------------- Inputs ----------------
#define PANIC_BUTTON  33

// ---------------- Thresholds ----------------
#define GAS_THRESHOLD   3000
#define TEMP_THRESHOLD  40.0

// ---------------- LoRa Settings ----------------
#define LORA_FREQUENCY 433E6
#define LORA_SF        7
#define LORA_BANDWIDTH 125E3
#define LORA_CODING_RATE 5
#define LORA_SYNC_WORD 0x12
#define LORA_TX_POWER  17

// =====================================================
// TEMPERATURE SENSOR
// =====================================================

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensor(&oneWire);

// =====================================================
// CHEST DATA
// Data received from chest unit through ESP-NOW
// =====================================================

typedef struct {

  float temp;
  int heartRate;
  int spo2;
  bool fall;

} chest_data_t;

chest_data_t chestData;

// =====================================================
// PANIC TIMER
// =====================================================

unsigned long lastSend = 0;
unsigned long panicTimer = 0;

// =====================================================
// ESP-NOW RECEIVE CALLBACK
// =====================================================

void onChestDataReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len
) {

  if (len == sizeof(chestData)) {

    memcpy(
      &chestData,
      data,
      sizeof(chestData)
    );
  }
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ---------------- GPIO ----------------

  pinMode(
    BUZZER_PIN,
    OUTPUT
  );

  pinMode(
    RED_LED,
    OUTPUT
  );

  pinMode(
    GREEN_LED,
    OUTPUT
  );

  pinMode(
    PANIC_BUTTON,
    INPUT_PULLUP
  );

  pinMode(
    VIBRATION_PIN,
    OUTPUT
  );


  // ---------------- Default State ----------------

  // Active LOW buzzer
  digitalWrite(
    BUZZER_PIN,
    HIGH
  );

  digitalWrite(
    RED_LED,
    LOW
  );

  digitalWrite(
    GREEN_LED,
    HIGH
  );

  digitalWrite(
    VIBRATION_PIN,
    LOW
  );


  // ---------------- Temperature Sensor ----------------

  temperatureSensor.begin();


  // =================================================
  // ESP-NOW
  // =================================================

  WiFi.mode(WIFI_STA);

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

  esp_now_register_recv_cb(
    onChestDataReceive
  );


  // =================================================
  // LoRa
  // =================================================

  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );


  if (
    !LoRa.begin(LORA_FREQUENCY)
  ) {

    Serial.println(
      "LoRa initialization failed!"
    );

    while (1) {
      delay(1000);
    }
  }


  // ---------------- LoRa Configuration ----------------

  LoRa.setSpreadingFactor(
    LORA_SF
  );

  LoRa.setSignalBandwidth(
    LORA_BANDWIDTH
  );

  LoRa.setCodingRate4(
    LORA_CODING_RATE
  );

  LoRa.setSyncWord(
    LORA_SYNC_WORD
  );

  LoRa.setTxPower(
    LORA_TX_POWER
  );


  Serial.println();
  Serial.println(
    "======================================"
  );

  Serial.println(
    " SMART MINE SAFETY - HELMET UNIT"
  );

  Serial.println(
    " ESP32 + SX1278 LoRa"
  );

  Serial.println(
    "======================================"
  );

  Serial.println(
    "Helmet Ready"
  );
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  // ===================================================
  // RECEIVE PANIC SIGNAL FROM RECEIVER
  // ===================================================

  bool receiverPanic = false;

  unsigned long listenStart =
    millis();


  while (
    millis() - listenStart < 200
  ) {

    int packetSize =
      LoRa.parsePacket();


    if (packetSize) {

      String message = "";


      while (LoRa.available()) {

        message +=
          (char)LoRa.read();
      }


      if (
        message.indexOf(
          "PANIC:RX"
        ) != -1
      ) {

        receiverPanic = true;

        panicTimer =
          millis();

        Serial.println(
          "RECEIVER PANIC DETECTED"
        );
      }
    }
  }


  // ===================================================
  // LOCAL PANIC BUTTON
  // ===================================================

  bool localPanic =
    (
      digitalRead(
        PANIC_BUTTON
      ) == LOW
    );


  // ===================================================
  // FINAL PANIC STATUS
  // ===================================================

  bool panic =
    localPanic ||
    (
      millis() - panicTimer < 5000
    );


  // ===================================================
  // VIBRATION ALERT
  // ===================================================

  digitalWrite(
    VIBRATION_PIN,
    panic ? HIGH : LOW
  );


  // ===================================================
  // READ ENVIRONMENTAL SENSORS
  // ===================================================

  int gasValue =
    analogRead(
      GAS_PIN
    );


  int flameValue =
    analogRead(
      FLAME_PIN
    );


  temperatureSensor.requestTemperatures();


  float temperature =
    temperatureSensor.getTempCByIndex(0);


  // ===================================================
  // SENSOR ALERT CONDITIONS
  // ===================================================

  bool flameAlert =
    (
      flameValue < 2000
    );


  bool gasAlert =
    (
      gasValue > GAS_THRESHOLD
    );


  bool temperatureAlert =
    (
      temperature > TEMP_THRESHOLD &&
      temperature != -127
    );


  bool fallAlert =
    chestData.fall;


  bool systemAlert =
    gasAlert ||
    flameAlert ||
    temperatureAlert ||
    fallAlert ||
    panic;


  // ===================================================
  // LOCAL ALERT OUTPUTS
  // ===================================================

  // Active LOW buzzer
  digitalWrite(
    BUZZER_PIN,
    systemAlert ? LOW : HIGH
  );


  digitalWrite(
    RED_LED,
    systemAlert ? HIGH : LOW
  );


  digitalWrite(
    GREEN_LED,
    systemAlert ? LOW : HIGH
  );


  // ===================================================
  // SEND DATA TO RECEIVER
  // ===================================================

  if (
    millis() - lastSend >= 3000
  ) {

    lastSend =
      millis();


    String packet =
      "ID:01";


    packet +=
      ",GAS:" +
      String(gasValue);


    packet +=
      ",FLAME:";


    packet +=
      (
        flameAlert
        ? "FIRE"
        : "SAFE"
      );


    packet +=
      ",TEMP:";


    packet +=
      String(temperature);


    packet +=
      ",BODYTEMP:";


    packet +=
      String(
        chestData.temp
      );


    packet +=
      ",HR:";


    packet +=
      String(
        chestData.heartRate
      );


    packet +=
      ",SPO2:";


    packet +=
      String(
        chestData.spo2
      );


    packet +=
      ",FALL:";


    packet +=
      (
        fallAlert
        ? "YES"
        : "NO"
      );


    packet +=
      ",PANIC:";


    packet +=
      (
        panic
        ? "YES"
        : "NO"
      );


    // ---------------- LoRa Transmission ----------------

    LoRa.beginPacket();

    LoRa.print(
      packet
    );

    LoRa.endPacket();


    // ---------------- Serial Output ----------------

    Serial.println(
      "LoRa Data Sent:"
    );

    Serial.println(
      packet
    );
  }
}
