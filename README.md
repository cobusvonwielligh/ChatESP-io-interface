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

- ~~SSD1306 OLED display support~~ 
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
### ESP32 TFT Touch with Camera v1.3

- Chipset: ESP32-WROVER
- Display: 3.5inch 320x480 TFT LCD driver
- Display Driver: ILI9488, SPI communication
- Touch Chipset: Capacitive FT6236
- Camera: 2MP OV2640
- Storage: integrated Micro SD card slot
- Power: 5V, Type-C USB

<p align="left">
  <img src="https://i.ibb.co/N2gCqTf/ESPTFT35-CA-008.jpg" width="35%" style="border: 5px solid black; border-radius: 5px;">
</p>

<p align="left">
  <img src="https://i.ibb.co/thgTHgH/ESPTFT35-CA-007.jpg" width="35%" style="border: 5px solid black; border-radius: 5px;">
</p>

#### Additional Resources
1. [Product Wiki](https://wiki.makerfabs.com/MaTouch_3.5_TFT_Touch_with_Camera.html)
2. [Example Code](https://github.com/Makerfabs/Project_Touch-Screen-Camera)
3. [Display Driver Code](https://github.com/microrobotics/ESPTFT35CA)

---

## Software Prerequisites
UPDATE THIS

### Arduino IDE V1.8.10
[Software Setup](https://www.arduino.cc/en/software) 

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
- Prefix prompt with `IMAGE:` to request a bitmap response.

UPDATE THIS
---


## License

Released under the MIT License.


---
