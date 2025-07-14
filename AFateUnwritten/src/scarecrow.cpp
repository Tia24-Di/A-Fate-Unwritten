#include "Scarecrow.h"
#include "ScarecrowNarrative.h"

using namespace sf;
using namespace std;

// constructor: initialize Scarecrow scene, load resources, prepare questions, and resize elements
Scarecrow::Scarecrow(RenderWindow& window) : Scene(window), finalScore(0) {
    loadResources();
    initializeQuestions();
    resize();
}

void Scarecrow::initializeQuestions() {
    scarecrowQuestions = ScarecrowNarrative::getScarecrowQuestions(); // retrieve question bank from narrative 
    questionHandler = make_unique<QuestionHandler>(window); // create new question handler
    questionHandler->setQuestions(scarecrowQuestions); // load question bank to question handler
    questionHandler->selectRandomQuestions(10); // select 10 random questions from the question bank
    questionHandler->setButtonColor(LevelType::Scarecrow); // set colour of question buttons 
}

void Scarecrow::loadResources() {
    bgManager.setDefault("resources/images/scarecrow_sad.png");
    ScarecrowNarrative::getBackgroundTriggers(bgManager); // retrieve the triggers that will register any background changes 

    auto start = ScarecrowNarrative::getStartNarrative(); // load initial dialogue (pre-quiz narrative)
    scarecrowDialogues.assign(start.begin(), start.end());

    loadCurrentDialogue();
}

void Scarecrow::loadCurrentDialogue() {
    if (currentDialogueIndex >= scarecrowDialogues.size()) return; // end of dialogue

    const auto& line = scarecrowDialogues[currentDialogueIndex]; // current dialogue line
    scarecrowDialogue = make_unique<Dialogue>(window, line.speaker, line.text); // create dialogue object , initialize who the speaker is and what they are saying 

    scarecrowDialogue->setBackgroundChanges(bgManager.getTriggerMap());
    scarecrowDialogue->setBackgroundChangeCallback([&](const string& key) {
        bgManager(key); // change background using operator()
    });
}

void Scarecrow::update() {
    switch (currentState) {

    case CurrentState::PreQuizDialogue:
        if (scarecrowDialogue) {
            scarecrowDialogue->update(); // handle pre-quiz dialogue logic

            if (scarecrowDialogue->isTextComplete()) { // if current dialogue line completes
                currentDialogueIndex++;  // then move to next line
                if (currentDialogueIndex < scarecrowDialogues.size()) { // if pre-quiz dialogue remain
                    loadCurrentDialogue(); // then load next line
                }
                else { // otherwise switch to quiz state when all pre-quiz dialogue is done
                    currentState = CurrentState::QuizActive;
                    questionHandler->reset(); // reset quiz state
                    questionHandler->loadCurrentQuestion(); // start first question
                }
            }
        }
        break;

    case CurrentState::QuizActive:
        questionHandler->update(); // perform quiz question handling (button clicks, scoring)

        gameScore.setScore(GameScore::SCARECROW, questionHandler->getScore());
        finalScore = gameScore.getScore(GameScore::SCARECROW);

        if (questionHandler->isComplete()) { // if quiz is complete
            currentState = CurrentState::ShowingResults; // then move to the next state - progress report
            currentDialogueIndex = 0; // mark beginning of the end narrative

            auto end = (gameScore.Condition(GameScore::SCARECROW))
                ? ScarecrowNarrative::getGoodEndNarrative()
                : ScarecrowNarrative::getBadEndNarrative();

            scarecrowDialogues = end; // load appropriate ending narrative 
            loadCurrentDialogue();
        }
        break;

    case CurrentState::ShowingResults:
        if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) { // if the user presses the enter key
            enterPressed = true;
            currentState = CurrentState::PostQuizDialogue; // then move to the next state - scarecrow good/bad ending
            loadCurrentDialogue();
        }
        break;

    case CurrentState::PostQuizDialogue:
        if (scarecrowDialogue) {
            scarecrowDialogue->update(); // handle post-quiz dialogue logic

            if (scarecrowDialogue->isTextComplete()) { // if current dialogue line completes
                currentDialogueIndex++;  // then move to next line
                if (currentDialogueIndex < scarecrowDialogues.size()) { // if post-quiz dialogue remain
                    loadCurrentDialogue(); // then load next line
                }
            }
        }
        break;
    }

    if (!Keyboard::isKeyPressed(Keyboard::Enter)) { // reset enter key state to allow dialogue to progress
        enterPressed = false;
    }
}

void Scarecrow::render() {
    window.clear();
    bgManager.draw(window);

    switch (currentState) {
    case CurrentState::ShowingResults:
        questionHandler->progressReport(Color(141, 97, 68, 180), 2); // draws progress report , initialize to scarecrow colour
        break;
    case CurrentState::QuizActive:
        questionHandler->draw(window); // draws elements ( question, answer options, score, next ) from the question handler onto the game window
        break;
    case CurrentState::PreQuizDialogue: // dialogue drawn at start of level
    case CurrentState::PostQuizDialogue:
        if (scarecrowDialogue) {
            scarecrowDialogue->render(); // draw dialogue after quiz has completed
        }
        break;
    }

    window.display();
}

bool Scarecrow::areQuestionsComplete() const {
    return questionHandler && questionHandler->isComplete();  // returns true if all quiz questions are answered
}

void Scarecrow::reset() {
    bgManager.setDefault("resources/images/scarecrow_sad.png");

    // reset scarecrow dialogue if it exists
    if (scarecrowDialogue) {
        scarecrowDialogue->reset();
    }
    // reset all state variables
    currentDialogueIndex = 0;
    finalScore = 0;
    enterPressed = false;
    currentState = CurrentState::PreQuizDialogue;

    // reload pre-quiz dialogue
    scarecrowDialogues.clear();
    auto start = ScarecrowNarrative::getStartNarrative();
    scarecrowDialogues.assign(start.begin(), start.end());
    loadCurrentDialogue();

    // reset question handler if it exists
    if (questionHandler) {
        questionHandler->reset();
        questionHandler->setComplete(false);
    }
}

bool Scarecrow::isTextComplete() const {
    return scarecrowDialogue && scarecrowDialogue->isTextComplete(); // returns true if the current dialogue is fully displayed
}

void Scarecrow::resize() {
    bgManager.resize(window); // resize background
    if (scarecrowDialogue) {
        scarecrowDialogue->resize(window); // resize dialogue elements
    }
    if (questionHandler) {
        questionHandler->resize(); // resize quiz elements
    }
}
