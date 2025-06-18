#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/* ChatGPT API interface */

enum Page { PAGE_WEATHER, PAGE_CHATGPT };

void initChatGpt();
void callChatGpt(String prompt);
void resetChatState();
bool isTyping();
String getChatGptPartialResponse();
unsigned long getLastTypingTime();
void updateLastTypingTime();
int getTypingDelay();

/* Request a small monochrome image.
 * Bitmap must hold width*height/8 bytes.
 * Returns true on success. */
bool callChatGptImage(String prompt, uint8_t* bitmap,
                      size_t width, size_t height);
