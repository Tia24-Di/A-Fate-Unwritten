#ifndef SCENE_H
#define SCENE_H

// SFML Includes
#include <SFML/Graphics.hpp>
#include <memory>

// project includes
#include "Dialogue.h"

// template class representing a generic scene
// T is the type of the scene (e.g., Intro, Lion, etc.)
template <typename T>
class Scene {
public:
    // -- constructor --
    Scene(sf::RenderWindow& window) : window(window) {}  // constructor that initializes the window reference

    // -- pure virtual functions --
    virtual void loadResources() = 0;                // pure virtual function to load resources for the scene (textures, sounds, etc.)
    virtual void render() = 0;                       // pure virtual function to render the scene (e.g., draw the background and dialogue)
    virtual void update() = 0;                       // pure virtual function to update the scene (e.g., handle input, update dialogue)
    virtual void reset() = 0;                        // pure virtual function to reset the scene, useful for replays or retries
    virtual bool isTextComplete() const = 0;         // pure virtual function to check if all dialogue text has been fully displayed
protected:
    sf::RenderWindow& window;                       // reference to the game window
    std::unique_ptr<Dialogue> currentDialogue;      // unique pointer to hold the current dialogue for the scene
};

#endif
