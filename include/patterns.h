#ifndef PATTERNS_H
#define PATTERNS_H

#include <FastLED.h>

// パターン関数の型定義
// tick: アニメーションの経過時間（ミリ秒など）
// leds: LED配列へのポインタ
// numLeds: LED数
typedef void (*PatternFunc)(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);

// パターン関数の宣言
void patternSolidWhite(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternEndDebug(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternRainbow(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternChase(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternBreathing(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternTwinkle(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternWipe(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed);
void patternFire(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed);
void patternIce(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed);
void patternRGStripe(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed);

#endif // PATTERNS_H
