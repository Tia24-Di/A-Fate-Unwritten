#ifndef SCARECROW_H
#define SCARECROW_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <memory>

// project includes
#include "Dialogue.h"
#include "Scene.h"
#include "QuestionHandler.h"
#include "BackgroundManager.h"
#include "GameScore.h"

// represents the Scarecrow level scene: handles dialogue, quiz logic, and flow state
class Scarecrow : public Scene<Scarecrow> {
public:
    // -- constructor --
    Scarecrow(sf::RenderWindow& window);

    // -- core game loop methods --
    void loadResources() override;            // loads resources for the scene (textures, dialogues, questions)
    void render() override;                   // handles rendering for the scene
    void update() override;                   // updates logic based on current state
    void reset() override;                    // resets the scene state to initial
    bool isTextComplete() const override;     // checks if all dialogue has been displayed

    // -- layout and quiz logic --
    bool areQuestionsComplete() const;        // checks if all questions have been answered
    void resize();                            // adjusts visuals for window resizing

private:
    // -- background rendering --
    sf::Texture bgTexture;         // texture for background image
    sf::Sprite bgSprite;           // sprite to display background
    BackgroundManager bgManager;   // handles background switching and resizing

    // -- scene state control --
    enum class CurrentState {
        PreQuizDialogue,           // initial story dialogue before quiz
        QuizActive,                // quiz is currently active
        ShowingResults,            // displaying final quiz results
        PostQuizDialogue           // concluding story dialogue after quiz
    };

    CurrentState currentState = CurrentState::PreQuizDialogue; // current state of the scene

    // -- dialogue data --
    std::vector<Dialogue::DialogueLine> scarecrowDialogues;    // stores dialogue lines for Scarecrow level
    std::size_t currentDialogueIndex = 0;                      // tracks current line in dialogue
    std::unique_ptr<Dialogue> scarecrowDialogue;               // manages text rendering and progression

    // -- user interaction flags --
    bool waitingForInput;        // true if waiting for player to press Enter
    bool enterPressed = false;   // tracks Enter key press

    // -- quiz data --
    std::vector<QuestionHandler::Question> scarecrowQuestions; // list of questions for the Scarecrow level
    std::unique_ptr<QuestionHandler> questionHandler;          // manages question rendering & answer checking
    int finalScore;                                            // stores final quiz score
    GameScore& gameScore = GameScore::getInstance();           // singleton to store cumulative score

    // -- helper methods --
    void loadCurrentDialogue();                                // loads and updates the active dialogue segment
    void initializeQuestions();               // loads questions into the question handler
};

#endif
