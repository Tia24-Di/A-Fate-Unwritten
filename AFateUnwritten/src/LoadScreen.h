#ifndef LOADSCREEN_H
#define LOADSCREEN_H

// SFML Includes
#include <SFML/Graphics.hpp>  

// use the SFML namespace for easier access to SFML classes
using namespace sf;

// enum to represent the possible choices in the load screen (chapters + back option)
enum class LoadChoice {
    Chapter1,     // chapter 1 selection
    Chapter2,     // chapter 2 selection
    Chapter3,     // chapter 3 selection
    Back          // go back to previous screen
};

// handles displaying the load screen and processing user input
class LoadScreen {
public:
    // -- constructor --
    explicit LoadScreen(sf::RenderWindow& window); // initializes the load screen with the game window reference

    // -- core functionality --
    LoadChoice show();     // displays the load screen and processes input for chapter selection
    void resize();         // ensure texts are resized

private:
    // -- UI elements --
    sf::RenderWindow& window;  // reference to the main game window for rendering
    sf::Font font;             // font used for title and option texts
    sf::Text loadText;         // text object to display the load screen title
    sf::Text options[4];       // array of text objects representing the load options (Chapter 1, 2, 3, Back)
    int selectedItemIndex;     // tracks the index of the currently selected option

    // -- helper methods --
    sf::Color getChapterColor(int index); // returns the color for each chapter option based on its index
    void draw();                          // draws the load screen, including the title and options
    void setUpOptions();                  // sets up the options (chapter selections)
    void updateOptions();                 // updates the selection highlight and handles input for changing selected option
};

#endif
