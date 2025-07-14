#ifndef BACKGROUND_MANAGER_H
#define BACKGROUND_MANAGER_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

// handles loading, displaying, and switching background images
// note: assumes valid window reference for lifetime of BackgroundManager usage
class BackgroundManager {
public:
    // -- constructor --
    BackgroundManager();

    // -- background management --
    void setDefault(const std::string& path);               // loads and shows a default background
    void draw(sf::RenderWindow& window);                    // draws the current background
    void operator()(const std::string& key);                // switches background based on key
    std::string& operator[](const std::string& key);        // sets or gets the file path for a key
    const std::unordered_map<std::string, std::string>& getTriggerMap() const; // returns the key->path map

    // -- responsive UI support --
    void resize(const sf::RenderWindow& window);            // resizes background to fit the window

private:
    // -- window context and association --
    sf::RenderWindow* window = nullptr;                     // pointer to the main window
    void setWindow(sf::RenderWindow& win);                  // sets the window for scaling

    // -- background assets --
    sf::Texture bgTexture;                                  // the current background image
    sf::Sprite bgSprite;                                    // sprite to display the texture

    // -- background key map --
    std::unordered_map<std::string, std::string> keyToPath; // map of string keys to file paths
    std::string currentKey = "";                            // keeps track of current background

    // -- helper methods --
    void resizeSprite();                                    // rescales the sprite to fit window
};

#endif 
