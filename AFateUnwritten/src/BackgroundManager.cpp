#include "BackgroundManager.h"
#include "LoadResources.h"

using namespace std;
using namespace sf;

// constructor : nothing to set up yet
// standard practice does nothing now, makes possible future setup easier (and thought about object creation)
BackgroundManager::BackgroundManager() {}

// stores a reference to the render window so we can later scale backgrounds properly
void BackgroundManager::setWindow(RenderWindow& win) {
    window = &win;
}

// called when window gets resized – update the internal pointer and rescale background to fit new window size
void BackgroundManager::resize(const RenderWindow& win) {
    setWindow(const_cast<RenderWindow&>(win));            // cast away const so we can store a pointer
    resizeSprite();                                       // recalculate scale for new size
}

// recalculates the scale of the background sprite so it stretches (or shrinks) to fill the window
void BackgroundManager::resizeSprite() {
    if (!window) return; // make sure the window is set

    auto windowSize = window->getSize();                  // get current window size
    auto textureSize = bgTexture.getSize();               // get size of the loaded texture

    if (textureSize.x == 0 || textureSize.y == 0) return; // prevent divide-by-zero if nothing's loaded

    // calculate scaling factors for width and height
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    bgSprite.setScale(scaleX, scaleY);                    // apply the scaling
}

// loads a background image from a file path and sets it as the initial background
void BackgroundManager::setDefault(const string& path) {
    resourceLoader(bgTexture, path);                      // attempts to load a texture from a file path and applies it to the target texture
    bgSprite.setTexture(bgTexture);                       // attach the texture to the sprite
    resizeSprite();                                       // scale it to fit the window
}

// draws the current background sprite onto the provided render window
void BackgroundManager::draw(RenderWindow& window) {
    window.draw(bgSprite);
}

// allows external code to map a key to a background image file or retrieve the existing path for a key
string& BackgroundManager::operator[](const string& key) {
    return keyToPath[key];                                 // creates an empty value if the key doesn't exist
}

// switches the background image based on a given key, the key must already be mapped to a valid image path
void BackgroundManager::operator()(const string& key) {
    auto it = keyToPath.find(key);                               // look up the key in our map
    if (it == keyToPath.end()) {
        throw runtime_error("Background key not found: " + key); // if not found, throw error
    }
    const string& path = it->second; // get the file path associated with the key
    resourceLoader(bgTexture, path); // attempts to load a texture from a file path and applies it to the target texture
    bgSprite.setTexture(bgTexture);  // update the sprite with the new texture
    currentKey = key;                // store which key is active
    resizeSprite();                  // make sure it fills the window
}

// gives read-only access to all key/path pairs used for background switching
const unordered_map<string, string>& BackgroundManager::getTriggerMap() const {
    return keyToPath;
}