#include "PauseMenu.h"
#include "ResizeManager.h"
#include "LoadResources.h"
#include "ControlScreen.h"
#include <SFML/Window/Mouse.hpp>

using namespace std;
using namespace sf;

// constructor: initializes the menu, loads resources, and sets up the options
PauseMenu::PauseMenu(RenderWindow& window) : window(window), selectedItemIndex(0) {
    // load font and background texture for the pause menu
    resourceLoader(font, "resources/fonts/CALIFR.ttf");
    resourceLoader(pauseBackgroundTexture, "resources/images/pause.png");
    pauseBackgroundSprite.setTexture(pauseBackgroundTexture);

    resize();        // Resize the pause menu elements based on the window size
    setupOptions();  // Set up the options (buttons) for the pause menu
}

// setup the text options for the pause menu
void PauseMenu::setupOptions() {
    Vector2f scale = ResizeManager::getScale(window);
    string labels[4] = { "Resume", "Main Menu", "Controls", "Quit" };

    // create and position each menu option
    for (int i = 0; i < 4; ++i) {
        options[i].setFont(font);
        options[i].setString(labels[i]);

        float scaledSize = ResizeManager::scaleText(50.f, scale.y);                 
        options[i].setCharacterSize(static_cast<unsigned int>(scaledSize));

        options[i].setFillColor(Color::White);
        options[i].setStyle(Text::Bold);

        // set position based on the design resolution and then scale it
        Vector2f basePos(ResizeManager::BASE_RESOLUTION.x / 2.f, 250.f + i * 80.f);  
        Vector2f finalPos = ResizeManager::scalePosition(basePos, scale);
        options[i].setPosition(
            finalPos.x - options[i].getGlobalBounds().width / 2.f,
            finalPos.y
        );
    }
}

// resize the pause menu elements when the window is resized
void PauseMenu::resize() {
    Vector2f scale = ResizeManager::getScale(window);  
    pauseBackgroundSprite.setScale(scale);
    setupOptions();                                     
}

// draw the pause menu (background and options)
void PauseMenu::draw() {
    window.draw(pauseBackgroundSprite);     // draw the pause image

    for (const auto& option : options) {    // draw each option text
        window.draw(option);
    }

    window.display();                       // display the updated frame
}


// update the color of the options when the mouse hovers over them
void PauseMenu::updateOptions() {
    Vector2i mousePos = Mouse::getPosition(window);

    // check if the mouse is over any of the options
    for (int i = 0; i < 4; ++i) {
        if (options[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            options[i].setFillColor(Color(139, 0, 0));
        }
        else {
            options[i].setFillColor(Color::White);
        }
    }
}

// main loop for showing the pause menu and handling user input
PauseMenuChoice PauseMenu::show() {
    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                // close the game if the window is closed
                return PauseMenuChoice::Quit;
            if (event.type == Event::Resized) {
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));   // adjust the view
                resize();                            // scale the background properly                     
            }
            // close the PauseMenu if Escape is pressed
            if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Space)) {
                return PauseMenuChoice::Resume;
            }
        }

        updateOptions();  // update the option colors based on mouse position

        // check if the left mouse button is clicked
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);
            for (int i = 0; i < 4; ++i) {
                // if the mouse is over an option, handle the click
                if (options[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    switch (i) {
                        case 0: return PauseMenuChoice::Resume;
                        case 1: return PauseMenuChoice::MainMenu;
                        case 2: {
                            ControlScreen controls(window);
                            if (controls.show() == ControlResult::Back) {
                                resize();
                                break;
                            }
                            break;
                        }
                        case 3: return PauseMenuChoice::Quit;
                    }
                }
            }
        }
        draw(); // draw the updated pause menu
    }
    return PauseMenuChoice::Quit; // return quit choice if the window is closed
}