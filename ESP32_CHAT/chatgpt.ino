#include "chatgpt.h"
#include "display.h"
#include "secrets.h"

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