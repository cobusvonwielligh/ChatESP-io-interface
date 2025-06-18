# ChatESP 
<h4 align="left" class="rainbow">with mini OLED Display</h4>

<p align="center">
  <img src="chatESP-github-logo.png" width="70%" alt="ChatESP logo" class="border-rainbow">
</p>

<style>
@keyframes rainbow-text {
  0% { background-position: 0% 50%; }
  100% { background-position: 100% 50%; }
}
.rainbow {
  font-weight: 700;
  background: linear-gradient(90deg, red, orange, yellow, green, blue, indigo, violet);
  background-size: 400% 100%;
  -webkit-background-clip: text;
  color: transparent;
  animation: rainbow-text 8s linear infinite;
}
.border-rainbow {
  border: 4px solid;
  border-image: linear-gradient(90deg, indigo, red, orange, red, indigo, orange) 1.5;
  border-radius: 8px;
}
</style>

---


### 🚧 Work in Progress
#### v0.20

**ChatESP** ChatESP transforms an ESP32 into a compact, portable AI companion. Its long-term vision is to evolve into a feature-rich home automation and control interface capable of managing multiple sensors, I/O devices, and user interactions through voice, video or text.

**Planned use cases include:**
- Smart mirror interface
- Home control station

A 

A desktop assistant for weather, alerts, and conversation

Currently it fetches local weather from Open‑Meteo and streams responses from ChatGPT directly to the screen, it can also display a small bitmap image generated from a prompt.

---


## Current Features

- Display current temperature, min, and max using Open‑Meteo API
- Dynamic weather icon (sun/rain) with update progress bar
- ChatGPT text rendering with animated typing effect
- `IMAGE:` prompt support draws 32×32 pixel monochrome bitmap
- SSD1306 OLED display support (internal or external)
- Wi-Fi based communication and modular architecture

---


### Planned Features

- Microphone support for voice-driven prompts
- Camera integration to support image queries (e.g., fashion suggestions)
- Home Assistant integration for IoT device control and feedback
- Larger display support for extended text, weather maps, or dashboards
- Small speaker for reply/notification playback
- Ambient reactive LED lighting for system status
- Smart mirror integration with ChatGPT, weather, calendar, and alerts

> _Smart Mirror powered by ChatESP: a real-time ambient interface behind glass_

---


## Hardware Requirements

- ESP32 development board (tested on Wemos Lolin32 with ESP-WROOM-32)
- 0.96" SSD1306 OLED display (I2C)
- Micro-USB cable for power and upload

---


### Built-in OLED (e.g., Wemos Lolin32)

![Wemos Lolin32](https://cdn.xingosoftware.com/elektor/images/fetch/dpr_1,w_406,h_406,c_fit/https%3A%2F%2Fwww.elektormagazine.com%2Fassets%2Fupload%2Fimages%2F26%2F20190212142631_18575---top-view.jpg)

- SDA: GPIO 5  
- SCL: GPIO 4

---


### External OLED Wiring

![External display wiring](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1)

- Vin → 3.3 V  
- GND → GND  
- SDA → GPIO 21  
- SCL → GPIO 22

_Update your `Wire.begin()` in `ESP32_CHAT.ino` if using different pins._

---


## Arduino IDE 2 Setup

1. [Download Arduino IDE 2](https://www.arduino.cc/en/software)
2. Open **File → Preferences** and add:  
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`  
   to *Additional Board Manager URLs*.
3. Go to **Tools → Board → Boards Manager**, search and install **ESP32 by Espressif Systems**.
4. Install the following libraries:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `ArduinoJson`

---


## Project Setup

1. Clone this repository and open `ESP32_CHAT/ESP32_CHAT.ino`.
2. Copy `ESP32_CHAT/secrets.example.h` → `ESP32_CHAT/secrets.h`
3. Edit `secrets.h` with your Wi‑Fi, API key, and coordinates:
   ```cpp
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASS "YourWiFiPassword"
   #define OPENAI_API_KEY "sk-..."
   #define PLACE_NAME "Your City"
   #define PLACE_LAT  "-33.9"
   #define PLACE_LNG  "18.4"
   ```
4. Select your board and COM port in Arduino IDE, then compile and upload.

---


## Usage

- On boot the device shows the weather and updates about every 30 seconds.
- Open Serial Monitor at 115200 baud to send ChatGPT prompts.
- Prefix prompt with `IMAGE:` to request a 32×32 bitmap response.

---


## License

Released under the MIT License.

---