#ifndef MENU_H
#define MENU_H

// SFML includes
#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "GifAnimator.h" 

// maximum number of items in the menu
#define MAX_NUMBER_OF_ITEMS 4

// responsible for displaying the main menu
class Menu : public Scene<Menu> { // inherits from Scene<Menu> to be a specialized scene
public:
    // -- constructor and destructor --
    Menu(sf::RenderWindow& window);          // constructor that initializes the window reference
    // -- public member functions --
    int getPressedItem() const { return selectedItemIndex; } // returns the index of the currently selected menu item
    void loadResources() override;           // loads necessary resources, like background image
    void render() override;                  // renders the menu to the window
    void update() override;                  // updates the menu, including handling user input
    void reset() override;                   // resets the menu (if needed)
    bool isTextComplete() const override;    // checks if the menu text display is complete
    void resize();                           // adjusts layout for different window sizes

private:
    // -- private member variables --
    int selectedItemIndex;                   // index of the currently selected menu item
    bool selectionMade = false;              // flag to track if an item has been selected
    sf::Texture backgroundTexture;           // texture for the menu background
    sf::Sprite backgroundSprite;             // sprite for the menu background
    sf::Font font;                           // font used for menu text
    sf::Text menu[MAX_NUMBER_OF_ITEMS];      // array of text objects for each menu item
    sf::Text title;                          // title text of the menu
    sf::Font titleFont;                      // font for the title text
    GifAnimator gifAnimator = GifAnimator("resources/home/home_screen_", 66); // gif animator for home screen animation
    bool playGif = false;                    // flag to indicate whether the gif is being played
    bool mousePreviouslyPressed = false;     // tracks if the mouse was pressed previously
};

#endif
