<h1 align="left" class="rainbow">ChatESP</h1>
<h3 align="left">Smart I/O control with ChatGPT on ESP32</h3>

---

<p align="center">
  <img src="chatESP-banner.gif">
</p>

---

<table>
  <tr>
    <td style="vertical-align:top; width:60%">

**ChatESP** transforms an ESP32 into a compact, portable AI companion and smart interface. The long-term goal is to develop a full-feature home automation and control interface that can manage sensors, I/O devices, and user interactions via voice, video, or text.

This project aims to evolve into a modular, AI-driven platform with support for IoT devices and I/O interfaces focussed on automation, convenience and companionship.

ChatESP should eventually be able to support:

- Audio and video input for prompt generation  
- IoT control via Home Assistant integration  
- Voice or text-based interaction  
- Smart lighting responses based on ambient brightness  
- Weather and alert monitoring  
- Spoken responses from ChatGPT  

</td>
    <td style="vertical-align:top; padding-left:1%; width:40%">

<h4>Use cases include:</h4>

- Smart mirror interface  
- Voice-controlled home dashboard  
- Desktop assistant for weather, alerts, and ChatGPT interaction  
- Wall-mounted information panel in a hallway or entrance  
- Embedded assistant in smart appliances  

</td>
  </tr>
</table>


---

<table>
  <tr>
    <td style="vertical-align:top; width:60%">

#### Development Roadmap

1. ~~SSD1306 OLED display driver, drawing and support~~ ✅  
2. ~~Display real-time weather info including weather icon~~ ✅  
3. ~~ChatGPT text prompt and response handeling~~ ✅  
4. Home Assistant integration for IoT device control and monitoring  
5. Microphone support for voice-driven prompts  
6. Camera integration to support image & video prompts (e.g., fashion suggestions)  
7. Larger display support for extended text, weather maps, or dashboards  
8. Small speaker for reply/notification playback  
9. Ambient reactive LED lighting for system status  
10. Smart 2-way mirror integration with ChatESP including UI  

</td>
    <td style="vertical-align:top; padding-left:1%; width:60%">

#### Current Features

- SSD1306 OLED display support  
- Display current temperature, min, and max on weather page using Open‑Meteo API  
- Dynamic weather icon (sun/rain) with update progress bar  
- ChatGPT API request and response handling  
- ChatGPT response text rendering with animated typing effect  
- `IMAGE:` prompt support draws 32×32 pixel monochrome bitmap  

</td>
  </tr>
</table>

---

## Hardware Requirements

- ESP32 development board (tested on Wemos Lolin32 with ESP-WROOM-32)
- 0.96" SSD1306 OLED display (I2C)
- Micro-USB cable


---

#### Built-in OLED (e.g. Wemos Lolin32)

<p align="left">
  <img src="https://cdn.xingosoftware.com/elektor/images/fetch/dpr_1,w_406,h_406,c_fit/https%3A%2F%2Fwww.elektormagazine.com%2Fassets%2Fupload%2Fimages%2F26%2F20190212142631_18575---top-view.jpg" width="30%" style="border: 5px solid black; border-radius: 5px;">
</p>

- SDA: GPIO 5  
- SCL: GPIO 4

---

#### External OLED Wiring

<p align="left">
  <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1" width="40%" style="border: 5px solid black; border-radius: 5px;">
</p>

- Vin → 3.3 V  
- GND → GND  
- SDA → GPIO 21  
- SCL → GPIO 22

_Update your `Wire.begin()` in `ESP32_CHAT.ino` if your dev board has a different pin layout._

---

## Software Prerequisites

#### Arduino IDE 2 Setup 

1. [Download Arduino IDE 2](https://www.arduino.cc/en/software)
2. Open **File → Preferences** and add:  
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`  
   to *Additional Board Manager URLs*.
3. Go to **Tools → Board → Boards Manager**, search and install **ESP32 by Espressif Systems**.
4. Install the following libraries:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `ArduinoJson`



#### Project Setup

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


### Usage

- On boot the device shows the weather and updates about every 30 seconds.
- Open Serial Monitor at 115200 baud to send ChatGPT prompts.
- Prefix prompt with `IMAGE:` to request a 32×32 bitmap response.


---


## License

Released under the MIT License.


---
