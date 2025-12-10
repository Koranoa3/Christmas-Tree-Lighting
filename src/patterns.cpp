#include "patterns.h"
#include "functions.cpp"

// 全LED白点灯（静的）
void patternSolidWhite(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fill_solid(leds, numLeds, CRGB::White);
}

// デバッグ用：最後のLEDを白、最後から2番目を灰色に点灯
void patternEndDebug(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fill_solid(leds, numLeds, CRGB::Black);
    leds[numLeds - 1] = CRGB::White;
    leds[numLeds - 2] = CRGB::Red;
}

// レインボー（虹色が流れる）
void patternRainbow(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    uint8_t hue = ((tick - uint16_t(impact * 10000)) / (animSpeed / 200)) % 256;
    fill_rainbow(leds, numLeds, hue, 7);
}

// チェイス（追いかけるパターン）
void patternChase(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fill_solid(leds, numLeds, CRGB::Black);
    uint16_t pos = (tick / 100) % numLeds;
    leds[pos] = CRGB::White;
}

// ブリージング（呼吸のように明滅）
void patternBreathing(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    uint8_t brightness = beatsin8(30, 0, 255, 0, tick / 4);
    fill_solid(leds, numLeds, CRGB::White);
    for (uint16_t i = 0; i < numLeds; i++)
    {
        leds[i].nscale8(brightness);
    }
}

// きらめき（ランダムに点滅）
void patternTwinkle(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fadeToBlackBy(leds, numLeds, 20);
    if (random8() < 255)
    {
        leds[random16(numLeds)] = CRGB::White;
    }
    if (impact == 1.0f)
        for (uint16_t i = 0; i < numLeds / 3; i++)
            leds[random16(numLeds)] = CRGB::White;
}

void patternWipe(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fadeToBlackBy(leds, numLeds, 120);

    static uint32_t additional_offset[16]; // impact>0.0fの場合。(tick-offset)を秒単位に変換し、1000ms以内のものはそれぞれ再生
    static uint8_t offset_index = 0;
    if (impact == 1.0f)
    {
        additional_offset[offset_index % 16] = tick;
        offset_index++;
    }

    if (impact == 0.0f)
    {
        // 通常のwipe再生（impact==0.0fの場合のループアニメーション）
        float t = (tick % animSpeed) / (float)animSpeed;
        float prev = linearstep(0.3f, 0.7f, t - 0.2f);
        float curr = linearstep(0.3f, 0.7f, t);

        uint16_t startLed = (uint16_t)(prev * numLeds);
        uint16_t endLed = (uint16_t)(curr * numLeds);

        for (uint16_t i = startLed; i < endLed && i < numLeds; i++)
        {
            leds[i] = CRGB::White;
        }
    }
    else
    {
        fadeToBlackBy(leds, numLeds, 80);

        // impact発動時の追加wipe再生（1000ms以内のオフセットをそれぞれ再生）
        for (uint8_t j = 0; j < 16; j++)
        {
            uint32_t offset = additional_offset[j];
            if (offset == 0)
                continue;

            uint32_t elapsed = (tick - offset) * 10;
            if (elapsed > animSpeed)
            {
                additional_offset[j] = 0; // 期限切れのオフセットをクリア
                continue;
            }

            float t_add = elapsed / (float)animSpeed;
            float prev_add = linearstep(0.3f, 0.7f, t_add);
            float curr_add = linearstep(0.3f, 0.7f, t_add + 0.2f);

            uint16_t startLed_add = (uint16_t)(prev_add * numLeds);
            uint16_t endLed_add = (uint16_t)(curr_add * numLeds);

            for (uint16_t i = startLed_add; i < endLed_add && i < numLeds; i++)
            {
                leds[i] = CRGB::White;
            }
        }
    }
}

void patternFire(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    static uint32_t fire_offset[16];
    static uint8_t fire_offset_index = 0;

    if (impact == 1.0f)
    {
        fire_offset[fire_offset_index % 16] = tick;
        fire_offset_index++;
    }

    if (impact == 0.0f)
    {
        auto fireColor = [](float t) -> CRGB
        {
            if (t <= 0.4f)
            {
                float factor = t / 0.4f;
                return CRGB(255, (uint8_t)(180 * (1.0f - factor)), 0);
            }
            else
            {
                float factor = (t - 0.4f) / 0.6f;
                return CRGB((uint8_t)(255 * (1.0f - factor)), (uint8_t)(20 * (1.0f - factor)), 0);
            }
        };

        fadeToBlackBy(leds, numLeds, 5);

        for (uint16_t i = 0; i < numLeds; i++)
        {
            if (random8() > 20)
            {
                continue; // ランダムにスキップして炎の揺らぎを表現
            }
            float baseHeat = (float)i / (float)numLeds;
            float randomOffset = ((float)random8(255) / 255.0f - 0.5f) * 0.4f; // -0.2 to +0.2
            float heat = constrain(baseHeat + randomOffset, 0.0f, 1.0f);
            leds[i] = fireColor(heat);
        }
    }
    else
    {
        fadeToBlackBy(leds, numLeds, 160);

        // impact発動時のwipeエフェクト（赤色）
        for (uint8_t j = 0; j < 16; j++)
        {
            uint32_t offset = fire_offset[j];
            if (offset == 0)
                continue;

            uint32_t elapsed = (tick - offset) * 10;
            if (elapsed > animSpeed)
            {
                fire_offset[j] = 0;
                continue;
            }

            float t_add = elapsed / (float)animSpeed;
            float prev_add = linearstep(0.3f, 0.7f, t_add);
            float curr_add = linearstep(0.3f, 0.7f, t_add + 0.2f);

            uint16_t startLed_add = (uint16_t)(prev_add * numLeds);
            uint16_t endLed_add = (uint16_t)(curr_add * numLeds);

            for (uint16_t i = startLed_add; i < endLed_add && i < numLeds; i++)
            {
                leds[i] = CRGB::Red;
            }
        }
    }
}

void patternIce(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    static uint32_t ice_offset[16];
    static uint8_t ice_offset_index = 0;

    if (impact == 1.0f)
    {
        ice_offset[ice_offset_index % 16] = tick;
        ice_offset_index++;
    }

    if (impact == 0.0f)
    {
        auto fireColor = [](float t) -> CRGB
        {
            if (t <= 0.4f)
            {
                float fc = 1.0f - t / 0.4f;
                return CRGB((uint8_t)(180 * fc), (uint8_t)(255 * fc), 255);
            }
            else
            {
                float fc = 1.0f - (t - 0.4f) / 0.6f;
                return CRGB(0, (uint8_t)(255 * fc), 255);
            }
        };

        fadeToBlackBy(leds, numLeds, 5);

        for (uint16_t i = 0; i < numLeds; i++)
        {
            if (random8() > 20)
            {
                continue; // ランダムにスキップして炎の揺らぎを表現
            }
            float baseHeat = (float)i / (float)numLeds;
            float randomOffset = ((float)random8(255) / 255.0f - 0.5f) * 0.4f; // -0.2 to +0.2
            float heat = constrain(baseHeat + randomOffset, 0.0f, 1.0f);
            leds[i] = fireColor(heat);
        }
    }
    else
    {
        fadeToBlackBy(leds, numLeds, 160);

        // impact発動時のwipeエフェクト（青色）
        for (uint8_t j = 0; j < 16; j++)
        {
            uint32_t offset = ice_offset[j];
            if (offset == 0)
                continue;

            uint32_t elapsed = (tick - offset) * 10;
            if (elapsed > animSpeed)
            {
                ice_offset[j] = 0;
                continue;
            }

            float t_add = elapsed / (float)animSpeed;
            float prev_add = linearstep(0.3f, 0.7f, t_add);
            float curr_add = linearstep(0.3f, 0.7f, t_add + 0.2f);

            uint16_t startLed_add = (uint16_t)(prev_add * numLeds);
            uint16_t endLed_add = (uint16_t)(curr_add * numLeds);

            for (uint16_t i = startLed_add; i < endLed_add && i < numLeds; i++)
            {
                leds[i] = CRGB::Blue;
            }
        }
    }
}

void patternRGStripe(uint32_t tick, CRGB *leds, uint16_t numLeds, uint16_t animSpeed, float impact)
{
    fadeToBlackBy(leds, numLeds, 80);

    // 赤と緑が交互に流れる
    int stripeWidth = 40; // ストライプの幅

    for (uint16_t i = 0; i < numLeds; i++)
    {
        int pos = (i + ((tick - uint16_t(impact * 10000)) / (animSpeed / 50))) % (stripeWidth * 2);
        int posInStripe = pos % stripeWidth;
        float t = (float)posInStripe / (float)stripeWidth;
        float brightness = smoothstep(0.0f, 1.0f, 1.0f - t); // 先頭1.0f → 終端0.0f

        CRGB color;
        if (pos < stripeWidth)
        {
            leds[i] = CRGB::Red;
        }
        else
        {
            leds[i] = CRGB::Green;
        }
        leds[i].nscale8((uint8_t)(brightness * 255));
    }
}