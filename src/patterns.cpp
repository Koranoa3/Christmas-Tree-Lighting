#include "patterns.h"
#include "functions.cpp"

// 全LED白点灯（静的）
void patternSolidWhite(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fill_solid(leds, numLeds, CRGB::White);
}

// レインボー（虹色が流れる）
void patternRainbow(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    uint8_t hue = (tick / 10) % 256;
    fill_rainbow(leds, numLeds, hue, 7);
}

// チェイス（追いかけるパターン）
void patternChase(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fill_solid(leds, numLeds, CRGB::Black);
    uint16_t pos = (tick / 100) % numLeds;
    leds[pos] = CRGB::White;
}

// ブリージング（呼吸のように明滅）
void patternBreathing(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    uint8_t brightness = beatsin8(30, 0, 255, 0, tick / 4);
    fill_solid(leds, numLeds, CRGB::White);
    for (uint16_t i = 0; i < numLeds; i++) {
        leds[i].nscale8(brightness);
    }
}

// きらめき（ランダムに点滅）
void patternTwinkle(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fadeToBlackBy(leds, numLeds, 20);
    if (random8() < 80) {
        leds[random16(numLeds)] = CRGB::White;
    }
}

void patternWipe(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fadeToBlackBy(leds, numLeds, 80);
    
    float t = (tick%animSpeed) / (float)animSpeed; // tickを秒単位に変換
    float prev = smoothstep(0.3f, 0.7f, t - 0.2f);
    float curr = smoothstep(0.3f, 0.7f, t);
    
    uint16_t startLed = (uint16_t)(prev * numLeds);
    uint16_t endLed = (uint16_t)(curr * numLeds);
    
    for (uint16_t i = startLed; i < endLed && i < numLeds; i++) {
        leds[i] = CRGB::White;
    }

    Serial.printf("%d",startLed);
}