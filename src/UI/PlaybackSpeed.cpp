#include "PlaybackSpeed.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace UI {

PlaybackSpeed::PlaybackSpeed(float minSpeed, float maxSpeed, float initialSpeed)
    : minValue(minSpeed),
      maxValue(maxSpeed),
      value(initialSpeed) {
    if (maxValue < minValue) {
        std::swap(maxValue, minValue);
    }
    setValue(initialSpeed);
}

float PlaybackSpeed::getValue() const {
    return value;
}

float PlaybackSpeed::getMinValue() const {
    return minValue;
}

float PlaybackSpeed::getMaxValue() const {
    return maxValue;
}

void PlaybackSpeed::setValue(float newValue) {
    value = std::clamp(newValue, minValue, maxValue);
}

void PlaybackSpeed::setFromRatio(float ratio) {
    const float clampedRatio = std::clamp(ratio, 0.0f, 1.0f);
    setValue(minValue + clampedRatio * (maxValue - minValue));
}

float PlaybackSpeed::toRatio() const {
    const float span = maxValue - minValue;
    if (span <= 0.0f) {
        return 0.0f;
    }

    return std::clamp((value - minValue) / span, 0.0f, 1.0f);
}

std::string PlaybackSpeed::toLabel() const {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << value << "x";
    return output.str();
}

} // namespace UI
