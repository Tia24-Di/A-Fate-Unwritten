#ifndef TINMAN_H
#define TINMAN_H

// SFML and Standard Library Includes
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <sstream>

// project includes
#include "Dialogue.h"
#include "Timer.h"
#include "Scene.h"
#include "BackgroundManager.h"
#include "QuestionHandler.h"
#include "GameScore.h"

// handles the Tinman level scene (dialogue, quiz, riddles, flow control)
class Tinman : public Scene<Tinman> {
public:
    // -- constructor --
    Tinman(sf::RenderWindow& window);

    // -- core scene methods --
    void loadResources() override;        // loads textures, dialogues, questions, etc.
    void render() override;               // renders background, dialogue, UI elements
    void update() override;               // updates scene state (dialogue, timer, questions)
    void reset() override;                // resets scene to its initial state
    void resize();                        // adjusts visuals when window size changes
    bool isTextComplete() const override; // checks if all dialogue text is finished

    // -- progress checkers --
    bool areQuestionsComplete() const;    // checks if all quiz questions have been answered
    bool areRiddlesComplete() const;      // checks if all riddles have been answered

private:
    // -- enum to control scene flow --
    enum class CurrentState {
        PreQuizDialogue,   // before quiz
        QuizActive,        // during quiz
        ShowingResults,    // after quiz
        PostQuizDialogue,  // post-quiz story
        RiddlesActive,     // riddle section active
        RiddlesComplete,   // riddles completed
        PreRiddleDialogue  // pre-riddle story dialogue
    };

    CurrentState currentState = CurrentState::PreQuizDialogue; // current scene state

    // -- background rendering --
    sf::Texture bgTexture;                 // background texture
    sf::Sprite bgSprite;                   // background sprite
    BackgroundManager bgManager;           // manages background scaling and switching

    // -- result display after quiz --
    sf::RectangleShape resultBox;          // box to display results
    sf::Font resultFont;                   // font for result text
    sf::Text resultText;                   // result text
    std::stringstream ss;                  // stringstream for formatting scores

    // -- input tracking --
    bool enterPressed = false;             // tracks if Enter key was pressed

    // -- dialogue system --
    std::vector<Dialogue::DialogueLine> tinmanDialogues; // Tinman's dialogues
    size_t currentDialogueIndex = 0;                     // current dialogue line index
    std::unique_ptr<Dialogue> tinmanDialogue;            // dialogue manager

    // -- question and riddle handling --
    std::vector<QuestionHandler::Question> tinmanQuestions; // quiz questions
    std::vector<QuestionHandler::Question> tinmanRiddles;   // riddle questions
    std::unique_ptr<QuestionHandler> questionHandler;       // quiz handler
    std::unique_ptr<QuestionHandler> riddleHandler;         // riddle handler

    // -- timer for riddles --
    Timer timer;                               // countdown timer for riddle section

    // -- scoring --
    int riddleScore = 0;                       // score from riddles
    int quizScore = 0;                         // score from quiz
    GameScore& gameScore = GameScore::getInstance(); // singleton for total game score

    // -- failure handling --
    sf::RectangleShape failureBox;             // failure message box
    sf::Text failureText;                      // text inside failure box
    sf::Font failureFont;                      // font for failure text
    bool showFailureBox = false;               // flag to display failure box

    // -- helper functions --
    void initializeHandlers();  // initialize question and riddle handlers
    void initializeUI();        // initialize UI elements

    void loadCurrentDialogue(); // load dialogue line based on current scene state
    void progressReport();      // report and update game score progress

    void updateQuiz();          // handle quiz updates
    void updateRiddles();       // handle riddle updates
    void updateResults();       // handle results display

};

#endif
