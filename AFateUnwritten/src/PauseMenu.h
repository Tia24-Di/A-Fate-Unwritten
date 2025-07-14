#ifndef PAUSEMENU_H
#define PAUSEMENU_H

// SFML includes
#include <SFML/Graphics.hpp>

// enum representing the choices in the pause menu
enum class PauseMenuChoice {
    Resume,     // resume the game
    MainMenu,   // go to the main menu
    Controls,   // show the control settings
    Quit        // quit the game
};

// pauseMenu class for managing and interacting with the pause menu
class PauseMenu {
public:
    // -- constructor --
    PauseMenu(sf::RenderWindow& window); // constructor that initializes the window reference

    // -- member functions --
    PauseMenuChoice show();              // displays the pause menu and returns the selected option
    void resize();                       // adjusts the layout in case the window is resized

private:
    // -- member variables and functions --
    sf::RenderWindow& window;            // reference to the game window
    sf::Font font;                       // font for text rendering
    sf::Text options[4];                 // text objects for the menu options
    int selectedItemIndex;               // tracks which menu option is selected

    sf::Texture pauseBackgroundTexture;  // texture for the pause menu background
    sf::Sprite pauseBackgroundSprite;    // sprite to display the background

    void draw();                         // draws the pause menu to the screen
    void updateOptions();                // updates the menu options, like selection and navigation

    // -- private helper functions --
    void setupOptions();                 // initializes the menu options
};

#endif
