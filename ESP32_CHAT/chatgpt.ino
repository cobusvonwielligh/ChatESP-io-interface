#include "chatgpt.h"
#include "display.h"
#include "secrets.h"

/* Lightweight ChatGPT client */

static HTTPClient http;
static const char* apiKey = OPENAI_API_KEY;
static String apiUrl = "https://api.openai.com/v1/chat/completions";
static String finalPayload = "";
static bool initialPrompt = true;

static String chatGptText = "";
static bool showTyping = false;
static int typingIndex = 0;
static unsigned long lastTypingTime = 0;
static const int typingDelay = 50;

void initChatGpt() {
  http.begin(apiUrl);
}

void resetChatState() {
  chatGptText = "";
  showTyping = false;
  typingIndex = 0;
}

bool isTyping() {
  return showTyping;
}

void callChatGpt(String prompt) {
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(apiKey));

  if (initialPrompt) {
    finalPayload = "{\"model\": \"gpt-3.5-turbo\",\"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}]}";
    initialPrompt = false;
  } else {
    finalPayload = finalPayload + ",{\"role\": \"user\", \"content\": \"" + prompt + "\"}]}";
  }

  int code = http.POST(finalPayload);
  if (code == 200) {
    String response = http.getString();
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, response);
    chatGptText = doc["choices"][0]["message"]["content"].as<String>();
    chatGptText.replace("\n", " ");
    showTyping = true;
    typingIndex = 0;
  } else {
    chatGptText = "Error: " + String(code);
    showTyping = true;
    typingIndex = 0;
  }
}

String getChatGptPartialResponse() {
  if (typingIndex <= chatGptText.length()) {
    return chatGptText.substring(0, typingIndex++);
  }
  return chatGptText;
}

unsigned long getLastTypingTime() {
  return lastTypingTime;
}

void updateLastTypingTime() {
  lastTypingTime = millis();
}

int getTypingDelay() {
  return typingDelay;
}

bool callChatGptImage(String prompt, uint8_t* bitmap,
                      size_t width, size_t height) {
  HTTPClient imgClient;
  imgClient.begin(apiUrl);
  imgClient.addHeader("Content-Type", "application/json");
  imgClient.addHeader("Authorization", "Bearer " + String(apiKey));

  String artRequest =
      String("Return a ") + width + "x" + height +
      " monochrome image using '#' for filled pixels and '.' for empty. " +
      prompt + " Provide only the lines.";
  String payload =
      String("{\"model\": \"gpt-3.5-turbo\",\"messages\": [{\"role\": \"user\",\"content\": \"") +
      artRequest + "\"}]}";

  int code = imgClient.POST(payload);
  if (code != 200) {
    imgClient.end();
    return false;
  }

  String response = imgClient.getString();
  imgClient.end();
  DynamicJsonDocument doc(4096);
  if (deserializeJson(doc, response)) return false;

  String art = doc["choices"][0]["message"]["content"].as<String>();
  art.replace("\r", "");

  /* clear buffer */
  memset(bitmap, 0, width * height / 8);

  int x = 0, y = 0;
  for (size_t i = 0; i < art.length() && y < height; i++) {
    char c = art[i];
    if (c == '#') {
      size_t idx = y * width + x;
      bitmap[idx / 8] |= 0x80 >> (idx % 8);
      x++;
    } else if (c == '.') {
      x++;
    } else if (c == '\n') {
      y++;
      x = 0;
    }
    if (x >= (int)width) {
      y++;
      x = 0;
    }
  }

  return true;
}
