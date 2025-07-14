#include "TextManager.h"
#include <sstream>
#include "ResizeManager.h"
#include "LoadResources.h"

using namespace std;
using namespace sf;

// constructor: initializes the text manager, sets up resources, and prepares dialogue settings
TextManager::TextManager(RenderWindow& window, const string& text, const string& name)
    : window(window), fullText(text), currentIndex(0), textComplete(false) {
    loadResources();       // load font and set up the text object
    handleTextWrapping();  // handle text wrapping to avoid overflow
    setComponentSize();    // set component sizes based on window
}

// loads font and sets initial properties of text objects
void TextManager::loadResources() {
    resourceLoader(font, "resources/fonts/INFROMAN.ttf");
    dialogueText.setFont(font);                          // apply the loaded font to dialogue text
    setComponentSize();                                  // call to adjust component sizes
}

void TextManager::setComponentSize() {
    Vector2f scale = ResizeManager::getScale(window);    // get scaling factors based on window size
    float uniformScale = std::min(scale.x, scale.y);     // use the smallest scale for uniform resizing

    // Dialogue box setup
    dialogueBox.setSize(Vector2f(ResizeManager::BASE_RESOLUTION.x * scale.x, 200 * scale.y)); // adjust box size based on window size
    dialogueBox.setFillColor(Color(0, 0, 0, 180));      // set semi-transparent black background for the box
    dialogueBox.setPosition(0, window.getSize().y - dialogueBox.getSize().y);  // position at bottom of screen

    // Dialogue text setup
    dialogueText.setFont(font);
    dialogueText.setCharacterSize(ResizeManager::scaleText(40, uniformScale));
    dialogueText.setFillColor(Color::White);
    dialogueText.setPosition(ResizeManager::scalePosition(Vector2f(15.f, ResizeManager::BASE_RESOLUTION.y - 200.f), scale));
}

string TextManager::getFullDisplayedText() const {
    return wrappedText.substr(0, currentIndex);  // return the full visible text up to the current index
}

// draws dialogue box and text onto window
void TextManager::render() {
    window.draw(dialogueBox);    // draw background for dialogue box
    window.draw(dialogueText);   // draw the actual dialogue text
}

// returns whether entire text has been displayed or not
bool TextManager::isTextComplete() const {
    return textComplete;
}

bool TextManager::awaitingChunk() const {
    return awaitingNextChunk;
}

void TextManager::handleTextWrapping() {
    if (!originalText.empty())
        fullText = originalText;  // reset full text to original if available

    textChunks.clear();   // clear any existing chunks
    wrappedText.clear();  // reset wrapped text

    Vector2f scale = ResizeManager::getScale(window);                  // get scale for text wrapping
    float maxWidth = ResizeManager::BASE_RESOLUTION.x * scale.x - 30;  // maximum width for text wrapping
    float maxHeight = 200 * scale.y - 20;                              // maximum height for text box

    Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(dialogueText.getCharacterSize());  // set temp text to same font size

    stringstream ss(fullText);
    string word, currentLine, currentChunk;

    // Wrap text into chunks based on window size
    while (ss >> word) {
        string testLine = currentLine + word + " ";
        tempText.setString(testLine);                       // test the line length with the current word
        if (tempText.getLocalBounds().width <= maxWidth) {  // if the line fits, continue adding to it
            currentLine = testLine;
        }
        else {
            currentChunk += currentLine + "\n";  // add the line to the chunk
            wrappedText += currentLine + "\n";   // append to wrapped text
            currentLine = word + " ";            // start new line with the current word
        }

        tempText.setString(currentChunk + currentLine);
        if (tempText.getLocalBounds().height > maxHeight) {  // if the chunk exceeds max height, split it
            textChunks.push_back(currentChunk);
            currentChunk.clear();
        }
    }

    currentChunk += currentLine;         // add any remaining text
    wrappedText += currentLine;
    textChunks.push_back(currentChunk);  // store the last chunk
    fullText = wrappedText;              // update full text with wrapped text
}

void TextManager::typeText() {
    if (currentChunkIndex >= textChunks.size()) return;   // if no more chunks, exit

    string& chunk = textChunks[currentChunkIndex];        // get the current chunk to display

    if (currentIndex < chunk.size()) {        // if not all characters are displayed in the chunk
        currentText += chunk[currentIndex];   // add one character at a time
        dialogueText.setString(currentText);  // update the text object
        currentIndex++;                       // move to the next character
    }
    else {
        if (currentChunkIndex < textChunks.size() - 1) {  // if there are more chunks to go to
            awaitingNextChunk = true;                     // indicate next chunk is ready
        }
        else {
            awaitingFinalConfirm = true;                  // if last chunk, wait for final confirmation
        }
    }
}

void TextManager::fillBox() {
    currentText = textChunks[currentChunkIndex];      // immediately fill the box with current chunk
    dialogueText.setString(currentText);              // update text object with the chunk
    currentIndex = currentText.size();                // mark the chunk as fully displayed

    if (currentChunkIndex < textChunks.size() - 1) {  // if more chunks left, prepare for next chunk
        awaitingNextChunk = true;
    }

    if (currentChunkIndex >= textChunks.size()) textComplete = true;  // mark text complete if it's the last chunk
}

void TextManager::update() {
    // handle advancing to the next chunk (ENTER or RIGHT arrow key)
    bool enterPressedNow = Keyboard::isKeyPressed(Keyboard::Enter);

    if ((enterPressedNow && !enterPressed) || Keyboard::isKeyPressed(Keyboard::Right)) {
        if (awaitingNextChunk) {
            currentChunkIndex++;   // move to the next chunk
            currentText.clear();   // clear current text display
            currentIndex = 0;      // reset the index for typing
            awaitingNextChunk = false;
            dialogueText.setString("");  // clear dialogue text
        }
        else if (awaitingFinalConfirm) {
            textComplete = true;   // confirm text is complete
            awaitingFinalConfirm = false;
        }
        else if (enterPressedNow && !textComplete) {
            fillBox();  // fill the box with current chunk
        }
    }

    // update key states
    enterPressed = enterPressedNow;

    // type the text one character at a time
    if (!textComplete && !awaitingNextChunk) {
        typeText();
    }
}

void TextManager::reset() {
    currentIndex = 0;
    currentText.clear();          // clear current text
    textComplete = false;         // reset completion flag
    currentChunkIndex = 0;        // reset chunk index
    awaitingNextChunk = false;    // reset chunk awaiting flag
    enterPressed = false;         // reset Enter key press flag
    awaitingFinalConfirm = false; // reset final confirmation flag
    handleTextWrapping();         // re-wrap text for a fresh start
}

void TextManager::resize() {
    setComponentSize();          // adjust component size
    handleTextWrapping();        // re-wrap text based on new window size

    // try to resume from current chunk and index
    if (currentChunkIndex < textChunks.size()) {
        string& currentChunk = textChunks[currentChunkIndex];
        currentText = currentChunk.substr(0, currentIndex);  // restore typed text so far
        dialogueText.setString(currentText);                 // update text object
    }
}
