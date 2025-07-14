#ifndef DIALOGUE_H
#define DIALOGUE_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>

// project includes
#include "TextManager.h"

// handles: character dialogue, name tags, background change triggers
class Dialogue {   
public:
    // character types (for dialogue speaker identification)
    enum class DialogueType {
        Narrator, Dorothy, Lion, Scarecrow, Tinman, Wizard
    };

    // struct for individual dialogue lines
    struct DialogueLine {
        DialogueType speaker;
        std::string text;
    };
    // -- constructor --
    Dialogue(sf::RenderWindow& window, DialogueType characterType, const std::string& text);

    // -- core functionality --
    void render();                 // render name tag and text
    void update();                 // update typing effect and background trigger
    void reset();                  // reset dialogue state for reuse
    bool isTextComplete() const;   // check if all text has been displayed
    bool awaitingChunk() const;    // check if still typing current text

    // -- background change management --
    void setBackgroundChangeCallback(const std::function<void(const std::string&)>& callback);
    void setBackgroundChanges(const std::unordered_map<std::string, std::string>& changes);

    // -- responsive UI support --
    void resize(sf::RenderWindow& newWindow);

private:
    // -- rendering & display --
    sf::RenderWindow& window;                // main game window
    sf::Font font;                           // dialogue font
    sf::Text nameTag;                        // speaker name (styled bold)
    sf::Text nameText;                       // speaker name text
    sf::RectangleShape nameBackground;       // background for name tag
    sf::Text dialogueText;                   // main dialogue text

    // -- dialogue initialization --
    void loadResources();                                // load font and assets
    void loadCharacterInfo(DialogueType characterType);  // load speaker styles
    void setComponentSize();                             // size and position setup

    // -- name & layout --
    std::string characterName;
    const float baseNameSize = 55.f;
    const float nameTagHeight = 150.f;

    // -- dialogue flow & state --
    std::string displayedText;                // currently displayed chunk
    std::string lastAppliedText;              // last fully rendered text
    DialogueType characterType;               // current dialogue speaker

    // -- Character-Specific Styling --
    struct CharacterInfo {
        std::string name;
        sf::Color nameColour;
        sf::Color tagColour;
        sf::Color textColour;
    };
    CharacterInfo characterInfo;

    // -- text display manager --
    std::unique_ptr<TextManager> textManager;

    // -- background change system --
    std::unordered_map<std::string, std::string> backgroundChanges;  // phrase -> image path map
    std::unordered_set<std::string> triggeredPhrases;                // phrases already triggered
    std::function<void(const std::string&)> onBackgroundChange;      // callback for background swaps
    void checkBackgroundChange();

    // -- accessor --
    TextManager& getTextManager() const { return *textManager; }
};

#endif 
