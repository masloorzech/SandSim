//
// Created by anton on 19.06.2025.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include <random>


namespace Config {
    constexpr int SLIDERS_WIDTH = 150;
    constexpr int SLIDERS_HEIGHT = 30;
    constexpr int PIXEL_SIZE = 3;
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 840;
    constexpr int MAP_WIDTH = 256;
    constexpr int MAP_HEIGHT = 256;
    constexpr int SLIDERS_X_OFFSET = 10;
}

inline std::minstd_rand fast_rng(std::random_device{}());

#endif //GLOBALS_H
