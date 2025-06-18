#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

enum Page { PAGE_WEATHER, PAGE_CHATGPT };

void initChatGpt();
void callChatGpt(String prompt);
void resetChatState();
bool isTyping();
String getChatGptPartialResponse();
unsigned long getLastTypingTime();
void updateLastTypingTime();
int getTypingDelay();