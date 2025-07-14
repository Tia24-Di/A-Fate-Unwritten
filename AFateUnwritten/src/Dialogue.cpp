#include "Dialogue.h"
#include "ResizeManager.h"
#include "LoadResources.h"

#include <memory>

using namespace std;
using namespace sf;

// constructor: initialize dialogue with character's text
Dialogue::Dialogue(RenderWindow &window, DialogueType characterType, const string &text)
    : window(window), characterType(characterType) {
    loadResources();                      // loads necessary resources like fonts
    loadCharacterInfo(characterType);     // loads character-specific dialogue info
    textManager = make_unique<TextManager>(window, text, characterInfo.name);  // initialize the text manager with the current character's name
    setComponentSize();  // set the size and position of UI components like name tag and text
}

// sets the size and position of name tag and dialogue components
void Dialogue::setComponentSize() {
    auto scale = ResizeManager::getScale(window);  // get scale factors based on window size

    // ensure that character name is populated for the dialogue
    characterName = characterInfo.name;

    if (!characterName.empty() && characterName != "Narrator") {
        // set up nameTag (the bold label for the character's name)
        nameTag.setFont(font);
        nameTag.setString(characterName);
        nameTag.setFillColor(Color::White);
        nameTag.setCharacterSize(static_cast<unsigned>(55.f * scale.y));
        nameTag.setStyle(Text::Bold);

        // set up nameText (same as nameTag for optional subtitle)
        nameText.setFont(font);
        nameText.setString(characterName);
        nameText.setFillColor(Color::White);
        nameText.setCharacterSize(static_cast<unsigned>(55.f * scale.y));

        // background rectangle for name tag (with some padding)
        float bgWidth = nameText.getLocalBounds().width + 20.f * scale.x;
        float bgHeight = 50.f * scale.y;
        nameBackground.setSize(Vector2f(bgWidth, bgHeight));
        nameBackground.setFillColor(characterInfo.tagColour);

        // positioning the background near the bottom-left of the screen
        float x = 10.f * scale.x;
        float y = window.getSize().y - 150.f * scale.y - bgHeight - 53.f * scale.y;
        nameBackground.setPosition(x, y);

        // center the text inside the tag
        FloatRect textBounds = nameText.getLocalBounds();
        nameText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        nameTag.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);

        // set text position to be exactly in the middle of the tag
        nameText.setPosition(nameBackground.getPosition() + nameBackground.getSize() / 2.f);
        nameTag.setPosition(nameBackground.getPosition() + nameBackground.getSize() / 2.f);
    }
}


// loads necessary resources like fonts for displaying text
void Dialogue::loadResources() {
    resourceLoader(font, "resources/fonts/INFROMAN.ttf");
}

// checks if the text is still being typed (waiting for the next chunk of text)
bool Dialogue::awaitingChunk() const {
    return textManager->awaitingChunk();  // ask the text manager if it's waiting for more text
}

// loads information about character's dialogue style (name, colors, etc.)
void Dialogue::loadCharacterInfo(DialogueType characterType) {
    static unordered_map<DialogueType, CharacterInfo> characterMap = {
        {DialogueType::Narrator, {"Narrator", Color::White, Color::Black, Color::White}},
        {DialogueType::Dorothy, {"Dorothy", Color::White, Color(137, 207, 240), Color::White}},
        {DialogueType::Lion, {"Lion", Color::White, Color(250, 180, 0), Color::White}},
        {DialogueType::Scarecrow, {"Scarecrow", Color::White, Color(141, 97, 68), Color::White}},
        {DialogueType::Tinman, {"Tinman", Color::White, Color(192, 192, 192), Color::White}},
        {DialogueType::Wizard, {"Wizard", Color::White, Color(53, 6, 62), Color::White}}
    };

    characterInfo = characterMap[characterType];  // set character-specific details
}

// renders character's name tag and dialogue text on the window
void Dialogue::render() {
    // if it's not the narrator, draw the name background and the character's name text
    if (characterType != DialogueType::Narrator) {
        window.draw(nameBackground);      // draw the name tag background
        window.draw(nameText);            // draw the character's name
    }
    textManager->render();                // render the dialogue text below the name tag
}

// updates the dialogue state (typing animation and background check)
void Dialogue::update() {
    textManager->update();                // update the text manager (handles typing effect)
    checkBackgroundChange();              // check if a background change is triggered
}

// checks if any background change phrases have been triggered
void Dialogue::checkBackgroundChange() {
    string fullDisplayed = textManager->getFullDisplayedText();  // get all the text that has been displayed so far
    for (const auto &entry : backgroundChanges) {
        const string &phrase = entry.first;  // phrase that triggers the background change
        const string &image = entry.second;  // corresponding background image

        if (fullDisplayed.find(phrase) != string::npos && triggeredPhrases.find(phrase) == triggeredPhrases.end()) {
            triggeredPhrases.insert(phrase); // mark the phrase as triggered
            if (onBackgroundChange) {
                onBackgroundChange(phrase);  // call the callback function to change the background
            }
            break;
        }
    }
}

// resets the dialogue system (reuse in different scenes)
void Dialogue::reset() {
    textManager->reset();          // reset the text manager to start fresh
    setComponentSize();            // recalculate size and position of name tag and text
    lastAppliedText = "";          // reset displayed text
    displayedText = "";            // reset text for the next sequence
    triggeredPhrases.clear();      // clear all triggered background phrases
}

// checks if the text is completely displayed
bool Dialogue::isTextComplete() const {
    return textManager->isTextComplete() && !textManager->awaitingChunk();  // check if all text is displayed
}

// sets the background change callback function
void Dialogue::setBackgroundChangeCallback(const function<void(const string&)> &callback) {
    onBackgroundChange = callback; // store the callback function
}

// sets the background change phrases (when certain phrases are displayed, change the background)
void Dialogue::setBackgroundChanges(const unordered_map<string, string> &changes) {
    backgroundChanges = changes;   // set the phrases and corresponding background images
}

// resizes components based on the window size
void Dialogue::resize(RenderWindow &window) {
    textManager->resize();         // resize the text manager (handles all text resizing logic)
    setComponentSize();            // recalculate size and position of name tag and text
}