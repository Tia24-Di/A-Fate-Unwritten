#include "Menu.h"
#include "ResizeManager.h"
#include "LoadResources.h"

using namespace sf;

// constructor for the Menu class
Menu::Menu(RenderWindow& window) : Scene(window), selectedItemIndex(0) {
    // load fonts for title and menu items
    resourceLoader(titleFont, "resources/fonts/MTCORSVA.ttf");
    resourceLoader(font, "resources/fonts/CALIFR.ttf");

    // resize and position elements
    resize();

    // load and prepare animated background (GIF)
    gifAnimator.loadFrames();
    gifAnimator.setLooping(true);
}

// load resources specific to Menu (none needed here since GIF background is dynamic)
void Menu::loadResources() {
    // no static resources needed for Menu
}

// adjust sizes and positions based on window scaling
void Menu::resize() {
    Vector2f scale = ResizeManager::getScale(window);

    // -- title setup --
    title.setFont(titleFont);
    float scaledTitleSize = ResizeManager::scaleText(110.f, scale.y);
    title.setCharacterSize(static_cast<unsigned int>(scaledTitleSize));
    title.setFillColor(Color::White);
    title.setStyle(Text::Bold);
    title.setString("A Fate Unwritten");

    // center the title text
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);

    float centerY = ResizeManager::BASE_RESOLUTION.y / 2.f;

    // variables for vertical layout adjustments
    float globalShiftUp = -150.f;  // negative shifts everything higher
    float gapOffset = 80.f;        // distance between title and menu block

    // set title position
    title.setPosition(ResizeManager::scalePosition(
        Vector2f(ResizeManager::BASE_RESOLUTION.x / 2.f, centerY + globalShiftUp - gapOffset),
        scale
    ));

    // -- menu items setup --
    float scaledTextSize = ResizeManager::scaleText(60.f, scale.y);
    float spacing = 100.f;   // vertical space between menu items

    std::string labels[4] = { "Start", "Load", "Controls", "Close"};

    for (int i = 0; i < 4; i++) {
        menu[i].setFont(font);
        menu[i].setCharacterSize(static_cast<unsigned int>(scaledTextSize));
        menu[i].setString(labels[i]);

        // highlight the selected item, otherwise keep it white
        menu[i].setFillColor((selectedItemIndex == i) ? Color(139, 0, 0) : Color::White);
        
        // center menu text
        FloatRect bounds = menu[i].getLocalBounds();
        menu[i].setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        // position: relative to shifted center
        menu[i].setPosition(ResizeManager::scalePosition(
            Vector2f(ResizeManager::BASE_RESOLUTION.x / 2.f,
                centerY + globalShiftUp + gapOffset + i * spacing),
            scale
        ));
    }

    // -- background scaling --
    Vector2f windowSize(window.getSize());
    Vector2f textureSize(backgroundTexture.getSize());
    backgroundSprite.setScale(
        windowSize.x / textureSize.x,
        windowSize.y / textureSize.y
    );
}

// update the menu: handle mouse movement and clicks
void Menu::update() {
    gifAnimator.update(); // update background animation

    // get the mouse position relative to the window
    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // reset all menu items to default colour(white)
    for (int i = 0; i < 4; i++) {
        menu[i].setFillColor(Color::White);
        menu[i].setStyle(Text::Bold);
    }

    bool mousePressed = Mouse::isButtonPressed(Mouse::Left);

    // check if the mouse is hovering over any menu item
    for (int i = 0; i < 4; i++) {
        if (menu[i].getGlobalBounds().contains(mousePosF)) {

            // only highlight on hover
            menu[i].setFillColor(Color(139, 0, 0));
            // if mouse was clicked (not just hovering), set selected item
            if (mousePressed && !mousePreviouslyPressed) {
                selectedItemIndex = i;
                selectionMade = true;
            }
            break; // only allow one item to be highlighted
        }
    }

    // remember if mouse was pressed this frame
    mousePreviouslyPressed = mousePressed;
}

// draw everything on the screen
void Menu::render() {
    window.clear();

    // always draw the GIF, even if it's finished — so it loops.
    gifAnimator.draw(window);

    window.draw(title);

    // draw each menu option
    for (int i = 0; i < 4; i++) {
        window.draw(menu[i]);
    }
    window.display();
}

// reset the menu to initial state
void Menu::reset() {
    selectedItemIndex = -1;
    selectionMade = false;
    mousePreviouslyPressed = Mouse::isButtonPressed(Mouse::Left);  // set to current state on reset
}

// change the body to have it as marked "complete" if selection is made
bool Menu::isTextComplete() const {
    return selectionMade;
}
