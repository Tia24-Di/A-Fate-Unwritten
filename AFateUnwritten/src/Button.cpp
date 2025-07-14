#include "Button.h"
#include "ResizeManager.h"

using namespace std;
using namespace sf;

// static colour definitions for buttons
const Color Button::DEFAULT = Color(0, 0, 0, 180);        // semi-transparent black
const Color Button::NEXT = Color(139, 0, 0, 180);         // semi-transparent red for "Next" buttons

// constructor: sets up the button's shape, position, label, and appearance
Button::Button(RenderWindow& window, const string& label, Font& font,
    Vector2f position, Vector2f size, bool nextButton, LevelType level)
    : window(window), isNextButton(nextButton), levelType(level)
{
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(DEFAULT);

    text.setFont(font);
    text.setString(label);
    text.setFillColor(Color::White);

    // scale text size based on window resolution
    Vector2f scale = ResizeManager::getScale(window);
    float scaledTextSize = ResizeManager::scaleText(36, scale.y);
    text.setCharacterSize(static_cast<unsigned int>(scaledTextSize));

    centerText(position, size);
}

// draws the button and its text on the screen
void Button::draw(RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

// checks if a given point is inside the button's bounds (for mouse detection)
bool Button::contains(Vector2f point) const {
    return button.getGlobalBounds().contains(point);
}

// updates the button's appearance when hovered or selected
void Button::setHover(bool isHovered) {
    if (isNextButton) {
        button.setFillColor(isHovered || isSelected ? NEXT : DEFAULT);
    }
    else if (isHovered && !isSelected) {
        button.setFillColor(getLevelColor());
    }
    else if (!isSelected) {
        button.setFillColor(DEFAULT);
    }
}

// marks the button as selected and applies the appropriate highlight colour
void Button::select() {
    isSelected = true;
    button.setFillColor(isNextButton ? NEXT : getLevelColor());
}

// clears the selection state and restores the default colour
void Button::deselect() {
    isSelected = false;
    button.setFillColor(DEFAULT);
}

// returns the level-specific highlight colour based on LevelType
Color Button::getLevelColor() const {
    switch (levelType) {
    case LevelType::Lion:       return Color(250, 180, 0, 180);   // Gold
    case LevelType::Scarecrow:  return Color(141, 97, 68, 180);   // Brown
    case LevelType::Tinman:     return Color(192, 192, 192, 180); // Silver
    default:                    return DEFAULT;
    }
}

// rescales and repositions the button and text after a window resize
void Button::resize(Vector2f basePosition, Vector2f baseSize) {
    Vector2f scale = ResizeManager::getScale(window);
    Vector2f newSize = { baseSize.x * scale.x, baseSize.y * scale.y };
    Vector2f newPosition = { basePosition.x * scale.x, basePosition.y * scale.y };

    button.setSize(newSize);
    button.setPosition(newPosition);

    float scaledTextSize = ResizeManager::scaleText(36, scale.y);
    text.setCharacterSize(static_cast<unsigned int>(scaledTextSize));
    centerText(newPosition, newSize);
}

// calculates and sets the text position so it is centered within the button's shape
void Button::centerText(Vector2f position, Vector2f size) {
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        position.x + (size.x - textBounds.width) / 2 - textBounds.left,
        position.y + (size.y - textBounds.height) / 2 - textBounds.top
    );
}
