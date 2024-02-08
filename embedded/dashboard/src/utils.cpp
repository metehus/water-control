//
// Created by methi on 25/01/2024.
//

#include "utils.h"

float lerp(float range1_0, float range1_1, float range2_0, float range2_1, float value) {
    return ((value - range1_0) * (range2_1 - range2_0)) / (range1_1 - range1_0) + range2_0;
}
