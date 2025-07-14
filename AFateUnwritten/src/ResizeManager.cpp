#include "ResizeManager.h"  

using namespace sf;         

// define the game's base resolution (designed for 1408 x 728 windows by default)
const Vector2f ResizeManager::BASE_RESOLUTION(1408.f, 728.f);

// figures out how much everything should be scaled to fit the current window size
Vector2f ResizeManager::getScale(const RenderWindow& window) {
    Vector2u size = window.getSize();                    // grab the actual pixel size of the window
    return {
        static_cast<float>(size.x) / BASE_RESOLUTION.x,  // calculate horizontal scale factor
        static_cast<float>(size.y) / BASE_RESOLUTION.y   // calculate vertical scale factor
    };
}

// takes a position and applies the scaling factor so it fits the resized window
Vector2f ResizeManager::scalePosition(Vector2f pos, Vector2f scale) {
    return { pos.x * scale.x, pos.y * scale.y };         // multiply both x and y by the scale to adjust position
}

// takes a base font size and scales it so text looks correct after resizing
float ResizeManager::scaleText(float baseSize, float scaleY) {
    return baseSize * scaleY;                            // adjust the text size proportionally to the window's height scale
}
