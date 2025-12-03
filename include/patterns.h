#ifndef PATTERNS_H
#define PATTERNS_H

#include <FastLED.h>

// パターン関数の型定義
// tick: アニメーションの経過時間（ミリ秒など）
// leds: LED配列へのポインタ
// numLeds: LED数
typedef void (*PatternFunc)(uint32_t tick, CRGB *leds, uint16_t numLeds);

// パターン関数の宣言
void patternSolidWhite(uint32_t tick, CRGB *leds, uint16_t numLeds);
void patternRainbow(uint32_t tick, CRGB *leds, uint16_t numLeds);
void patternChase(uint32_t tick, CRGB *leds, uint16_t numLeds);
void patternBreathing(uint32_t tick, CRGB *leds, uint16_t numLeds);
void patternTwinkle(uint32_t tick, CRGB *leds, uint16_t numLeds);
void patternWipe(uint32_t tick, CRGB *leds, uint16_t numLeds);

#endif // PATTERNS_H
