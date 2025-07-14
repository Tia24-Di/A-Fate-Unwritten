#ifndef QUESTIONHANDLER_H
#define QUESTIONHANDLER_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

// project includes
#include "Button.h"

// represents the quiz/question system : handles question management/rendering , scoring , progress reports
class QuestionHandler {
public:
    // used to group question text, answer options, and correct index together
    struct Question {
        std::string question;               // question text
        std::vector<std::string> options;   // possible answers
        int correctOptionIndex;             // index of the correct answer
    };

    // -- constructor --
    QuestionHandler(sf::RenderWindow& window);

    // -- question initialization --
    void setQuestions(const std::vector<Question>& newQuestions);    // load set of questions
    void selectRandomQuestions(int count);                           // shuffle and resize questions

    // -- quiz operation --
    void reset();                                                    // reset quiz state
    void loadCurrentQuestion();                                      // load the current question and set up buttons
    void update();                                                   // handle quiz logic (button clicks, scores counter update)
    void draw(sf::RenderWindow& window);                             // render all elements
    void updateScoreText();                                          // update the score display

    // -- progress report --
    void progressReport(sf::Color c, int l);                         // generate progress report display
    
    // -- state accessors --
    bool isComplete() const { return _isComplete; }                  // check if quiz is complete
    void setComplete(bool complete) { _isComplete = complete; }      // set completion state

    // -- scoring --
    int getScore() const { return score; }                           // get current score

    // -- UI customization --
    void setButtonColor(LevelType type) { currentLevelType = type; } // set level-specific button colour
    void resize();

private:
    // -- UI elements --
    sf::Font font;                          // font used for text
    sf::Text questionText;                  // text for current question
    sf::RectangleShape questionBox;         // box to hold question
    sf::Texture backgroundTexture;          // background texture
    sf::Sprite backgroundSprite;            // sprite to display the background
    sf::Text scoreText;                     // text displaying the score
    std::string report;                     // progress report text
    sf::RectangleShape box;                 // box to hold progress report
    sf::Text progressText;                  // formatted progress text display
    std::vector<Button> buttons;            // vector for answer option buttons
    Button nextButton;                      // "Next" button to proceed to next question

    // -- quiz state --
    std::vector<Question> questions;        // vector for question bank
    int currentQuestionIndex = 0;           // keeps track of which question is active
    int selectedIndex = -1;                 // tracks selected answer
    int score = 0;                          // correct answer count
    int requiredScore[2] = { 4, 8 };        // scores needed to pass for each level type

    sf::RenderWindow& window;               // reference to game window
    bool _isComplete = false;               // tracks if all questions are answered
    LevelType currentLevelType;             // current level type (determines button colours)
    void updateReportLayout(sf::Color c);   // update progress report format

    // sound for button clicks
    sf::SoundBuffer buttonSound;
    sf::Sound button;

    // sound for choice selection
    sf::SoundBuffer choiceSound;
    sf::Sound choice;

    void loadSound(sf::SoundBuffer& soundBuffer, const std::string& filename, sf::Sound& sound);
};

#endif
