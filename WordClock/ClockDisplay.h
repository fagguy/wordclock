#ifndef _CLOCKDISPLAY_H_
#define _CLOCKDISPLAY_H_

#include "FadeTransitionManager.h"
#include <NeoPixelBrightnessBus.h>
#include <unordered_set>
#include <vector>

class ClockDisplay {
  private:
    // Word addresses
    const uint8_t wordIts[3]        = {132, 133, 134};
    const uint8_t wordAbout[5]      = {136, 137, 138, 139, 140};
    const uint8_t wordMinTwenty[6]  = {131, 130, 129, 128, 127, 126};
    const uint8_t wordMinFive[4]    = {124, 123, 122, 121};
    const uint8_t wordMinTen[3]     = {108, 109, 110};
    const uint8_t wordA[1]          = {111};
    const uint8_t wordQuarter[7]    = {113, 114, 115, 116, 117, 118, 119};
    const uint8_t wordMinutes[7]    = {107, 106, 105, 104, 103, 102, 101};
    const uint8_t wordHalf[4]       = {100, 99, 98, 97};
    const uint8_t wordTo[2]         = {84, 85};
    const uint8_t wordPast[4]       = {86, 87, 88, 89};
    const uint8_t wordHourFive[4]   = {91, 92, 93, 94};
    const uint8_t wordHourOne[3]    = {83, 82, 81};
    const uint8_t wordHourTwo[3]    = {80, 79, 78};
    const uint8_t wordHourThree[5]  = {77, 76, 75, 74, 73};
    const uint8_t wordHourFour[4]   = {60, 61, 62, 63};
    const uint8_t wordHourSix[3]    = {64, 65, 66};
    const uint8_t wordHourSeven[5]  = {67, 68, 69, 70, 71};
    const uint8_t wordHourEight[5]  = {59, 58, 57, 56, 55};
    const uint8_t wordHourNine[4]   = {54, 53, 52, 51};
    const uint8_t wordHourTen[3]    = {50, 49, 48};
    const uint8_t wordHourEleven[6] = {36, 37, 38, 39, 40, 41};
    const uint8_t wordHourTwelve[6] = {42, 43, 44, 45, 46, 47};
    const uint8_t wordOClock[6]     = {35, 34, 33, 32, 31, 30};
    const uint8_t wordGood[4]       = {28, 27, 26, 25};
    const uint8_t wordMorning[7]    = {12, 13, 14, 15, 16, 17, 18};
    const uint8_t wordNight[5]      = {19, 20, 21, 22, 23};
    const uint8_t wordAfternoon[9]  = {11, 10, 9, 8, 7, 6, 5, 4, 3};
    const uint8_t symbolSunny       = 2;
    const uint8_t symbolCloudy      = 1;
    const uint8_t symbolRaining     = 0;

    // Colors
    const RgbColor magenta, yellow, lightGrey, darkCyan, black;

    // Timings
    const uint16_t maxFadeInDelay = 1000;
    const uint16_t maxFadeOutDelay = 1000;
    const uint16_t animationTime = 500;

    // LED parameters
    const uint16_t PixelCount = 144; // make sure to set this to the number of pixels in your strip
    const uint8_t PixelPin = 2; // make sure to set this to the correct pin, ignored for Esp8266

    NeoPixelBrightnessBus < NeoGrbFeature, Neo800KbpsMethod > strip;
    // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
    // There are other Esp8266 alternative methods that provide more pin options, but also have
    // other side effects.
    // for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods

    FadeTransitionManager transitionManager;

    std::unordered_set <uint8_t> currentPixels, nextPixels;

    void updateTimePixels(int hours, int minutes, bool isDay);
    void updateWeatherPixels(String weather, bool isDay);
    void addPixelToNextSet(uint8_t pixelIndex, const RgbColor& targetColor);
    std::vector<uint8_t> getCharactersNotInOtherSet(std::unordered_set<uint8_t>& referenceSet, std::unordered_set<uint8_t>& otherSet);

  public:
    ClockDisplay():
      magenta(HtmlColor(0xff00ff)),
      yellow(HtmlColor(0xffff00)),
      lightGrey(HtmlColor(0xcdcdcd)),
      darkCyan(HtmlColor(0x83d7ee)),
      black(HtmlColor(0x000000)),
      strip(PixelCount, PixelPin),
      transitionManager(strip) {
    };

    // begin() needs to be called after Serial.Begin(). See https://github.com/Makuna/NeoPixelBus/issues/119
    void begin();
    // Updates the display to the next state and schedules the transition animations
    void updateDisplay(int hours, int minutes, bool isDay, String weather);
    // Updates the transition animations
    void updateAnimations();
};
#endif
