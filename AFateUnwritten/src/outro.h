#ifndef OUTRO_H
#define OUTRO_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <string>

// project includes
#include "Scene.h"
#include "Dialogue.h"
#include "BackgroundManager.h"
#include "GameScore.h"
#include "GifAnimator.h" 

// handles logic for the Outro scene, including dialogue, final score, and gif animation
class Outro : public Scene<Outro> {       // Outro uses the Scene template
public:
    // -- constructor --
    Outro(sf::RenderWindow& window);      // constructor takes window reference to display scene

    // -- core game loop methods --
    void loadResources() override;        // loads necessary resources (like background img)
    void render() override;               // rdraws intro, including bg and any text
    void update() override;               // updates dialogue, checking for scene transitions and user input
    void reset() override;                // resets intro, useful for reloading or restarting
    bool isTextComplete() const override; // checks if all dialogue text has been fully displayed

    // -- flag to mark end of outro --
    bool outroFinished = false;           // flag to check if the Outro scene is finished

    // -- responsive UI support --
    void resize();                        // updates positions and scales of ui elements when window is resized

    bool isPlayingGif() {                 // used in game to play gif sound specifically
        return playGif;  
    }

private:
    // -- background rendering --
    sf::Texture bgTexture;                // texture for bg img
    sf::Sprite bgSprite;                  // sprite to display bg img
    BackgroundManager bgManager;          // handles background transitions

    // -- score and progress report --
    GameScore& gameScore = GameScore::getInstance();  // access to global score singleton
    void progressReport(sf::Color c);                 // generates and displays the player's progress summary
    void updateReportLayout();                        // ensures layout of report is propottional to window  
    void updateGifAnimation();                        // show gif during bad ending

    sf::RectangleShape box;     // rectangle used as a container for the report text (ui box)
    sf::Text outroReport;       // text object for rendering the final report message
    std::string report;         // string storing the final report text
    sf::Font font;              // font used for rendering the outro text
    bool showingResults = true; // flag to control whether the progress report is visible
    sf::Color c;                // color used for the progress report display

    // -- user input and dialogue --
    bool waitingForInput;                      // true if waiting for player to press Enter
    bool enterPressed = false;                 // tracks Enter key press
    std::vector<Dialogue::DialogueLine> outroDialogues; // stores dialogue lines for Outro scene
    size_t currentDialogueIndex = 0;           // tracks current line in dialogue
    std::unique_ptr<Dialogue> outroDialogue;   // manages text rendering and progression
    void loadCurrentDialogue();                // helper function to load the next dialogue line

    // -- gif animation --
    bool triggerChecked = false;               // ensures the gif animation only triggers once when conditions are met
    GifAnimator gifAnimator = GifAnimator("resources/bad_end/frame", 75); // preconfigured animator for "bad ending" gif
    bool playGif;                              // flag to play gif animation if bad ending

    // -- overlay text --
    sf::Text overlayText;                      // text shown during animation
    std::vector<sf::Text> overlayLines;        // multiple overlay text lines
    sf::Text promptText;

    // -- score condition and ending variations --
    bool passed;                 // tracks which end game condition should be triggered
    bool isBadEnding = false;    // true if "bad ending" 
    int lastTriggeredIndex = -1; // helps sync overlay text and GIF frames
};

#endif