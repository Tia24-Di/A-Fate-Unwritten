#ifndef RESIZE_MANAGER_H
#define RESIZE_MANAGER_H

#include <SFML/Graphics.hpp>  

// handles scaling game objects to fit different window sizes
class ResizeManager {
public:
    static const sf::Vector2f BASE_RESOLUTION;                          // the default game resolution everything is designed around
    static sf::Vector2f getScale(const sf::RenderWindow& window);       // calculates how much to scale things to fit the current window
    static sf::Vector2f scalePosition(sf::Vector2f pos, sf::Vector2f scale); // scales a given position based on the window size
    static float scaleText(float baseSize, float scaleY);               // scales text size so it looks good no matter the resolution
};

#endif
