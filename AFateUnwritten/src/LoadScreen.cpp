#include "LoadScreen.h"
#include "LoadResources.h"
#include "ResizeManager.h"
#include <SFML/Window/Mouse.hpp>

using namespace sf;

// constructor: initialize the LoadScreen with window reference and set up initial text
LoadScreen::LoadScreen(RenderWindow& window) : window(window), selectedItemIndex(0) {
    resourceLoader(font, "resources/fonts/CALIFR.ttf"); 

    loadText.setFont(font);
    loadText.setString("Load Game"); 
    loadText.setFillColor(Color(137, 207, 240)); 
    loadText.setStyle(Text::Bold);

    resize(); // set positions and sizes based on window size
}

// handle resizing of the title and repositioning
void LoadScreen::resize() {
    Vector2f scale = ResizeManager::getScale(window);

    // scale and position the title "Load Game"
    float scaledSize = ResizeManager::scaleText(80.f, scale.y);
    loadText.setCharacterSize(static_cast<unsigned int>(scaledSize));

    FloatRect titleBounds = loadText.getLocalBounds();
    loadText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    loadText.setPosition(window.getSize().x / 2.f, window.getSize().y / 6.f);

    setUpOptions(); // adjust options below the title
}

// set up chapter and back options for loading a game
void LoadScreen::setUpOptions() {
    Vector2f scale = ResizeManager::getScale(window);
    float screenWidth = static_cast<float>(window.getSize().x);
    float startY = 250.f; // starting Y position
    float spacing = 80.f; // base spacing

    // labels for each selectable option
    String labels[] = {
        "Chapter 1: The Lion",
        "Chapter 2: The Scarecrow",
        "Chapter 3: The Tinman",
        "Back"
    };

    // set up each option
    float optionSpacing = 100.f; // spacing between options
    for (int i = 0; i < 4; ++i) {
        options[i].setFont(font);
        options[i].setString(labels[i]);
        options[i].setCharacterSize(static_cast<unsigned int>(50.f * scale.y));
        options[i].setStyle(Text::Bold);
        options[i].setPosition(
            window.getSize().x / 2.f - options[i].getGlobalBounds().width / 2.f, // centered horizontally
            (250.f + i * optionSpacing) * scale.y // vertical position with spacing
        );
    }
}

// draw the load screen (title + options)
void LoadScreen::draw() {
    window.clear();
    window.draw(loadText);
    for (const auto& option : options) {
        window.draw(option);
    }
    window.display();
}

// update the color of options depending on mouse hover
void LoadScreen::updateOptions() {
    Vector2f mousePos = (Vector2f)Mouse::getPosition(window);

    for (int i = 0; i < 4; ++i) {
        if (options[i].getGlobalBounds().contains(mousePos)) {
            options[i].setFillColor(Color(139, 0, 0));   // highlighted when hovered (dark red)
        }
        else {
            options[i].setFillColor(getChapterColor(i)); // default chapter color
        }
    }
}

// main loop to display load screen and handle user interaction
LoadChoice LoadScreen::show() {
    resize(); // make sure everything is scaled properly
    Event event;

    while (window.isOpen()) {
        // handle window events
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close(); // close window if requested

            if (event.type == Event::Resized) {
                // adjust view and UI if window is resized
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));
                resize();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                return LoadChoice::Back;
        }

        updateOptions(); // update hover highlights

        // check for mouse click
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2f mousePos = (Vector2f)Mouse::getPosition(window);

            for (int i = 0; i < 4; ++i) {
                if (options[i].getGlobalBounds().contains(mousePos)) {
                    // return appropriate choice based on selection
                    switch (i) {
                        case 0: return LoadChoice::Chapter1;
                        case 1: return LoadChoice::Chapter2;
                        case 2: return LoadChoice::Chapter3;
                        case 3: return LoadChoice::Back;
                    }
                }
            }
        }

        draw(); // render the screen
    }

    return LoadChoice::Back; // default return if window somehow closes
}

// get the default color for each chapter option
Color LoadScreen::getChapterColor(int index) {
    switch (index) {
        case 0: return Color(250, 180, 0);   // yellow-orange for Chapter 1 (Lion)
        case 1: return Color(141, 97, 68);   // brown for Chapter 2 (Scarecrow)
        case 2: return Color(192, 192, 192); // silver for Chapter 3 (Tinman)
        default: return Color::White;        // white for "Back"
    }
}
