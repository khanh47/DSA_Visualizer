#pragma once

#include <string>

namespace UI {

class PlaybackSpeed {
public:
    PlaybackSpeed(float minSpeed = 0.25f, float maxSpeed = 3.0f, float initialSpeed = 1.0f);

    float getValue() const;
    float getMinValue() const;
    float getMaxValue() const;

    void setValue(float value);
    void setFromRatio(float ratio);

    float toRatio() const;
    std::string toLabel() const;

private:
    float minValue;
    float maxValue;
    float value;
};

} // namespace UI
