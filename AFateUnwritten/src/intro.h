#ifndef INTRO_H
#define INTRO_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <memory>

// project includes
#include "Scene.h"
#include "Dialogue.h"
#include "BackgroundManager.h"

// handles: intro scene logic, dialogue flow, background management
class Intro : public Scene<Intro> {
public:
    // -- constructor --
    explicit Intro(sf::RenderWindow& window);  // sets up the intro scene with the game window

    // -- core functionality --
    void loadResources() override;         // loads textures, dialogue data, and other assets
    void render() override;                // draws background and dialogue elements
    void update() override;                // handles dialogue progression and input
    void reset() override;                 // resets the scene for replay or restart
    bool isTextComplete() const override;  // returns true if all dialogue is finished

    // -- responsive UI support --
    void resize();                         // adjusts layout on window size change

private:
    // -- background rendering --
    sf::Texture bgTexture;                 // background texture
    sf::Sprite bgSprite;                   // sprite for rendering the background
    BackgroundManager bgManager;           // handles background transitions

    // -- dialogue flow --
    std::vector<Dialogue::DialogueLine> introDialogues; // stores intro dialogue lines
    size_t currentDialogueIndex = 0;                    // tracks current dialogue position
    std::unique_ptr<Dialogue> introDialogue;            // handles active dialogue rendering

    // -- dialogue management --
    void loadCurrentDialogue();            // loads and prepares the current dialogue line
};

#endif
