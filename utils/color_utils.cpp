//
// Created by anton on 05.05.2025.
//

#include "color_utils.h"
sf::Color HSLtoRGB(float h, float s, float l) {
    float c = (1 - std::fabs(2 * l - 1)) * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
    float m = l - c / 2;

    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    return sf::Color((r + m) * 255, (g + m) * 255, (b + m) * 255);
}