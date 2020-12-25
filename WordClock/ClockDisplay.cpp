#include "ClockDisplay.h"

void ClockDisplay::updateDisplay(int hours, int minutes, bool isDay, String weather) {
  nextPixels.clear();
  updateTimePixels(hours, minutes, isDay);
  updateWeatherPixels(weather, isDay);

  // Fade out pixels not in use
  for (auto i: getCharactersNotInOtherSet(currentPixels, nextPixels)) {
    transitionManager.addPixel(i, black, random(maxFadeOutDelay), animationTime);
  }

  // Next set of pixels becomes the current set
  currentPixels = nextPixels;

  transitionManager.start();
}

void ClockDisplay::updateTimePixels(int hours, int minutes, bool isDay) {  
  for (auto i: wordIts) { addPixelToNextSet(i, magenta); } // It's

  if (minutes % 5 != 0) for (auto i: wordAbout) { addPixelToNextSet(i, magenta); } // About

  if ((minutes > 2 && minutes <= 7) || (minutes > 52 && minutes <= 57)) {
    for (auto i: wordMinFive) { addPixelToNextSet(i, magenta); } // Five
    for (auto i: wordMinutes) { addPixelToNextSet(i, magenta); } // Minutes
  }
  else if ((minutes > 7 && minutes <= 12) || (minutes > 47 && minutes <= 52)) {
    for (auto i: wordMinTen) { addPixelToNextSet(i, magenta); } // Ten
    for (auto i: wordMinutes) { addPixelToNextSet(i, magenta); } // Minutes
  }
  else if ((minutes > 12 && minutes <= 17) || (minutes > 42 && minutes <= 47)) {
    for (auto i: wordA) { addPixelToNextSet(i, magenta); } // A
    for (auto i: wordQuarter) { addPixelToNextSet(i, magenta); } // Quarter
  }
  else if ((minutes > 17 && minutes <= 22) || (minutes > 37 && minutes <= 42)) {
    for (auto i: wordMinTwenty) { addPixelToNextSet(i, magenta); } // Twenty
    for (auto i: wordMinutes) { addPixelToNextSet(i, magenta); } // Minutes
  }
  else if ((minutes > 22 && minutes <= 27) || (minutes > 32 && minutes <= 37)) {
    for (auto i: wordMinTwenty) { addPixelToNextSet(i, magenta); } // Twenty
    for (auto i: wordMinFive) { addPixelToNextSet(i, magenta); } // Five
    for (auto i: wordMinutes) { addPixelToNextSet(i, magenta); } // Minutes
  }
  else if (minutes > 27 && minutes <= 32) {
    for (auto i: wordHalf) { addPixelToNextSet(i, magenta); } // Half
  }
  else { // From --:58 to --:02
    for (auto i: wordOClock) { addPixelToNextSet(i, magenta); } // O'Clock
  }

  if (!isDay) {
    for (auto i: wordGood) { addPixelToNextSet(i, magenta); } // Good
    for (auto i: wordNight) { addPixelToNextSet(i, magenta); } // Night
  }
  else if (hours < 12) {
    for (auto i: wordGood) { addPixelToNextSet(i, magenta); } // Good
    for (auto i: wordMorning) { addPixelToNextSet(i, magenta); } // Morning
  }
  else if (hours < 17) {
    for (auto i: wordGood) { addPixelToNextSet(i, magenta); } // Good
    for (auto i: wordAfternoon) { addPixelToNextSet(i, magenta); } // Afternoon
  }

  if (minutes > 2 && minutes <= 32) {
    for (auto i: wordPast) { addPixelToNextSet(i, magenta); } // Past
  }
  else if (minutes > 32 && minutes <= 57) {
    for (auto i: wordTo) { addPixelToNextSet(i, magenta); } // To
  }

  if (minutes > 32) hours++;

  switch (hours % 12) {
    case 0:
      for (auto i: wordHourTwelve) { addPixelToNextSet(i, magenta); } break; // Twelve
    case 1:
      for (auto i: wordHourOne) { addPixelToNextSet(i, magenta); } break; // One
    case 2:
      for (auto i: wordHourTwo) { addPixelToNextSet(i, magenta); } break; // Two
    case 3:
      for (auto i: wordHourThree) { addPixelToNextSet(i, magenta); } break; // Three
    case 4:
      for (auto i: wordHourFour) { addPixelToNextSet(i, magenta); } break; // Four
    case 5:
      for (auto i: wordHourFive) { addPixelToNextSet(i, magenta); } break; // Five
    case 6:
      for (auto i: wordHourSix) { addPixelToNextSet(i, magenta); } break; // Six
    case 7:
      for (auto i: wordHourSeven) { addPixelToNextSet(i, magenta); } break; // Seven
    case 8:
      for (auto i: wordHourEight) { addPixelToNextSet(i, magenta); } break; // Eight
    case 9:
      for (auto i: wordHourNine) { addPixelToNextSet(i, magenta); } break; // Nine
    case 10:
      for (auto i: wordHourTen) { addPixelToNextSet(i, magenta); } break; // Ten
    case 11:
      for (auto i: wordHourEleven) { addPixelToNextSet(i, magenta); } break; // Eleven
  }
}

void ClockDisplay::updateWeatherPixels(String weather, bool isDay) {
  if (weather.indexOf("Rain") > -1 || weather.indexOf("Showers") > -1) {
    addPixelToNextSet(symbolRaining, darkCyan);
  }
  else if (weather.indexOf("Cloudy") > -1) {
    addPixelToNextSet(symbolCloudy, lightGrey);
  }
  else if (isDay){
    addPixelToNextSet(symbolSunny, yellow);
  }
}

void ClockDisplay::updateAnimations() {
  transitionManager.updateAnimations();
}

void ClockDisplay::addPixelToNextSet(uint8_t pixelIndex, const RgbColor& targetColor) {
  nextPixels.insert(pixelIndex);

  // Schedule this pixel to fade in if it is new
  if (currentPixels.find(pixelIndex) == currentPixels.end()) {
    transitionManager.addPixel(pixelIndex, targetColor, random(maxFadeInDelay), animationTime);
  }
}

std::vector<uint8_t> ClockDisplay::getCharactersNotInOtherSet(std::unordered_set<uint8_t>& referenceSet, std::unordered_set<uint8_t>& otherSet) {  
  std::vector<uint8_t> result;
  for (auto i: referenceSet) {
    if (otherSet.find(i) == otherSet.end()) {
      result.push_back(i);
    }
  }

  return result;
}
