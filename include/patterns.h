#ifndef PATTERNS_H
#define PATTERNS_H

#include <FastLED.h>

// パターン関数の型定義
// tick: アニメーションの経過時間（ミリ秒など）
// leds: LED配列へのポインタ
// numLeds: LED数
typedef void (*PatternFunc)(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);

// パターン関数の宣言
void patternSolidWhite(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternEndDebug(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternRainbow(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternChase(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternBreathing(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternTwinkle(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternWipe(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternFire(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternIce(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed, float impact);
void patternRGStripe(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed, float impact);

#endif // PATTERNS_H
