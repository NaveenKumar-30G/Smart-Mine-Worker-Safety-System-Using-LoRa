#include <SPI.h>
#include <LoRa.h>

// =====================================================
// SMART MINE WORKER SAFETY & MONITORING SYSTEM
// Receiver Unit - ESP8266 + SX1278 LoRa
// =====================================================

// ---------------- LORA PINS ----------------
// NodeMCU ESP8266
// D5 = GPIO14 -> SCK
// D6 = GPIO12 -> MISO
// D7 = GPIO13 -> MOSI
// D8 = GPIO15 -> NSS / SS

#define LORA_SS   D8

// DIO0 is not required because this program
// uses LoRa.parsePacket() polling.
#define LORA_DIO0 -1

// Radio reset is omitted from MCU-controlled pins.
// Connect SX1278 RESET to the ESP8266 reset line
// or use the module's appropriate reset configuration.
#define LORA_RST  -1

// ---------------- INPUTS ----------------
#define VIB_PIN       D2
#define BUTTON_PIN    D1

// ---------------- OUTPUTS ----------------
#define BUZZER_PIN    D4

// ---------------- RGB LED ----------------
#define RED_PIN       D0
#define GREEN_PIN     D3

// Blue is not required by the alert logic.
// The original project uses RED for ALERT
// and GREEN for NORMAL.

// ---------------- SETTINGS ----------------
#define LORA_FREQUENCY 433E6
#define GAS_THRESHOLD  3000

String receivedData = "";

unsigned long lastPanicSend = 0;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ---------------- INPUTS ----------------

  pinMode(VIB_PIN, INPUT);

  pinMode(
    BUTTON_PIN,
    INPUT_PULLUP
  );

  // ---------------- OUTPUTS ----------------

  pinMode(
    BUZZER_PIN,
    OUTPUT
  );

  pinMode(
    RED_PIN,
    OUTPUT
  );

  pinMode(
    GREEN_PIN,
    OUTPUT
  );

  // ---------------- DEFAULT STATE ----------------

  // Active LOW buzzer
  digitalWrite(
    BUZZER_PIN,
    HIGH
  );

  // Normal = GREEN
  digitalWrite(
    RED_PIN,
    LOW
  );

  digitalWrite(
    GREEN_PIN,
    HIGH
  );


  // =================================================
  // LORA INITIALIZATION
  // =================================================

  // ESP8266 hardware SPI:
  // D5 = SCK
  // D6 = MISO
  // D7 = MOSI
  // D8 = SS

  SPI.begin();

  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );


  if (!LoRa.begin(LORA_FREQUENCY)) {

    Serial.println(
      "LoRa initialization failed!"
    );

    while (1) {
      delay(1000);
    }
  }


  // ---------------- LORA SETTINGS ----------------

  LoRa.setSpreadingFactor(7);

  LoRa.setSignalBandwidth(
    125E3
  );

  LoRa.setCodingRate4(5);

  LoRa.setSyncWord(0x12);

  LoRa.setTxPower(17);

  LoRa.enableCrc();


  Serial.println();
  Serial.println(
    "================================"
  );
  Serial.println(
    " SMART MINE SAFETY RECEIVER"
  );
  Serial.println(
    " ESP8266 + SX1278 LoRa"
  );
  Serial.println(
    "================================"
  );

  Serial.println(
    "Receiver Ready"
  );
}


// =====================================================
// RGB LED CONTROL
// =====================================================

void setColor(bool alert) {

  if (alert) {

    // RED = ALERT
    digitalWrite(
      RED_PIN,
      HIGH
    );

    digitalWrite(
      GREEN_PIN,
      LOW
    );

  } else {

    // GREEN = NORMAL
    digitalWrite(
      RED_PIN,
      LOW
    );

    digitalWrite(
      GREEN_PIN,
      HIGH
    );
  }
}


// =====================================================
// EXTRACT GAS VALUE
// =====================================================

int getGasValue(String data) {

  int gasIndex =
    data.indexOf("GAS:");

  if (gasIndex == -1) {
    return -1;
  }

  int commaIndex =
    data.indexOf(
      ",",
      gasIndex
    );

  if (commaIndex == -1) {
    return -1;
  }

  String gasString =
    data.substring(
      gasIndex + 4,
      commaIndex
    );

  return gasString.toInt();
}


// =====================================================
// SEND PANIC MESSAGE
// =====================================================

void sendPanicSignal() {

  LoRa.beginPacket();

  LoRa.print(
    "PANIC:RX"
  );

  LoRa.endPacket();

  LoRa.idle();

  Serial.println(
    "PANIC SIGNAL SENT"
  );
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // =================================================
  // READ LOCAL INPUTS
  // =================================================

  bool vibrationDetected =
    (digitalRead(VIB_PIN) == HIGH);

  bool buttonPressed =
    (digitalRead(BUTTON_PIN) == LOW);


  // =================================================
  // RECEIVER PANIC BUTTON
  // =================================================

  if (
    buttonPressed &&
    millis() - lastPanicSend > 1000
  ) {

    lastPanicSend =
      millis();

    sendPanicSignal();
  }


  // =================================================
  // RECEIVE LORA DATA
  // =================================================

  int packetSize =
    LoRa.parsePacket();


  if (packetSize) {

    receivedData = "";


    while (LoRa.available()) {

      receivedData +=
        (char)LoRa.read();
    }


    int rssi =
      LoRa.packetRssi();


    Serial.println();
    Serial.println(
      "-------------------------------"
    );

    Serial.println(
      "RAW DATA:"
    );

    Serial.println(
      receivedData
    );


    // =================================================
    // VERIFY WORKER PACKET
    // =================================================

    if (
      !receivedData.startsWith(
        "ID:01"
      )
    ) {

      Serial.println(
        "Unknown packet ignored"
      );

      return;
    }


    // =================================================
    // ALERT PROCESSING
    // =================================================

    bool alert = false;


    // ---------------- LOCAL ALERTS ----------------

    if (vibrationDetected) {

      alert = true;
    }


    if (buttonPressed) {

      alert = true;
    }


    // ---------------- FLAME ----------------

    if (
      receivedData.indexOf(
        "FLAME:FIRE"
      ) != -1
    ) {

      alert = true;
    }


    // ---------------- FALL ----------------

    if (
      receivedData.indexOf(
        "FALL:YES"
      ) != -1
    ) {

      alert = true;
    }


    // ---------------- PANIC ----------------

    if (
      receivedData.indexOf(
        "PANIC:YES"
      ) != -1
    ) {

      alert = true;
    }


    // ---------------- GAS ----------------

    int gasValue =
      getGasValue(
        receivedData
      );


    if (
      gasValue != -1 &&
      gasValue > GAS_THRESHOLD
    ) {

      alert = true;
    }


    // =================================================
    // OUTPUT CONTROL
    // =================================================

    digitalWrite(
      BUZZER_PIN,
      alert ? LOW : HIGH
    );


    setColor(alert);


    // =================================================
    // FINAL DATA PACKET
    // =================================================

    String finalData =
      receivedData;


    finalData +=
      ",VIB:";

    finalData +=
      (
        vibrationDetected
        ? "YES"
        : "NO"
      );


    finalData +=
      ",BUTTON:";

    finalData +=
      (
        buttonPressed
        ? "PRESSED"
        : "RELEASED"
      );


    finalData +=
      ",ALERT:";

    finalData +=
      (
        alert
        ? "YES"
        : "NO"
      );


    finalData +=
      ",RSSI:";

    finalData +=
      String(rssi);


    // =================================================
    // SERIAL OUTPUT
    // =================================================

    Serial.println(
      "PROCESSED DATA:"
    );

    Serial.println(
      finalData
    );

    Serial.println(
      "-------------------------------"
    );
  }


  delay(10);
}
