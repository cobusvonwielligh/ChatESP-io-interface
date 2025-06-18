# ChatESP

<p align="center">
  <img src="chatESP-github-logo.png" width="60%" alt="ChatESP logo">
</p>

ChatESP turns an ESP32 with a small OLED display into a portable weather station and ChatGPT client. It retrieves real-time weather data and prints ChatGPT replies directly on the screen.

## Features

- Current temperature and daily min/max from the Open‑Meteo API
- Simple sun/rain icons with a progress bar for updates
- Typewriter effect for ChatGPT responses
- `IMAGE:` prefix draws a 32×32 monochrome bitmap
- Works with built-in or external SSD1306 displays

### (!) Future features planned (!):

 - adding support for ![Home Assistant](https://www.home-assistant.io/) => be able to prompt-control IoT devices
 - integrating a mic as another input source => be able to prompt using voice
 - integrating a camera as another input source => be able to prompt using image feed/snapshot => be able to ask for clothing suggestions etc
 - adding support for a larger display => be able to see more information like calender or full weather report
 - adding support for a small speaker => used to indicate and notify
 - adding support for reactive LEDS => used to indicate and notify

## Hardware Requirements

- ESP32 development board. This project uses the Wemos Lolin32 board (comes with ESP-WROOM-32 Processor)
- 0.96" SSD1306 OLED display
- Micro-USB cable

### Built-in OLED (e.g. Wemos Lolin32 board)

![Wemos Lolin32](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Lolin32-OLED.jpg?w=750&quality=100&strip=all&ssl=1)

- SDA on GPIO 5
- SCL on GPIO 4

### External OLED

![External display wiring](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1)

- Vin → 3.3&nbsp;V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22

#### If your ESP32 development board has a different pin layout
Change the `Wire.begin()` call in `ESP32_CHAT.ino` to match your pins.

## Arduino IDE 2

1. [Download Arduino IDE&nbsp;2](https://www.arduino.cc/en/software) and install any required drivers.
2. Open **File → Preferences** and add:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   to *Additional Board Manager URLs*.
3. In **Tools → Board → Boards Manager**, install **ESP32 by Espressif Systems**.
4. Install the **Adafruit SSD1306**, **Adafruit GFX** and **ArduinoJson** libraries via the library manager.

## Project setup

1. Clone this repository and open `ESP32_CHAT/ESP32_CHAT.ino`.
2. Copy `ESP32_CHAT/secrets.example.h` to `ESP32_CHAT/secrets.h`.
3. Edit `secrets.h` and set your Wi‑Fi credentials, OpenAI API key and location:
   ```
   #define PLACE_NAME "City"
   #define PLACE_LAT  "12.34"
   #define PLACE_LNG  "56.78"
   ...
   ```
4. Select your board and COM port, then compile and upload.

## Usage

- On boot the device shows the weather and updates about every 30 seconds.
- Open the serial monitor (115200 baud) to send ChatGPT prompts.
- Prefix a prompt with `IMAGE:` to request a small bitmap.

## License

Released under the MIT License.
