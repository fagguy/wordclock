#include "FadeTransitionManager.h"

void FadeTransitionManager::addPixel(uint8_t pixelIndex, const RgbColor& targetColor, uint16_t delayTime, uint16_t animationTime) {
  transitionPixels.push(FadeTransitionPixel(pixelIndex, targetColor, delayTime, animationTime));
}

void FadeTransitionManager::start() {
  startTime = millis();
}

void FadeTransitionManager::updateAnimations() {
  // check if any animations need to be started
  if (!transitionPixels.empty() && millis() - startTime > transitionPixels.top().delayTime) {
    auto pixel = transitionPixels.top();

    animationState[pixel.ledIndex].StartingColor = strip.GetPixelColor(pixel.ledIndex);
    animationState[pixel.ledIndex].EndingColor = pixel.targetColor;

    animations.StartAnimation(pixel.ledIndex, pixel.animationTime,
      // simple blend function
      [&](const AnimationParam& param) {
        RgbColor updatedColor = RgbColor::LinearBlend(
          animationState[param.index].StartingColor,
          animationState[param.index].EndingColor,
          param.progress);

        // apply the color to the strip
        strip.SetPixelColor(param.index, updatedColor);
      }
    );

    transitionPixels.pop();
  }

  if (animations.IsAnimating()) {
    //Serial.println("Animating...");
    // the normal loop just needs these two to run the active animations
    animations.UpdateAnimations();
    strip.Show();
  }
}