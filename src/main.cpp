#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "patterns.h"

#define BTN1_PIN 18
#define BTN2_PIN 22
#define LED_PIN 19

CRGB leds[NUM_LEDS];
uint32_t tick = 0;
uint32_t centerPercent = 50;

// Patterns
PatternFunc patterns[] = {
    patternRGStripe,
    patternRainbow,
    patternTwinkle,
    patternWipe,
    patternFire,
    patternIce,
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

// ボタンが押された瞬間を検出する関数
// btnIndex: 0=BTN1_PIN, 1=BTN2_PIN
bool detectButtonJustPressed(uint8_t btnIndex)
{
  static bool lastBtnState[2] = {HIGH, HIGH};

  uint8_t pin = (btnIndex == 0) ? BTN1_PIN : BTN2_PIN;
  bool btnState = digitalRead(pin);
  bool justPressed = false;

  if (lastBtnState[btnIndex] == HIGH && btnState == LOW)
  {
    // ボタンが押された瞬間
    justPressed = true;
  }
  lastBtnState[btnIndex] = btnState;

  return justPressed;
}

// Setup function

void setup()
{
  Serial.begin(115200);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  butterflyMode = DEFAULT_BUTTERFLY_MODE;
  currentPatternIndex = DEFAULT_PATTERN_INDEX;
  currentPattern = patterns[currentPatternIndex];
  currentSpeedIndex = DEFAULT_ANIM_SPEED_INDEX; // Default to medium speed
  centerPercent = DEFAULT_CENTER_PERCENT;

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

  static float impact = 0.0f;
  // ボタン2 押下を検出し、impactを増加させる
  if (detectButtonJustPressed(1))
    impact += 0.8f;
  impact *= 0.9f; // 徐々に減衰

  // 現在のパターンを実行

  if (butterflyMode)
  {
    int largerLeds = NUM_LEDS*centerPercent/100;
    int smallerLeds = NUM_LEDS - largerLeds;

    // 一時バッファを確保
    static CRGB tempLeds[NUM_LEDS];
    
    // largerLeds分をcurrentPatternで生成
    currentPattern(tick, tempLeds, largerLeds, animSpeedLevels[currentSpeedIndex], impact);
    
    if (centerPercent >= 50)
    {
      // 中央が多い: 前からlargerLeds分配置し、残りをsmallerLeds分削って反転
      for (int i = 0; i < largerLeds; i++)
      {
      leds[i] = tempLeds[i];
      }
      
      // smallerLeds分を適切な割合で抽出して反転配置
      for (int i = 0; i < smallerLeds; i++)
      {
      int srcIndex = i * largerLeds / smallerLeds;
      leds[NUM_LEDS - 1 - i] = tempLeds[srcIndex];
      }
    }
    else
    {
      // 中央が少ない: largerLeds分を後ろから反転配置し、残りを前から配置
      for (int i = 0; i < largerLeds; i++)
      {
      leds[NUM_LEDS - 1 - i] = tempLeds[i];
      }
      
      // smallerLeds分を前から配置
      for (int i = 0; i < smallerLeds; i++)
      {
      int srcIndex = i * largerLeds / smallerLeds;
      leds[i] = tempLeds[srcIndex];
      }
    }
  }
  else
    currentPattern(tick, leds, NUM_LEDS, animSpeedLevels[currentSpeedIndex], impact);

  FastLED.show();
  delay(16); // 約60fps
}
