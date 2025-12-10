#include "patterns.h"
#include "functions.cpp"

// 全LED白点灯（静的）
void patternSolidWhite(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fill_solid(leds, numLeds, CRGB::White);
}

// レインボー（虹色が流れる）
void patternRainbow(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    uint8_t hue = (tick / (animSpeed / 200)) % 256;
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
    if (random8() < 255) {
        leds[random16(numLeds)] = CRGB::White;
    }
}

void patternWipe(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fadeToBlackBy(leds, numLeds, 120);
    
    float t = (tick%animSpeed) / (float)animSpeed; // tickを秒単位に変換
    float prev = linearstep(0.3f, 0.7f, t - 0.2f);
    float curr = linearstep(0.3f, 0.7f, t);
    
    uint16_t startLed = (uint16_t)(prev * numLeds);
    uint16_t endLed = (uint16_t)(curr * numLeds);
    
    for (uint16_t i = startLed; i < endLed && i < numLeds; i++) {
        leds[i] = CRGB::White;
    }

    Serial.printf("%d",startLed);
}

void patternFire(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {

    auto fireColor = [](float t) -> CRGB {
        if (t <= 0.4f) {
            float factor = t / 0.4f;
            return CRGB(255, (uint8_t)(180 * (1.0f - factor)), 0);
        } else {
            float factor = (t - 0.4f) / 0.6f;
            return CRGB((uint8_t)(255 * (1.0f - factor)), (uint8_t)(20 * (1.0f - factor)), 0);
        }
    };

    fadeToBlackBy(leds, numLeds, 5);

    for (uint16_t i = 0; i < numLeds; i++) {
        if (random8() > 20) {
            continue; // ランダムにスキップして炎の揺らぎを表現
        }
        float baseHeat = (float)i / (float)numLeds;
        float randomOffset = ((float)random8(255) / 255.0f - 0.5f) * 0.4f; // -0.2 to +0.2
        float heat = constrain(baseHeat + randomOffset, 0.0f, 1.0f);
        leds[i] = fireColor(heat);
    }

}

void patternRGStripe(uint32_t tick, CRGB* leds, uint16_t numLeds, uint16_t animSpeed) {
    fadeToBlackBy(leds, numLeds, 80);

    // 赤と緑が交互に流れる
    int stripeWidth = 40; // ストライプの幅

    for (uint16_t i = 0; i < numLeds; i++) {
        int pos = (i + (tick / (animSpeed / 50))) % (stripeWidth * 2);
        int posInStripe = pos % stripeWidth;
        float t = (float)posInStripe / (float)stripeWidth;
        float brightness = smoothstep(0.0f, 1.0f, 1.0f - t); // 先頭1.0f → 終端0.0f
        
        CRGB color;
        if (pos < stripeWidth){
            leds[i] = CRGB::Red;
        } else {
            leds[i] = CRGB::Green;
        }
        leds[i].nscale8((uint8_t)(brightness * 255));
    }
}