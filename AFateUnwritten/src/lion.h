#ifndef LION_H
#define LION_H

// SFML includes
#include <SFML/Graphics.hpp>

// standard library includes
#include <memory>
#include <vector>

// project includes
#include "Scene.h"
#include "Dialogue.h"
#include "BackgroundManager.h"
#include "GameScore.h"
#include "QuestionHandler.h"

// represents the Lion level scene: handles dialogue, quiz, and state progression
class Lion : public Scene<Lion> {
public:
    // -- constructor --
    Lion(sf::RenderWindow& window);

    // -- core game loop methods --
    void loadResources() override;            // loads resources for the scene (textures, dialogues, questions)
    void render() override;                   // handles rendering for the scene
    void update() override;                   // updates logic based on current state
    void reset() override;                    // resets the scene state to initial
    bool isTextComplete() const override;     // checks if all dialogue has been displayed

    // -- layout and quiz logic --
    void resize();                            // adjusts visuals for window resizing
    bool areQuestionsComplete() const;        // checks if all questions have been answered

private:
    // -- background rendering --
    sf::Texture bgTexture;                    // texture for background image
    sf::Sprite bgSprite;                      // sprite to display background
    BackgroundManager bgManager;              // handles background switching and resizing

    // -- scene state control --
    enum class CurrentState {
        PreQuizDialogue,                      // initial story dialogue before quiz
        QuizActive,                           // quiz is currently active
        RetryDialogue,                        // optional retry dialogue after failure
        ShowingResults,                       // displaying final quiz results
        PostQuizDialogue                      // concluding story dialogue after quiz
    };

    CurrentState currentState = CurrentState::PreQuizDialogue; // current state of the scene

    // -- user interaction flags --
    bool enterPressed = false;                  // tracks Enter key press

    // -- quiz data --
    std::vector<QuestionHandler::Question> lionQuestions;     // list of questions for the Lion level
    std::unique_ptr<QuestionHandler> questionHandler;         // manages question rendering & answer checking
    int finalScore;                                           // stores final quiz score
    GameScore& gameScore = GameScore::getInstance();          // singleton to store cumulative score

    // -- dialogue data --
    std::vector<Dialogue::DialogueLine> lionDialogues;        // stores dialogue lines for Lion level
    size_t currentDialogueIndex = 0;                          // tracks current line in dialogue
    std::unique_ptr<Dialogue> lionDialogue;                   // manages text rendering and progression

    // -- helper methods --
    void loadCurrentDialogue();                               // loads and updates the active dialogue segment
    void initializeQuestions();                               // loads questions into the question handler

};

#endif
