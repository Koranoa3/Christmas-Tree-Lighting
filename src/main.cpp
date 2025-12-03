#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 19
#define NUM_LEDS 300 // LEDの数 (max 300)
#define BRIGHTNESS 63// 明るさ (0-255)
#define ANIM_CYCLE_MS 3000

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  // 1~10個目のLEDを順番に白点灯
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::White;
    FastLED.show();
    delay(500);
  }

  // 全消灯してリセット
  FastLED.clear();
  FastLED.show();
  delay(1000);
}
