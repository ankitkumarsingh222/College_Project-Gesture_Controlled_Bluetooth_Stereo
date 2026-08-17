#include "ESP_I2S.h"
#include "BluetoothA2DPSink.h"

const uint8_t I2S_SCK = 5;       /* Audio data bit clock */
const uint8_t I2S_WS = 25;       /* Audio data left and right clock */
const uint8_t I2S_SDOUT = 26;    /* ESP32 audio data output (to speakers) */
I2SClass i2s;

BluetoothA2DPSink a2dp_sink(i2s);



// Define IR sensor pins
#define IR_LEFT_PIN 32  // Detect left swipe (Previous Track)
#define IR_RIGHT_PIN 33 // Detect right swipe (Next Track)
#define IR_UP_PIN 25    // Detect up swipe (Volume Up)
#define IR_DOWN_PIN 26  // Detect down swipe (Volume Down)

// Gesture thresholds (adjust based on your IR sensor)
#define GESTURE_THRESHOLD 500
#define GESTURE_DELAY 200  // Delay to debounce gestures

void setup() {
    i2s.setPins(I2S_SCK, I2S_WS, I2S_SDOUT);
    if (!i2s.begin(I2S_MODE_STD, 44100, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
      Serial.println("Failed to initialize I2S!");
      while (1); // do nothing
    }

  
  Serial.begin(115200);

  // Initialize IR sensors as input
  pinMode(IR_LEFT_PIN, INPUT_PULLUP);
  pinMode(IR_RIGHT_PIN, INPUT_PULLUP);
  pinMode(IR_UP_PIN, INPUT_PULLUP);
  pinMode(IR_DOWN_PIN, INPUT_PULLUP);

  // Initialize Bluetooth A2DP Sink
  a2dp_sink.start("ESP32 Gesture Speaker");
  Serial.println("ESP32 Gesture-Controlled Bluetooth Speaker is ready!");
}

void loop() {
  // Detect gestures and perform actions
  detectGestures();

  // Allow time for processing
  delay(100);
}

void detectGestures() {
  static unsigned long lastGestureTime = 0;

  // Avoid rapid successive gestures
  if (millis() - lastGestureTime < GESTURE_DELAY) {
    return;
  }

  // Read sensor values
  int leftState = digitalRead(IR_LEFT_PIN);
  int rightState = digitalRead(IR_RIGHT_PIN);
  int upState = digitalRead(IR_UP_PIN);
  int downState = digitalRead(IR_DOWN_PIN);

  // Check for gestures
  if (leftState == LOW) {
    Serial.println("Left swipe detected: Previous track");
    a2dp_sink.previous();
    lastGestureTime = millis();
  } else if (rightState == LOW) {
    Serial.println("Right swipe detected: Next track");
    a2dp_sink.next();
    lastGestureTime = millis();
  } else if (upState == LOW) {
    Serial.println("Up swipe detected: Volume up");
    a2dp_sink.pause();
    lastGestureTime = millis();
  } else if (downState == LOW) {
    Serial.println("Down swipe detected: Volume down");
    a2dp_sink.play();
    lastGestureTime = millis();
  }
}
