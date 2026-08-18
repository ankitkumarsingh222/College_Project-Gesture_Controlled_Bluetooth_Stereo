# 🎵 Gesture-Controlled Bluetooth Stereo

A Bluetooth stereo system that allows users to control music playback using **hand gestures** detected by IR sensors. The project uses an **ESP32** as the main controller, receives audio wirelessly through **Bluetooth A2DP Protocol**, and sends the audio signal to an external amplifier through the **I2S interface**.

The system provides a simple, touch-free way to control music playback using left, right, up, and down hand gestures.

---

## 🚀 Project Overview

The **Gesture-Controlled Bluetooth Stereo** combines:

* ESP32 microcontroller
* Bluetooth A2DP audio streaming
* IR sensors for gesture detection
* I2S digital audio output
* External audio amplifier
* Stereo speakers

The ESP32 receives music from a smartphone via Bluetooth and outputs the digital audio through I2S. At the same time, IR sensors detect hand gestures and the ESP32 sends corresponding media-control commands.

---

## ✨ Features

* 📱 Wireless Bluetooth music streaming
* 👋 Gesture-based music control
* ⏮️ Previous track
* ⏭️ Next track
* ⏸️ Pause playback
* ▶️ Resume playback
* 🔊 Volume increase using gesture
* 🔉 Volume decrease using gesture
* 🔊 Stereo audio output
* ⚡ ESP32-based control system
* 🎛️ Debouncing/delay to prevent repeated gesture detection

---

## 🛠️ Hardware Components

| Component           | Purpose                                |
| ------------------- | -------------------------------------- |
| ESP32               | Main controller and Bluetooth receiver |
| IR Sensor × 4       | Detect hand gestures                   |
| TPA3116D2 Amplifier | Amplifies audio signal                 |
| 5.25-inch Woofers   | Low-frequency audio output             |
| 3-inch Tweeters     | High-frequency audio output            |
| 18650 Battery       | Portable power source                  |
| Wires & Connectors  | Circuit connections                    |

---

## 🔌 ESP32 Pin Configuration

### I2S Audio

| ESP32 Pin | Function               |
| --------- | ---------------------- |
| GPIO 5    | I2S SCK / Bit Clock    |
| GPIO 25   | I2S WS / Word Select   |
| GPIO 26   | I2S SDOUT / Audio Data |

### IR Gesture Sensors

| Sensor |    GPIO | Action         |
| ------ | ------: | -------------- |
| Left   | GPIO 32 | Previous Track |
| Right  | GPIO 33 | Next Track     |
| Up     | GPIO 25 | Pause          |
| Down   | GPIO 26 | Play           |

> **Note:** The current program assigns GPIO 25 and GPIO 26 to both I2S signals and IR sensors. This pin overlap can cause hardware conflicts. For a final hardware implementation, the IR sensors should preferably be moved to separate GPIO pins.

---

## 💻 Software & Libraries

The Arduino program uses:

```cpp
#include "ESP_I2S.h"
#include "BluetoothA2DPSink.h"
```

### Main technologies

* Arduino IDE
* ESP32
* Bluetooth A2DP
* I2S
* Digital IR sensing
* C/C++

---

## ⚙️ How the Arduino Program Works

### 1. I2S Initialization

The program first configures the ESP32's I2S interface:

```cpp
i2s.setPins(I2S_SCK, I2S_WS, I2S_SDOUT);
```

The I2S interface is configured for:

* 44.1 kHz sampling rate
* 16-bit audio
* Stereo mode
* Both left and right audio channels

```cpp
i2s.begin(
    I2S_MODE_STD,
    44100,
    I2S_DATA_BIT_WIDTH_16BIT,
    I2S_SLOT_MODE_STEREO,
    I2S_STD_SLOT_BOTH
);
```

This allows the ESP32 to send digital stereo audio to the audio output stage.

---

### 2. Bluetooth A2DP Initialization

The program creates a Bluetooth A2DP sink:

```cpp
BluetoothA2DPSink a2dp_sink(i2s);
```

Then Bluetooth is started with:

```cpp
a2dp_sink.start("ESP32 Gesture Speaker");
```

The ESP32 therefore appears as a Bluetooth audio device named:

**ESP32 Gesture Speaker**

A smartphone or other Bluetooth device can connect to it and stream music.

---

### 3. IR Sensor Configuration

Four IR sensors are configured as digital inputs:

```cpp
pinMode(IR_LEFT_PIN, INPUT_PULLUP);
pinMode(IR_RIGHT_PIN, INPUT_PULLUP);
pinMode(IR_UP_PIN, INPUT_PULLUP);
pinMode(IR_DOWN_PIN, INPUT_PULLUP);
```

The use of `INPUT_PULLUP` means the sensor inputs normally remain HIGH and become LOW when the corresponding sensor detects the required gesture/object condition.

---

### 4. Gesture Detection

The `detectGestures()` function continuously reads the four IR sensors:

```cpp
int leftState = digitalRead(IR_LEFT_PIN);
int rightState = digitalRead(IR_RIGHT_PIN);
int upState = digitalRead(IR_UP_PIN);
int downState = digitalRead(IR_DOWN_PIN);
```

The program then checks which sensor has been activated.

### Gesture Mapping

```text
Left  → Previous Track
Right → Next Track
Up    → Pause
Down  → Play
```

For example:

```cpp
if (leftState == LOW) {
    a2dp_sink.previous();
}
```

This sends a previous-track command through the Bluetooth A2DP interface.

Similarly:

```cpp
else if (rightState == LOW) {
    a2dp_sink.next();
}
```

moves to the next track.

---

## ⏱️ Gesture Debouncing

The program includes a delay mechanism to prevent one gesture from being detected repeatedly:

```cpp
#define GESTURE_DELAY 200
```

The function stores the time of the previous gesture:

```cpp
static unsigned long lastGestureTime = 0;
```

Before processing a new gesture, it checks:

```cpp
if (millis() - lastGestureTime < GESTURE_DELAY) {
    return;
}
```

This creates a **200 ms debounce period**, reducing accidental repeated commands caused by sensor fluctuations.

---

## 🔄 Program Flow

```text
                         ┌─────────────────────┐
                         │        ESP32        │
                         │   Main Controller   │
                         └──────────┬──────────┘
                                    │
                  ┌─────────────────┴─────────────────┐
                  │                                   │
                  ▼                                   ▼
        ┌───────────────────┐              ┌────────────────────┐
        │   Bluetooth A2DP  │              │    IR Sensors      │
        │   Audio Receiver  │              │ Gesture Detection  │
        └─────────┬─────────┘              └─────────┬──────────┘
                  │                                  │
                  │                         ┌────────┴────────┐
                  │                         │                 │
                  │                         ▼                 ▼
                  │                  Gesture Detected   Debounce
                  │                         │
                  │                ┌────────┼────────┐
                  │                │        │        │
                  │                ▼        ▼        ▼
                  │             Left/Right Up/Down  Stable
                  │                │        │         │
                  │                │        │         │
                  │                ▼        ▼         ▼
                  │             Track    Volume      Play/Pause
                  │             Control  Control
                  │
                  │
                  ▼
          ┌─────────────────┐
          │   I2S Interface │
          │  Digital Audio  │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │ TPA3116D2 Audio │
          │    Amplifier    │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │ Stereo Speakers │
          │ Woofer + Tweeter│
          └─────────────────┘
```

### Gesture Control Mapping

| Gesture  | Function        |
| -------- | --------------- |
| 👈 Left  | Previous Track  |
| 👉 Right | Next Track      |
| 👆 Up    | Volume Increase |
| 👇 Down  | Volume Decrease |

### Complete Working Sequence

1. **ESP32 starts and initializes the I2S interface.**
2. I2S is configured for **44.1 kHz, 16-bit, stereo audio**.
3. The four IR sensors are configured as digital inputs.
4. The ESP32 starts the **Bluetooth A2DP Sink** and becomes discoverable as `ESP32 Gesture Speaker`.
5. A smartphone connects to the ESP32 and streams music wirelessly.
6. The received Bluetooth audio is transferred through the **I2S interface**.
7. The I2S audio signal is sent to the **TPA3116D2 amplifier**.
8. The amplifier drives the stereo **woofers and tweeters**.
9. At the same time, the ESP32 continuously monitors the IR sensors.
10. When a hand gesture is detected, the corresponding action is identified.
11. A **200 ms debounce delay** prevents accidental repeated commands.
12. The ESP32 performs the corresponding media-control operation:

    * Left → Previous track
    * Right → Next track
    * Up → Increase volume
    * Down → Decrease volume
13. The system continues monitoring for the next gesture while Bluetooth audio playback continues.

### Simplified Working

```text
Smartphone
    │
    │ Bluetooth A2DP
    ▼
  ESP32 ──────────────── IR Sensors
    │                         │
    │ I2S Audio              │
    ▼                         ▼
Amplifier              Gesture Detection
    │                         │
    ▼                         ▼
Speakers          Track / Volume Control
```

The ESP32 therefore performs **two functions simultaneously**: it acts as a **Bluetooth audio receiver** and as the **gesture-based media controller**.


## 🧠 Arduino Program Logic

The overall program follows this sequence:

1. Initialize the ESP32 I2S interface.
2. Configure the I2S audio parameters.
3. Initialize the serial monitor.
4. Configure the four IR sensors as inputs.
5. Start the Bluetooth A2DP sink.
6. Wait for Bluetooth audio streaming.
7. Continuously read the IR sensors.
8. Identify the activated sensor.
9. Apply the gesture debounce delay.
10. Send the corresponding media-control command.
11. Continue monitoring for the next gesture.

---


---

## 🔧 Libraries Required

Install/configure an ESP32 board package that provides:

```cpp
ESP_I2S.h
```

and install the Bluetooth A2DP library providing:

```cpp
BluetoothA2DPSink.h
```

The project is intended to be compiled using the **Arduino IDE with an ESP32 board selected**.

---

## 🎯 Applications

This project demonstrates applications of:

* Human-machine interaction
* Contactless control systems
* Bluetooth audio systems
* Embedded systems
* IoT-based interfaces
* Gesture recognition
* ESP32-based consumer electronics

---

## 🔮 Future Improvements

Possible improvements include:

* Add dedicated GPIO pins for all IR sensors
* Implement actual volume-up and volume-down controls
* Add OLED/LCD display for track information
* Add rechargeable battery management
* Improve gesture recognition accuracy
* Replace individual IR sensors with a more advanced gesture sensor
* Add physical buttons as a backup control system
* Add battery-level monitoring
* Improve enclosure and speaker design

---

## 👨‍💻 Technologies Used

**Hardware:**
ESP32 • IR Sensors • TPA3116D2 • Woofers • Tweeters • 18650 Battery

**Software:**
Arduino IDE • C/C++ • Bluetooth A2DP • I2S

**Concepts:**
Embedded Systems • Bluetooth Communication • Digital Sensors • Audio Processing • Gesture-Based Human-Machine Interaction

##
