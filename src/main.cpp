#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "patterns.h"

#define BTN_PIN 18
#define LED_PIN 19

CRGB leds[NUM_LEDS];
uint32_t tick = 0;

// Patterns
PatternFunc patterns[] = {
    patternRainbow,
    patternTwinkle,
    patternWipe,
    patternFire,
    patternRGStripe,
};
int currentPatternIndex;
PatternFunc currentPattern;

static bool butterflyMode = false;
void toggleButterflyMode()
{
  butterflyMode = !butterflyMode;
  if (butterflyMode)
  {
    Serial.println("Butterfly mode enabled");
  }
  else
  {
    Serial.println("Butterfly mode disabled");
  }
}

void cyclePattern()
{
  currentPatternIndex = (currentPatternIndex + 1) % (sizeof(patterns) / sizeof(patterns[0]));
  currentPattern = patterns[currentPatternIndex];
  Serial.printf("Pattern changed to: %d\n", currentPatternIndex);
}

// Animation speed
uint16_t animSpeedLevels[] = {500, 1000, 2000};
int currentSpeedIndex;

void cycleSpeed()
{
  currentSpeedIndex = (currentSpeedIndex + 1) % (sizeof(animSpeedLevels) / sizeof(animSpeedLevels[0]));
  Serial.printf("Animation speed changed to: %d ms\n", animSpeedLevels[currentSpeedIndex]);
}

// Setup function

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  butterflyMode = false;
  currentPatternIndex = DEFAULT_PATTERN_INDEX;
  currentPattern = patterns[currentPatternIndex];
  currentSpeedIndex = DEFAULT_ANIM_SPEED_INDEX; // Default to medium speed

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  tick = millis();

  // ボタンが押されたらパターンを切り替え、長押しで速度変更
  static bool lastBtnState = HIGH;
  static unsigned long btnPressTime = 0;
  bool btnState = digitalRead(BTN_PIN);

  if (lastBtnState == HIGH && btnState == LOW)
  {
    // ボタンが押された瞬間
    btnPressTime = millis();
  }
  else if (lastBtnState == LOW && btnState == HIGH)
  {
    // ボタンが離された瞬間
    unsigned long pressDuration = millis() - btnPressTime;
    if (pressDuration >= 1500)
    {
      toggleButterflyMode();
    }
    else if (pressDuration >= 500)
    {
      cycleSpeed();
    }
    else
    {
      cyclePattern();
    }
  }
  lastBtnState = btnState;

  // 現在のパターンを実行

  if (butterflyMode)
  {
    currentPattern(tick, leds, (NUM_LEDS + 1) / 2, animSpeedLevels[currentSpeedIndex]);

    // パターンの複製・反転追加
    for (uint16_t i = 0; i < NUM_LEDS / 2; i++)
    {
      leds[NUM_LEDS - 1 - i] = leds[i];
    }
  }
  else
    currentPattern(tick, leds, NUM_LEDS, animSpeedLevels[currentSpeedIndex]);

  FastLED.show();
  delay(16); // 約60fps
}
