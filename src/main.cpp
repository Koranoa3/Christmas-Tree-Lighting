#include <Arduino.h>
#include <FastLED.h>
#include "patterns.h"

#define BTN_PIN 18
#define LED_PIN 19
#define NUM_LEDS 50   // LEDの数 (max 300)
#define BRIGHTNESS 63 // 全体の明るさ (0-255)

CRGB leds[NUM_LEDS];
uint32_t tick = 0;

// 現在のパターン関数ポインタ
PatternFunc patterns[] = {
    patternSolidWhite,
    patternRainbow,
    patternChase,
    patternBreathing,
    patternTwinkle
};
int currentPatternIndex;
PatternFunc currentPattern;

void setup()
{
  pinMode(BTN_PIN, INPUT_PULLUP);
  currentPatternIndex = 0;
  currentPattern = patterns[currentPatternIndex];

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  tick = millis();

  // ボタンが押されたらパターンを切り替え
  static bool lastBtnState = HIGH;
  bool btnState = digitalRead(BTN_PIN);
  if (lastBtnState == HIGH && btnState == LOW) {
    currentPatternIndex = (currentPatternIndex + 1) % (sizeof(patterns) / sizeof(patterns[0]));
    currentPattern = patterns[currentPatternIndex];
  }
  lastBtnState = btnState;

  // 現在のパターンを実行
  currentPattern(tick, leds, NUM_LEDS);

  FastLED.show();
  delay(16); // 約60fps
}
