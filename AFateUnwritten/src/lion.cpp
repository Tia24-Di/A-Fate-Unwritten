#include "Lion.h"
#include "LionNarrative.h"

using namespace std;
using namespace sf;

// constructor: initialize Lion scene, load resources, prepare questions, and resize elements
Lion::Lion(RenderWindow& window) : Scene(window), finalScore(0) { 
    loadResources();
    initializeQuestions();
    resize();
}

// load backgrounds, dialogue triggers, and starting dialogues
void Lion::loadResources() {
    bgManager.setDefault("resources/images/lion_sad.png");
    LionNarrative::getBackgroundTriggers(bgManager);
    auto start = LionNarrative::getStartNarrative();
    lionDialogues.assign(start.begin(), start.end());
    loadCurrentDialogue();
}

// load current dialogue line into dialogue system
void Lion::loadCurrentDialogue() {
    if (currentDialogueIndex >= lionDialogues.size()) {
        return;
    }

    const auto& line = lionDialogues[currentDialogueIndex];
    lionDialogue = make_unique<Dialogue>(window, line.speaker, line.text);

    // set up background change triggers based on special keywords
    lionDialogue->setBackgroundChanges(bgManager.getTriggerMap());
    lionDialogue->setBackgroundChangeCallback([&](const string& key) {
        bgManager(key); 
    });
}

// handle dialogue, quiz, retry, and showing results
void Lion::update() {
    switch (currentState) {
        // handle initial dialogue before the quiz
        case CurrentState::PreQuizDialogue:
            if (lionDialogue) {
                lionDialogue->update();

                if (lionDialogue->isTextComplete()) {
                    currentDialogueIndex++;
                    if (currentDialogueIndex < lionDialogues.size()) {
                        loadCurrentDialogue();
                    }
                    else {
                        currentState = CurrentState::QuizActive;
                        questionHandler->reset();
                        questionHandler->loadCurrentQuestion();
                    }
                }
            }
            break;

        // handle quiz questions
        case CurrentState::QuizActive:
            questionHandler->update();
            gameScore.setScore(GameScore::LION, questionHandler->getScore());
            finalScore = gameScore.getScore(GameScore::LION);
            if (questionHandler->isComplete()) {
                // quiz failed: show retry dialogue
                if (!gameScore.Condition(GameScore::LION)) {
                    currentDialogueIndex = 0;
                    lionDialogues = LionNarrative::getRetryNarrative();
                    loadCurrentDialogue();
                    currentState = CurrentState::RetryDialogue;
                }
                else {
                    // quiz passed: show end dialogue
                    currentDialogueIndex = 0;
                    lionDialogues = LionNarrative::getGoodEndNarrative();
                    currentState = CurrentState::ShowingResults;
                }
            }
            break;

        // handle retry dialogue after failing the quiz
        case CurrentState::RetryDialogue:
            if (lionDialogue) {
                lionDialogue->update();
                if (lionDialogue->isTextComplete()) {
                    currentDialogueIndex++;
                    if (currentDialogueIndex < lionDialogues.size()) {
                        loadCurrentDialogue();
                    }
                    else {
                        currentState = CurrentState::QuizActive;
                        questionHandler->reset();
                        questionHandler->loadCurrentQuestion();
                    }
                }
            }
            break;

        case CurrentState::ShowingResults:
            // wait for Enter key to move to post-quiz dialogue
            if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) {
                enterPressed = true;
                currentState = CurrentState::PostQuizDialogue;
                loadCurrentDialogue();
            }
            break;
        
        // handle final dialogue after quiz
        case CurrentState::PostQuizDialogue:
            if (lionDialogue) {
                lionDialogue->update();

                if (lionDialogue->isTextComplete()) {
                    currentDialogueIndex++;
                    if (currentDialogueIndex < lionDialogues.size()) {
                        loadCurrentDialogue();
                    }
                }
            }
            break;
    }

    // track Enter key release to prevent skipping multiple states at once
    if (!Keyboard::isKeyPressed(Keyboard::Enter)) {
        enterPressed = false;
    }
}

// reset the Lion scene to its initial state
void Lion::reset() {
    bgManager.setDefault("resources/images/lion_sad.png");
    if (lionDialogue) {
        lionDialogue->reset();
    }

    currentDialogueIndex = 0;
    finalScore = 0;
    enterPressed = false;
    currentState = CurrentState::PreQuizDialogue;

    lionDialogues.clear();
    auto start = LionNarrative::getStartNarrative();
    lionDialogues.assign(start.begin(), start.end());
    loadCurrentDialogue();

    if (questionHandler) {
        questionHandler->reset();
        questionHandler->setComplete(false);
    }
}

// render background, dialogue, or quiz based on current state
void Lion::render() {
    window.clear();
    bgManager.draw(window);

    switch (currentState) {
    case CurrentState::ShowingResults:
        questionHandler->progressReport(Color(250, 180, 0, 180), 1);
        break;
    case CurrentState::QuizActive:
        questionHandler->draw(window);
        break;
    default:
        if (lionDialogue) {
            lionDialogue->render();
        }
        break;
    }

    window.display();
}

// check if current dialogue text is fully typed and finished
bool Lion::isTextComplete() const {
    return lionDialogue && lionDialogue->isTextComplete();
}


// handle resizing the background, dialogue, and question UI
void Lion::resize() {
    bgManager.resize(window);  
    if (lionDialogue) {
        lionDialogue->resize(window);  
    }
    if (questionHandler) {
        questionHandler->resize();
    }
}

// initialize Lion-specific questions and configure question handler
void Lion::initializeQuestions() {
    lionQuestions = LionNarrative::getLionQuestions();
    questionHandler = make_unique<QuestionHandler>(window);
    questionHandler->setQuestions(lionQuestions);
    questionHandler->setButtonColor(LevelType::Lion); 
}

// check if the player has completed all quiz questions
bool Lion::areQuestionsComplete() const {
    return questionHandler && questionHandler->isComplete();
}
