#ifndef _FADETRANSITIONMANAGER_H_
#define _FADETRANSITIONMANAGER_H_

#include <NeoPixelBrightnessBus.h>
#include <NeoPixelAnimator.h>
#include <vector>
#include <queue>

class FadeTransitionManager
{
private:
  NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>& strip;
  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
  // There are other Esp8266 alternative methods that provide more pin options, but also have
  // other side effects.
  // for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods

  NeoPixelAnimator animations; // NeoPixel animation management object

  // what is stored for state is specific to the need, in this case, the colors.
  // Basically what ever you need inside the animation update function
  struct MyAnimationState
  {
      RgbColor StartingColor;
      RgbColor EndingColor;
  };

  // one entry per pixel to match the animation timing manager
  std::vector<MyAnimationState> animationState;

  struct FadeTransitionPixel {
    uint8_t ledIndex;
    RgbColor targetColor; 
    uint16_t delayTime, animationTime;

    FadeTransitionPixel(uint8_t ledIndex_, RgbColor targetColor_, uint16_t delayTime_, uint16_t animationTime_) :
      ledIndex(ledIndex_), targetColor(targetColor_), delayTime(delayTime_), animationTime(animationTime_) {}
    
    bool operator<(const FadeTransitionPixel& rhs) const {
      return delayTime < rhs.delayTime;
    }

    bool operator>(const FadeTransitionPixel& rhs) const {
      return delayTime > rhs.delayTime;
    }
  };

  std::priority_queue<FadeTransitionPixel, std::vector<FadeTransitionPixel>, std::greater<FadeTransitionPixel>> transitionPixels;

  unsigned long startTime;

public:
  FadeTransitionManager(NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>& strip_) : 
      strip(strip_),
      animations(strip_.PixelCount()),
      animationState(std::vector<MyAnimationState>(strip_.PixelCount())) {
  };

  void addPixel(uint8_t pixelIndex, const RgbColor& targetColor, uint16_t delayTime, uint16_t animationTime);
  void start();
  void updateAnimations();
};
#endif