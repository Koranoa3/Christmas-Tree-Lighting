#include "patterns.h"

// 全LED白点灯（静的）
void patternSolidWhite(uint32_t tick, CRGB* leds, uint16_t numLeds) {
    fill_solid(leds, numLeds, CRGB::White);
}

// レインボー（虹色が流れる）
void patternRainbow(uint32_t tick, CRGB* leds, uint16_t numLeds) {
    uint8_t hue = (tick / 10) % 256;
    fill_rainbow(leds, numLeds, hue, 7);
}

// チェイス（追いかけるパターン）
void patternChase(uint32_t tick, CRGB* leds, uint16_t numLeds) {
    fill_solid(leds, numLeds, CRGB::Black);
    uint16_t pos = (tick / 100) % numLeds;
    leds[pos] = CRGB::White;
}

// ブリージング（呼吸のように明滅）
void patternBreathing(uint32_t tick, CRGB* leds, uint16_t numLeds) {
    uint8_t brightness = beatsin8(30, 0, 255, 0, tick / 4);
    fill_solid(leds, numLeds, CRGB::White);
    for (uint16_t i = 0; i < numLeds; i++) {
        leds[i].nscale8(brightness);
    }
}

// きらめき（ランダムに点滅）
void patternTwinkle(uint32_t tick, CRGB* leds, uint16_t numLeds) {
    fadeToBlackBy(leds, numLeds, 20);
    if (random8() < 80) {
        leds[random16(numLeds)] = CRGB::White;
    }
}
