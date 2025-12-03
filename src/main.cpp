#include <Arduino.h>
#include <FastLED.h>
#include "patterns.h"

#define BTN_PIN 18
#define LED_PIN 19
#define NUM_LEDS 50   // LEDの数 (max 300)
#define BRIGHTNESS 63 // 全体の明るさ (0-255)

CRGB leds[NUM_LEDS];
uint32_t tick = 0;

// Patterns
PatternFunc patterns[] = {
    patternRainbow,
    patternTwinkle,
    patternWipe,
};
int currentPatternIndex;
PatternFunc currentPattern;

void cyclePattern()
{
  currentPatternIndex = (currentPatternIndex + 1) % (sizeof(patterns) / sizeof(patterns[0]));
  currentPattern = patterns[currentPatternIndex];
  Serial.printf("Pattern changed to: %d\n", currentPatternIndex);
}

// Animation speed
uint16_t ANIM_SPEED = 1000; // アニメーション速度（ミリ秒）
uint16_t animSpeedLevels[] = {500, 1000, 2000};
int currentSpeedIndex;

void cycleSpeed()
{
  currentSpeedIndex = (currentSpeedIndex + 1) % (sizeof(animSpeedLevels) / sizeof(animSpeedLevels[0]));
  ANIM_SPEED = animSpeedLevels[currentSpeedIndex];
  Serial.printf("Animation speed changed to: %d ms\n", ANIM_SPEED);
}

// Setup function

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  currentPatternIndex = 0;
  currentPattern = patterns[currentPatternIndex];
  currentSpeedIndex = 1; // Default to medium speed

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
  if (lastBtnState == HIGH && btnState == LOW)
  {
    cyclePattern();
    // cycleSpeed();
  }
  lastBtnState = btnState;

  // 現在のパターンを実行
  currentPattern(tick, leds, NUM_LEDS, animSpeedLevels[currentSpeedIndex]);

  FastLED.show();
  delay(16); // 約60fps
}
