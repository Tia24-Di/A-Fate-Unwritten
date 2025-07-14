#include "Outro.h"
#include "OutroNarrative.h"
#include "LoadResources.h"
#include "ResizeManager.h"
#include "GameScore.h"

#include <sstream> 

using namespace std;
using namespace sf;

// constructor: initializes Outro scene, load resources and resize elements
Outro::Outro(RenderWindow& window) : playGif(false), Scene(window) { 
    resourceLoader(font, "resources/fonts/INFROMAN.ttf");
    loadResources();
    resize();
}

// loads all necessary resources and sets up the outro
void Outro::loadResources() {
    // load background triggers from OutroNarrative
    OutroNarrative::getBackgroundTriggers(bgManager);

    // get scores and determine if player passed
    passed = gameScore.Condition(GameScore::TOTALSCORE);

    // load dialogues based on pass/fail
    if (passed) {
        outroDialogues = OutroNarrative::getGoodEndNarrative();

    }
    else {
        outroDialogues = OutroNarrative::getBadEndNarrative();
    }

    // set default background and overlay color
    c = passed ? Color(137, 207, 240, 180)
        : Color(53, 6, 62, 180);

    bgManager.setDefault(passed ? "resources/images/good_ending.png" : "resources/images/bad_ending.png");

    // load GIF only for bad ending
    if (!passed) {                  
        gifAnimator.loadFrames();
    }

    overlayText.setFont(font);

    // load first line of dialogue
    loadCurrentDialogue();          
}

// load the current dialogue line
void Outro::loadCurrentDialogue() {
    if (currentDialogueIndex >= outroDialogues.size()) {
        outroDialogue.reset();
        return;
    }
    const auto& line = outroDialogues[currentDialogueIndex];
    outroDialogue = make_unique<Dialogue>(window, line.speaker, line.text);

    // hook up background change triggers
    outroDialogue->setBackgroundChanges(bgManager.getTriggerMap());
    outroDialogue->setBackgroundChangeCallback([&](const string& key) {
        bgManager(key);
    });
}

// rendering
void Outro::render() {
    window.clear();

    if (showingResults) {
        bgManager.draw(window);
        progressReport(c);
    }
    else if (playGif) {
        gifAnimator.draw(window);
        window.draw(overlayText);
        for (auto& line : overlayLines) window.draw(line);

        if (gifAnimator.isAnimationComplete()) {
            window.draw(promptText);
        }
    }
    else if (outroDialogue) {
        bgManager.draw(window);
        outroDialogue->render();
    }
    window.display();
}

// main update loop
void Outro::update() {
    if (showingResults) {
        if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) {
            enterPressed = true;
            showingResults = false;
            loadCurrentDialogue();
        }
    }
    else if (playGif) {
        updateGifAnimation();
    }

    else if (outroDialogue) {
        outroDialogue->update();

        if (!triggerChecked && outroDialogue->isTextComplete()) {
            const auto& currentText = outroDialogues[currentDialogueIndex].text;
            if (currentText.find("unstoppable current") != string::npos && lastTriggeredIndex != currentDialogueIndex) {
                playGif = true;
                triggerChecked = true;
                lastTriggeredIndex = currentDialogueIndex;  // mark as triggered for this line
                return; // pause here for GIF
            }
        }

        if (outroDialogue->isTextComplete()) {
            if (!playGif) {
                currentDialogueIndex++;
                if (currentDialogueIndex < outroDialogues.size()) {
                    loadCurrentDialogue();
                }
                else {
                    outroDialogue.reset();  // end of dialogues
                    outroFinished = true;   // mark as finished when dialogues run out
                }
                return; //move on to next dialogue
            }

            if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) {
                enterPressed = true;
                currentDialogueIndex++;

                if (currentDialogueIndex < outroDialogues.size()) {
                    triggerChecked = false;
                    loadCurrentDialogue();
                }
                else {
                    outroDialogue.reset();  // end of dialogues  
                }
            }
        }
    }

    // reset enterPressed when key is released
    if (!Keyboard::isKeyPressed(Keyboard::Enter)) {
        enterPressed = false;
    }
}

// handle GIF animation updates
void Outro::updateGifAnimation() {
    gifAnimator.update();
    int currentFrame = gifAnimator.getCurrentFrameIndex();

    Vector2f scale = ResizeManager::getScale(window);
    float scaledTextSize = ResizeManager::scaleText(45, scale.y);

    auto centerLines = [&](vector<string> lines) {
        overlayLines.clear();
        float spacing = scaledTextSize * 1.2f;
        float startY = window.getSize().y - (100.f * scale.y + (lines.size() - 1) * spacing / 2.f);

        for (size_t i = 0; i < lines.size(); ++i) {
            Text t(lines[i], font, static_cast<unsigned int>(scaledTextSize));
            t.setFillColor(Color::Red);
            FloatRect bounds = t.getLocalBounds();
            t.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            t.setPosition(window.getSize().x / 2.f, startY + i * spacing);
            overlayLines.push_back(t);
        }
    };

    // show different text based on animation frame
    if (currentFrame >= 3 && currentFrame < 47) {
        centerLines({
            "In the fleeting instant before the darkness claims her,",
            "she's aware of the steady beep of the heart monitor",
            "and the desperate pleas of her loved ones, begging her to stay."
            });
    }
    else if (currentFrame >= 47) {
        centerLines({
            "Then, she's gone, slipping away into the eternal void,",
            "leaving behind only the echoes of her existence."
            });
    }
    else {
        overlayLines.clear();
    }

    // once GIF is done, tell the player to "Press Enter"
    if (gifAnimator.isAnimationComplete()) {
        FloatRect bounds = promptText.getLocalBounds();
        promptText.setFont(font);
        promptText.setFillColor(Color::White);
        promptText.setString("[Press Enter]");

        promptText.setCharacterSize(static_cast<unsigned int>(scaledTextSize));
        promptText.setOrigin(bounds.width, bounds.height); // origin at bottom-right corner of the text

        // position near bottom right, with a small margin 
        promptText.setPosition(
            window.getSize().x - 40.f * scale.x,
            window.getSize().y - 40.f * scale.y
        );
        // wait for Enter input to mark outro as finished
        if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) {
            enterPressed = true;
            playGif = false;
            triggerChecked = false;
            currentDialogueIndex++;
            loadCurrentDialogue();
            outroFinished = true;
        }
    }
}

// update the layout for the progress report
void Outro::updateReportLayout() {
    Vector2f scale = ResizeManager::getScale(window);

    box.setSize(Vector2f(window.getSize().x * 0.75f, window.getSize().y * 0.7f)); // slightly less wide
    box.setOutlineColor(Color::White);
    box.setOutlineThickness(2 * scale.x);
    box.setFillColor(c); 

    box.setPosition(
        (window.getSize().x - box.getSize().x) / 2,
        (window.getSize().y - box.getSize().y) / 2
    );

    outroReport.setFont(font);
    outroReport.setFillColor(Color::White);
    float scaledTextSize = ResizeManager::scaleText(40, scale.y);
    outroReport.setCharacterSize(static_cast<unsigned int>(scaledTextSize));
    outroReport.setString(report);

    outroReport.setPosition(
        box.getPosition().x + 20 * scale.x,
        box.getPosition().y + 20 * scale.y
    );
}

// reset the outro scene
void Outro::reset() {
    outroDialogue->reset();
    playGif = false;
    triggerChecked = false;
    currentDialogueIndex = 0;
    showingResults = true;
}

// check if text is completely displayed
bool Outro::isTextComplete() const {
    return outroDialogue ? outroDialogue->isTextComplete() : true;
}

// handle window resizing
void Outro::resize() {
    bgManager.resize(window);
    if (outroDialogue) {
        outroDialogue->resize(window);
    }

    if (showingResults) {
        updateReportLayout(); // apply new positions when window changes
    }
}

// display final progress report
void Outro::progressReport(Color c) {
    Vector2f scale = ResizeManager::getScale(window);
    ostringstream ss;

    if (passed) {
        ss << "Bravo! You have passed the test!\n\n";
        ss << "You've answered " << gameScore.getScore(GameScore::TOTALSCORE) << " out of 30 correctly.\n";
        ss << "Needed to pass: 24.\n\n";
        ss << "Purification Progress: The wizard's power has now depleted!\n";
        ss << "The Wizard is now purified!\n";
        ss << "Well done, Adventurer!\n\n";
    }
    else {
        ss << "You have failed the test. It seems you were not cut out for this.\n\n";
        ss << "You've answered " << gameScore.getScore(GameScore::TOTALSCORE) << " out of 30 correctly.\n";
        ss << "Needed to pass: 24.\n\n";
        ss << "Purification Progress: You have failed your mission to purify the \n";
        ss << "Wizard and free Oz from his hold.\n";
        ss << "The Wizard remains all powerful.\n\n";
    }

    ss << "Press Enter to continue...";
    report = ss.str();

    updateReportLayout(); // apply size and position after setting text
    window.draw(box);
    window.draw(outroReport);
}