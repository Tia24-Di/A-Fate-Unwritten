#include "RetryScreen.h"
#include "LoadResources.h"
#include "GameScore.h"
#include "ResizeManager.h"

using namespace sf;
using namespace std;

// constructor: initializes fonts and text for retry options
RetryScreen::RetryScreen(RenderWindow& window) : window(window) {
    // load font resource for text rendering
    resourceLoader(font, "resources/fonts/CALIFR.ttf");

    // set up the main retry question text
    retryText.setFont(font);
    retryText.setFillColor(Color(139, 0, 0)); // set color to a darker shade of red
    retryText.setString("Do you want to try again?");
    retryText.setStyle(Text::Bold);

    // set up the "Watch Credits" text
    watchCreditsText.setFont(font);
    watchCreditsText.setString("Watch Credits");
    watchCreditsText.setStyle(Text::Bold | Text::Underlined); 

    // call resize to position and adjust text
    resize();
}

// resizes and repositions text and options based on window size
void RetryScreen::resize() {
    Vector2f scale = ResizeManager::getScale(window);

    // resize the retry question text
    float scaledSize = ResizeManager::scaleText(100.f, scale.y);
    retryText.setCharacterSize(static_cast<unsigned int>(scaledSize));

    // center the retry title 
    FloatRect textBounds = retryText.getLocalBounds();
    retryText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    retryText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f + 30.f);

    // set up the position and size for "Watch Credits" text
    float creditsSize = ResizeManager::scaleText(60.f, scale.y);
    watchCreditsText.setCharacterSize(static_cast<unsigned int>(creditsSize));
    FloatRect creditBounds = watchCreditsText.getLocalBounds();
    watchCreditsText.setOrigin(0, creditBounds.height);
    watchCreditsText.setPosition(40.f, window.getSize().y - creditsSize * 1.2f); // positioned at the bottom-left

    // set up the clickable options (Yes, No, Load)
    setUpOptions();
}

// sets up the option texts ("Yes", "No", "~>") and positions them on screen
void RetryScreen::setUpOptions() {
    Vector2f scale = ResizeManager::getScale(window);
    float screenWidth = static_cast<float>(window.getSize().x);
    float screenHeight = static_cast<float>(window.getSize().y);
    float size = ResizeManager::scaleText(70.f, scale.y);
    float centerY = screenHeight / 2.f + 30.f;

    // Labels and positions for the options 
    String labels[] = { "Yes", "No", "~>" }; 
    Vector2f positions[] = {
        { screenWidth / 2.f - 150.f, centerY },                 // Yes
        { screenWidth / 2.f + 150.f, centerY },                 // No
        { screenWidth - 40.f, screenHeight - size * 2.f }       // Load (bottom-right)
    };

    // iterate over each option and set its properties (font, size, position)
    for (int i = 0; i < 3; ++i) {
        optionTexts[i].setFont(font);
        optionTexts[i].setFillColor(Color::White);
        optionTexts[i].setString(labels[i]);

        float fontSize = (i == 2) ? size * 3.f : size; // larger size for arrow
        optionTexts[i].setCharacterSize(static_cast<unsigned int>(fontSize));
        optionTexts[i].setStyle(Text::Bold);

        // set the origin for text centering
        FloatRect bounds = optionTexts[i].getLocalBounds();
        optionTexts[i].setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        // arrow aligns from bottom-right corner
        if (i == 2) {
            optionTexts[i].setOrigin(bounds.width, bounds.height);
        }

        // set the position for the text based on predefined positions
        optionTexts[i].setPosition(positions[i]);
    }
}

// draws the retry screen including retry text, options, and "Watch Credits" text
void RetryScreen::draw() {
    window.clear();
    window.draw(retryText);
    for (auto& txt : optionTexts) window.draw(txt);
    window.draw(watchCreditsText);
    window.display();
}

// updates the hover effect for the options based on mouse position
void RetryScreen::updateOptions() {
    Vector2f mousePos = (Vector2f)Mouse::getPosition(window);

    // iterate through each option and check if the mouse is over it
    for (int i = 0; i < 3; ++i) {
        if (optionTexts[i].getGlobalBounds().contains(mousePos)) {
            // change color when hovering over option
            switch (i) {
                case 0: optionTexts[i].setFillColor(Color(0, 139, 0)); break;
                case 1: optionTexts[i].setFillColor(Color(139, 0, 0)); break;
                case 2: optionTexts[i].setFillColor(Color(139, 0, 0)); break;
            }
        }
        else {
            optionTexts[i].setFillColor(Color::White); // reset color when not hovering
        }
    }

    // update the color for hover effect on "Watch Credits"
    if (watchCreditsText.getGlobalBounds().contains(mousePos)) {
        watchCreditsText.setFillColor(Color(0, 139, 0));
    }
    else {
        watchCreditsText.setFillColor(Color(137, 207, 240));
    }
}

// displays the retry screen and handles user interactions
RetryResult RetryScreen::show() {
    resize();      // ensure screen elements are properly resized
    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                return RetryResult::Quit;  // if window is closed, quit the game

            if (event.type == Event::Resized) {
                // adjust the view and layout when the window is resized
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));
                resize();
            }
        }

        // update the hover effect for the options
        updateOptions();

        // handle mouse click events
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2f mousePos = (Vector2f)Mouse::getPosition(window);

            // clickable retry options
            for (int i = 0; i < 3; ++i) {
                if (optionTexts[i].getGlobalBounds().contains(mousePos)) {
                    switch (i) {
                        case 0: GameScore::getInstance().resetScore(GameScore::ScoreType::TOTALSCORE); 
                                return RetryResult::Restart;
                        case 1: return RetryResult::Quit;
                        case 2: return RetryResult::Load;
                    }
                }
            }

            // watch credits option, play credits video
            if (watchCreditsText.getGlobalBounds().contains(mousePos)) {
                system("start /wait \"\" \"resources/end_credits.mp4\"");
            }
        }
        draw();  // draw updated retry screen
    }
    return RetryResult::None;  // return None if no action is taken
}
