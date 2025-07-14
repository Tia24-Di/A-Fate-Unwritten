#include "ControlScreen.h"
#include "LoadResources.h"
#include "ResizeManager.h"

using namespace sf;

// constructor: initializes the control screen (loads background and font)
ControlScreen::ControlScreen(RenderWindow& window) : window(window) {
    // load image of controls
    resourceLoader(backgroundTexture, "resources/images/controls.png");
    backgroundSprite.setTexture(backgroundTexture);

    // set up "Back" text
    resourceLoader(font, "resources/fonts/CALIFR.ttf");
    backText.setFont(font);
    backText.setString("Back");
    backText.setStyle(Text::Bold);

    resize(); // apply initial scaling and positioning
}

// resizes background and text based on the window size
void ControlScreen::resize() {
    Vector2f scale = ResizeManager::getScale(window);
    backgroundSprite.setScale(scale);

    // scale text size relative to vertical scaling
    float scaledSize = ResizeManager::scaleText(40.f, scale.y);
    backText.setCharacterSize(static_cast<unsigned int>(scaledSize));

    // position "Back" text at bottom-right corner with a scaled margin
    float margin = ResizeManager::scaleText(20.f, scale.y);
    backText.setPosition(
        window.getSize().x - backText.getGlobalBounds().width - margin,
        window.getSize().y - backText.getGlobalBounds().height - margin
    );
}

// draws the control screen
void ControlScreen::draw() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(backText);
    window.display();
}

// updates the UI (changes "Back" button color when hovered)
void ControlScreen::update() {
    auto mousePos = Mouse::getPosition(window);

    if (backText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        backText.setFillColor(Color(139, 0, 0)); // dark red when hovered
    }
    else {
        backText.setFillColor(Color::Black);     // black when not hovered
    }

    backText.setStyle(Text::Bold);
}

// displays the control screen and handles user input
ControlResult ControlScreen::show() {
    Event event;

    while (window.isOpen()) {
        // handle all window events
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return ControlResult::Back;
            }
            if (event.type == Event::Resized) {
                // adjust view and UI when window is resized
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));
                resize();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                return ControlResult::Back;
        }

        update(); // update UI (hover effect)

        // handle mouse click on "Back" button
        if (Mouse::isButtonPressed(Mouse::Left)) {
            auto mousePos = Mouse::getPosition(window);
            if (backText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                return ControlResult::Back;
            }
        }

        draw(); // draw updated screen
    }

    // fallback in case window closes unexpectedly
    return ControlResult::Back;
}
