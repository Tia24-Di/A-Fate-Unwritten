#ifndef BUTTON_H
#define BUTTON_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <string>

// represents the level context for theming button highlights
enum class LevelType {
    Lion,
    Scarecrow,
    Tinman,
};

// handles: visual appearance, interaction logic, and selection state for buttons
class Button {
public:
    // -- constructor --
    Button(sf::RenderWindow& window, const std::string& label, sf::Font& font, sf::Vector2f position, sf::Vector2f size,
        bool nextButton = false, LevelType level = LevelType::Lion);

    // -- rendering --
    void draw(sf::RenderWindow& window);            // renders the button and its label

    // -- interaction & state --
    bool contains(sf::Vector2f point) const;        // checks if the mouse is over the button
    void setHover(bool isHovered);                  // updates appearance when hovered
    void select();                                  // applies selected highlight state
    void deselect();                                // removes selection and resets color

    // -- responsive UI support --
    void resize(sf::Vector2f basePosition, sf::Vector2f baseSize);  // adjusts layout on window resize

private:
    // -- window context  --
    sf::RenderWindow& window;       // reference to the game window

    // -- button handling --
    sf::Font font;                  // font used for button label
    std::string label;              // text label for the button
    sf::RectangleShape button;      // visual shape of the button
    sf::Text text;                  // label displayed on the button
    bool isSelected = false;        // whether the button is selected
    bool isNextButton;              // marks if this is a "Next" navigation button
    LevelType levelType;            // determines the highlight color by level context

    // -- color constants --
    static const sf::Color DEFAULT; // default button background color
    static const sf::Color NEXT;    // special highlight for "Next" button

    // -- helper methods --
    sf::Color getLevelColor() const;                           // resolves highlight color for the current level
    void centerText(sf::Vector2f position, sf::Vector2f size); // centers the label in the button
};

#endif