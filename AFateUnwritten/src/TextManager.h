#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

// SFML & standard library includes
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// handles: dialogue text rendering, text animation, and wrapping for the game
class TextManager {
public:
    // -- constructor --
    // initializes text manager with window and text
    TextManager(sf::RenderWindow& window, const std::string& text, const std::string& name = "");

    // -- helper methods --
    std::string getFullDisplayedText() const; // returns the full visible text displayed so far

    // -- text handling methods --
    std::string wrappedText;             // used in handleTextWrapping to store wrapped text
    void loadResources();                // loads font and sets up text components
    void setComponentSize();             // sets sizes of the dialogue box and text components
    void update();                       // updates the text (one character at a time)
    void typeText();                     // adds one character at a time (simulates typing)
    void handleTextWrapping();           // handles text wrapping to avoid overflow
    void render();                       // renders text and dialogue box on the window
    bool isTextComplete() const;         // checks if all the text has been fully displayed

    // -- reset and advance --
    void reset();                        // resets the text manager state for reuse in another level
    void fillBox();                      // immediately fills the box with text up to the overflow point
    bool awaitingChunk() const;          // checks if the dialogue is awaiting the next chunk of text

    // -- responsive UI support --
    void resize();                       // resizes text and components when the window size changes

private:
    // -- window and component setup --
    sf::RenderWindow& window;            // main window object for rendering text and graphics
    sf::Font font;                       // sfml font object, defines the text appearance
    sf::RectangleShape dialogueBox;      // sfml rectangle shape for the background of the dialogue box
    sf::Text dialogueText;               // sfml text object holding the dialogue text
    sf::Text nameText;                   // text object for displaying the character's name
    sf::RectangleShape nameBackground;   // background rectangle for the character's name tag
    std::string fullText;                // full block of text for dialogue
    std::string currentText;             // currently visible text, revealed progressively
    std::string characterName;           // name of the character currently speaking
    std::string originalText;            // original text before any modifications (for wrapping)

    // -- flags --
    bool textComplete;                   // indicates if the entire text has been displayed
    bool awaitingNextChunk = false;      // flag indicating if the dialogue is waiting for the next chunk
    bool awaitingFinalConfirm = false;   // flag to indicate readiness for final confirmation

    // -- text chunk management --
    std::vector<std::string> textChunks; // stores chunks of the text after wrapping
    int currentChunkIndex = 0;           // tracks which chunk is currently being displayed
    bool enterPressed = false;           // flag to detect if the enter key is pressed
    bool rightPressed = false;           // flag to detect if the right arrow key is pressed
    bool readyForNextSpeaker = false;    // flag to indicate readiness for the next speaker's dialogue
    bool waitingForChunkAdvance = false; // indicates if the system is ready to move to the next chunk
    size_t currentIndex;                 // keeps track of where we are in the text

    // -- character typing simulation --
    size_t charIndex = 0;                // current character index being typed
    std::string displayedText;           // currently visible portion of the text (progressively revealed)
};

#endif
