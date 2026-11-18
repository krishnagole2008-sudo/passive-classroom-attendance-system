
#include <Arduino.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==========================================
// 1. HARDWARE CONFIGURATION
// ==========================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define LED_PIN 23
#define BUZZER_PIN 18

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// ==========================================
// 2. REGISTERED BLE DEVICE
// ==========================================

const char* REGISTERED_ID = "ATTEND001";

const int RSSI_THRESHOLD = -80;

BLEScan* scanner;

// ==========================================
// 3. TIMING CONFIGURATION
// ==========================================

const unsigned long PRESENCE_TIMEOUT = 5000;
const unsigned long LED_DURATION = 1000;
const unsigned long BUZZER_DURATION = 150;
const unsigned long ALERT_COOLDOWN = 3000;

// ==========================================
// 4. GLOBAL VARIABLES
// ==========================================

bool deviceDetected = false;
bool alertRequested = false;
bool firstAlert = true;
bool ledActive = false;

int latestRSSI = 0;

unsigned long lastSeenTime = 0;
unsigned long lastAlertTime = 0;
unsigned long ledOnTime = 0;

// ==========================================
// 5. BLE DEVICE DETECTION
// ==========================================

class MyAdvertisedDeviceCallbacks :
  public BLEAdvertisedDeviceCallbacks {

  void onResult(BLEAdvertisedDevice device) override {

    if (!device.haveServiceData()) {
      return;
    }

    String serviceData =
      String(device.getServiceData().c_str());

    if (serviceData == REGISTERED_ID) {

      int rssi = device.getRSSI();

      // RSSI FILTERING
      if (rssi < RSSI_THRESHOLD) {
        return;
      }

      deviceDetected = true;
      latestRSSI = rssi;
      lastSeenTime = millis();

      // ALERT COOLDOWN
      if (firstAlert ||
          millis() - lastAlertTime >= ALERT_COOLDOWN) {

        alertRequested = true;
        lastAlertTime = millis();
        firstAlert = false;
      }
    }
  }
};

// ==========================================
// 6. OLED DISPLAY FUNCTION
// ==========================================

void updateDisplay() {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("PASSIVE ATTENDANCE");

  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  if (deviceDetected) {

    display.setCursor(0, 19);
    display.println("DEVICE DETECTED");

    display.setCursor(0, 32);
    display.print("ID: ");
    display.println(REGISTERED_ID);

    display.setCursor(0, 45);
    display.print("RSSI: ");
    display.print(latestRSSI);
    display.println(" dBm");

    display.setCursor(0, 57);
    display.println("STATUS: PRESENT");

  } else {

    display.setCursor(0, 25);
    display.println("Scanning BLE...");

    display.setCursor(0, 40);
    display.println("Waiting for device");

    display.setCursor(0, 55);
    display.println("NOT DETECTED");
  }

  display.display();
}

// ==========================================
// 7. BUZZER AND LED ALERT
// ==========================================

void triggerAlert() {

  if (!alertRequested) {
    return;
  }

  alertRequested = false;

  Serial.println("==========================");
  Serial.println("REGISTERED DEVICE DETECTED");

  Serial.print("DEVICE ID: ");
  Serial.println(REGISTERED_ID);

  Serial.print("RSSI: ");
  Serial.println(latestRSSI);

  // LED ON
  digitalWrite(LED_PIN, HIGH);

  ledActive = true;
  ledOnTime = millis();

  // BUZZER ON
  digitalWrite(BUZZER_PIN, HIGH);

  delay(BUZZER_DURATION);

  // BUZZER OFF
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("ALERT ACTIVATED");
  Serial.println("==========================");
}

// ==========================================
// 8. LED TIMING
// ==========================================

void updateLED() {

  if (ledActive &&
      millis() - ledOnTime >= LED_DURATION) {

    digitalWrite(LED_PIN, LOW);

    ledActive = false;
  }
}

// ==========================================
// 9. SETUP
// ==========================================

void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(21, 22);

  // OLED INITIALIZATION
  if (!display.begin(
        SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED NOT FOUND!");

    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 10);
  display.println("PASSIVE ATTENDANCE");

  display.setCursor(0, 30);
  display.println("Initializing BLE...");

  display.display();

  // BLE INITIALIZATION
  BLEDevice::init("ESP32_Attendance");

  scanner = BLEDevice::getScan();

  scanner->setAdvertisedDeviceCallbacks(
    new MyAdvertisedDeviceCallbacks()
  );

  scanner->setActiveScan(true);
  scanner->setInterval(100);
  scanner->setWindow(99);

  Serial.println("==========================");
  Serial.println("PASSIVE ATTENDANCE SYSTEM");
  Serial.println("SYSTEM READY");
  Serial.println("==========================");
}

// ==========================================
// 10. MAIN LOOP
// ==========================================

void loop() {

  deviceDetected = false;
  alertRequested = false;

  // BLE SCANNING
  scanner->start(1, false);

  scanner->clearResults();

  // PRESENCE VALIDATION
  deviceDetected =
    (lastSeenTime != 0 &&
     millis() - lastSeenTime <= PRESENCE_TIMEOUT);

  // ALERT SYSTEM
  triggerAlert();

  // LED CONTROL
  updateLED();

  // OLED UPDATE
  updateDisplay();

  delay(10);
}
