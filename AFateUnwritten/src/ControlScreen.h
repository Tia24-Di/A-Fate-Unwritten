#ifndef CONTROL_SCREEN_H
#define CONTROL_SCREEN_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>

enum class ControlResult { Back };  // enum for handling control results

class ControlScreen {
public:
    // constructor: initializes ControlScreen with a reference to the window
    ControlScreen(sf::RenderWindow& window);

    // show the control screen and return the result (Back)
    ControlResult show();

    // resizes the screen elements based on the new window size
    void resize();

private:
    // -- window context and UI elements --
    sf::RenderWindow& window;          // reference to the game window
    sf::Font font;                     // font for text elements
    sf::Sprite backgroundSprite;       // sprite for background image
    sf::Texture backgroundTexture;     // texture for the background sprite
    sf::Text backText;                 // text displayed for the "Back" option

    // -- helper methods --
    void draw();                       // renders the control screen UI
    void update();                     // updates any dynamic UI elements (e.g., animations)
};

#endif 
