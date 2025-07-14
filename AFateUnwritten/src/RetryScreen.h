#ifndef RETRYSCREEN_H
#define RETRYSCREEN_H

#include <SFML/Graphics.hpp>

// enum to represent the possible results from the retry screen
enum class RetryResult {
    None,    // no action taken
    Restart, // restart the scene
    Quit,    // exit the game
    Load     // load a saved game
};

// handles displaying the retry screen and processing user input
class RetryScreen {
public:
    // -- constructor --
    RetryScreen(sf::RenderWindow& window); // initializes the retry screen with the game window reference

    // -- core functionality --
    RetryResult show();         // displays the retry screen and handles user input for the retry choices
    void resize();              // ensure texts are resized

private:
    // -- UI elements --
    sf::RenderWindow& window;   // reference to the main game window for rendering
    sf::Font font;              // font used for option texts
    sf::Text retryText;         // text object for the retry message 
    sf::Text watchCreditsText;  // text object for watch credits message
    sf::Text optionTexts[3];    // array of text objects representing the options (Yes, No, Load)

    // -- helper methods --
    void setUpOptions();        // sets up the options (Yes, No, Load)
    void updateOptions();       // updates the selection highlight and handles input for changing selected option
    void draw();                // draws the retry screen, including the title and options
};

#endif
