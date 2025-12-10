#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "patterns.h"

#define BTN1_PIN 18
#define BTN2_PIN 22
#define LED_PIN 19

CRGB leds[NUM_LEDS];
uint32_t tick = 0;

// Patterns
PatternFunc patterns[] = {
    patternEndDebug,
    patternRainbow,
    patternTwinkle,
    patternWipe,
    patternFire,
    patternIce,
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
uint16_t animSpeedLevels[] = {2000, 4000};
int currentSpeedIndex;

void cycleSpeed()
{
  currentSpeedIndex = (currentSpeedIndex + 1) % (sizeof(animSpeedLevels) / sizeof(animSpeedLevels[0]));
  Serial.printf("Animation speed changed to: %d ms\n", animSpeedLevels[currentSpeedIndex]);
}

// ボタン押下時間を検出する関数
// ボタンが離された瞬間に押下時間(ms)を返す、それ以外は0を返す
// btnIndex: 0=BTN1_PIN, 1=BTN2_PIN
unsigned long detectButtonPressDuration(uint8_t btnIndex)
{
  static bool lastBtnState[2] = {HIGH, HIGH};
  static unsigned long btnPressTime[2] = {0, 0};

  uint8_t pin = (btnIndex == 0) ? BTN1_PIN : BTN2_PIN;
  bool btnState = digitalRead(pin);
  unsigned long pressDuration = 0;

  if (lastBtnState[btnIndex] == HIGH && btnState == LOW)
  {
    // ボタンが押された瞬間
    btnPressTime[btnIndex] = millis();
  }
  else if (lastBtnState[btnIndex] == LOW && btnState == HIGH)
  {
    // ボタンが離された瞬間
    pressDuration = millis() - btnPressTime[btnIndex];
  }
  lastBtnState[btnIndex] = btnState;

  return pressDuration;
}

// Setup function

void setup()
{
  Serial.begin(115200);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
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

  // ボタン1 押下時間を検出し、対応する機能を発動
  unsigned long pressDuration = detectButtonPressDuration(0);
  if (pressDuration > 0)
  {
    if (pressDuration >= 1500)
      toggleButterflyMode();
    else if (pressDuration >= 500)
      cycleSpeed();
    else
      cyclePattern();
  }

  // ボタン2 押下時間を検出し、対応する機能を発動
  static int centerPercent = 50;
  pressDuration = detectButtonPressDuration(1);
  if (pressDuration > 0)
  {
    if (pressDuration >= 500)
      centerPercent -= 10;
    else
      centerPercent += 1;
    
    Serial.printf("Center percent adjusted to: %d%%\n", centerPercent);
  }

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
