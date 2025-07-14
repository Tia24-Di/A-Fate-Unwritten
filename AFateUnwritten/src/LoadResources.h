#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H

#include <string>
#include <stdexcept>
#include <SFML/Audio.hpp> 

// generic loader for most resources (Texture, Font, SoundBuffer, etc.)
template <typename T>
bool resourceLoader(T& resource, const std::string& path) {
    if (!resource.loadFromFile(path)) {
        throw std::runtime_error("Error loading resource from: " + path);
    }
    return true;
}

// specialized loader for sf::Music (uses openFromFile instead of loadFromFile)
inline bool resourceLoader(sf::Music& music, const std::string& path) {
    if (!music.openFromFile(path)) {
        throw std::runtime_error("Error opening music from: " + path);
    }
    return true;
}

#endif
