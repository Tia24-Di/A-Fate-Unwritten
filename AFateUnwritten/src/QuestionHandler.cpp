#include "QuestionHandler.h"
#include "LoadResources.h"
#include "ResizeManager.h"
#include <algorithm>
#include <random>
#include <sstream>  

using namespace sf;
using namespace std;

// constructor: initializes fonts and text for question and score texts, and loads button sound
QuestionHandler::QuestionHandler(RenderWindow& window)
    : window(window),
    nextButton(window, "Next", font, Vector2f(window.getSize().x - 150, window.getSize().y - 80), Vector2f(120, 60), true) {
    resourceLoader(font, "resources/fonts/INFROMAN.ttf");
    // set up question text
    questionText.setFont(font);
    questionText.setCharacterSize(42);
    questionText.setFillColor(Color::White);

    // set up score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, window.getSize().y - 40); // positioned bottom left
    updateScoreText();

    //  load the button sound once at start
    loadSound(buttonSound, "resources/music/buttonEffect.ogg", button);

    //  load the choice selection sound once at start
    loadSound(choiceSound, "resources/music/choiceSound.ogg", choice);

    // ensures button texts are centered initially
    resize();
}

void QuestionHandler::setQuestions(const vector<Question>& newQuestions) {
    questions = newQuestions; // initialize questions
}

void QuestionHandler::selectRandomQuestions(int count) {
    if (questions.empty()) return;

    // generate a random seed
    random_device rd;
    mt19937 g(rd()); // randomizer engine - Mersenne Twister

    // shuffle and select the first 'count' questions
    shuffle(questions.begin(), questions.end(), g);

    if (count < static_cast<int>(questions.size())) {
        questions.resize(count);
    }
}

// resets quiz to its initial state:
void QuestionHandler::reset() {
    currentQuestionIndex = 0;
    score = 0;
    selectedIndex = -1;
    _isComplete = false;
    loadCurrentQuestion(); // load the first question to start the quiz fresh
    updateScoreText(); // current question will be set to one and score to zero
}

void QuestionHandler::loadCurrentQuestion() {
    if (currentQuestionIndex >= static_cast<int>(questions.size())) { // if all questions are answered
        _isComplete = true; // then  mark quiz as complete
        return;
    }

    const Vector2f scale = ResizeManager::getScale(window);
    const auto& currentQuestion = questions[currentQuestionIndex]; // get reference to current question

    // set up question text 
    questionText.setString(currentQuestion.question);
    questionText.setCharacterSize(ResizeManager::scaleText(42, scale.y));

    // calculate question box dimensions with padding
    const float padding = 40 * scale.x;
    const auto bounds = questionText.getLocalBounds();

    // position the question box
    questionBox.setSize(Vector2f(bounds.width + padding, bounds.height + padding));
    questionBox.setPosition((window.getSize().x - questionBox.getSize().x) / 2, 20 * scale.y);
    questionBox.setFillColor(Color(137, 207, 240, 180));

    // position question text within the box:
    questionText.setPosition(
        questionBox.getPosition().x + 20 * scale.x,
        questionBox.getPosition().y + 20 * scale.y
    );

    buttons.clear();
    selectedIndex = -1;
    // calculate maximum button width needed for options
    float maxButtonWidth = 0;
    for (const auto& option : currentQuestion.options) {
        Text temp;
        temp.setFont(font);
        temp.setString(option);
        temp.setCharacterSize(ResizeManager::scaleText(36, scale.y));
        maxButtonWidth = max(maxButtonWidth, temp.getLocalBounds().width);
    }
    // set button dimensions with padding
    const float buttonWidth = maxButtonWidth + 40 * scale.x;
    const float buttonHeight = 90 * scale.y;
    const float centerX = (window.getSize().x - buttonWidth) / 2;

    // to center buttons between the bottom of question box and bottom of screen
    const float spacing = 20 * scale.y;  // space between buttons
    const float totalHeight = currentQuestion.options.size() * buttonHeight +
        (currentQuestion.options.size() - 1) * spacing;

    const float availableTop = questionBox.getPosition().y + questionBox.getSize().y;
    const float availableBottom = window.getSize().y;
    const float verticalSpace = availableBottom - availableTop;

    const float startY = availableTop + (verticalSpace - totalHeight) / 2;
    // create buttons for each answer option, add to button vector
    for (size_t i = 0; i < currentQuestion.options.size(); i++) {
        buttons.emplace_back(
            window,                     // reference to game window
            currentQuestion.options[i], // option text
            font,                      // font to use
            Vector2f(centerX, startY + i * (buttonHeight + spacing)), // position
            Vector2f(buttonWidth, buttonHeight), // size
            false,                    // initial hover state
            currentLevelType           // level-specific button colour
        );
    }
}

void QuestionHandler::update() {
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));
    bool mouseClicked = Mouse::isButtonPressed(Mouse::Left);

    // handle answer buttons
    for (size_t i = 0; i < buttons.size(); i++) {
        Button& button = buttons[i];

        if (button.contains(mousePos)) {
            button.setHover(true); // highlight button currently under mouse

            if (mouseClicked && selectedIndex != static_cast<int>(i)) { // only update if selecting a new button
                if (selectedIndex != -1) {
                    buttons[selectedIndex].deselect(); // deselect previous selection (if any)
                }
                selectedIndex = static_cast<int>(i); // update selection to clicked button i
                button.select(); // highlight new selection

                //  play click sound when selecting an answer
                choice.play();
            }
        }
        else {
            button.setHover(false); // reset hover state
        }
    }

    // handle next button
    if (nextButton.contains(mousePos)) {
        nextButton.setHover(true);

        if (mouseClicked && selectedIndex != -1) {
            // check answer and update score

            //  play click sound when pressing "Next"
            button.play();

            if (selectedIndex == questions[currentQuestionIndex].correctOptionIndex) {
                score++;
            }
            // go to next question
            currentQuestionIndex++;
            // update display with new score and question number
            updateScoreText();
            if (currentQuestionIndex < questions.size()) { // load new question , if questions remain
                loadCurrentQuestion();
            }
            else { // or complete if done
                _isComplete = true;
            }
            // reset selection
            selectedIndex = -1;
        }
    }
    else {
        nextButton.setHover(false);
    }
}

void QuestionHandler::draw(RenderWindow& window) {
    // draws the question, answer options, score , next button to the screen
    window.draw(backgroundSprite);
    window.draw(questionBox);
    window.draw(questionText);
    nextButton.draw(window);
    window.draw(scoreText);

    for (auto& button : buttons) {
        button.draw(window);
    }
}

void QuestionHandler::updateReportLayout(Color c) {
    Vector2f scale = ResizeManager::getScale(window);

    // set box size based on window, keeping it proportional
    box.setSize(Vector2f(window.getSize().x * 0.7f, window.getSize().y * 0.7f));
    box.setFillColor(c);
    box.setOutlineThickness(2 * scale.x);
    box.setOutlineColor(Color::White);

    // center the box
    box.setPosition(
        (window.getSize().x - box.getSize().x) / 2,
        (window.getSize().y - box.getSize().y) / 2
    );

    // set text properties with scaling
    progressText.setFont(font);
    progressText.setCharacterSize(ResizeManager::scaleText(40, scale.y));
    progressText.setFillColor(Color::White);
    progressText.setString(report);

    // position text with padding based on scale
    progressText.setPosition(
        box.getPosition().x + 20 * scale.x,
        box.getPosition().y + 20 * scale.y
    );
}

void QuestionHandler::progressReport(Color c, int l) {
    ostringstream reportStream;

    if (score >= requiredScore[l - 1]) {
        reportStream << "Congratulations, Brave Adventurer!\n\n";
        reportStream << "You've answered " << score << " out of " << questions.size() << " correctly\n";
        reportStream << "Needed to pass: " << requiredScore[l - 1] << "\n\n";
        reportStream << "Purification Progress: The dark magic weakens!\n";
        reportStream << "The Wizard is one step closer to redemption!\n\n";
    }
    else {
        reportStream << "You've done your best, but there's still work to do.\n\n";
        reportStream << "You've answered " << score << " out of " << questions.size() << " correctly\n";
        reportStream << "Needed to pass: " << requiredScore[l - 1] << "\n\n";
        reportStream << "Purification Progress: The Wizard's curse remains strong. \n\n";
    }

    reportStream << "Press Enter to continue...";

    report = reportStream.str();
    updateReportLayout(c);
    window.draw(box);
    window.draw(progressText);
}

void QuestionHandler::updateScoreText() { // update current question and score to display
    string displayText =
        "Question: " + to_string(currentQuestionIndex + 1) + "/" + to_string(questions.size()) + " | Score:  " + to_string(score);
    scoreText.setString(displayText);
}

void QuestionHandler::loadSound(SoundBuffer& soundBuffer, const string& filename, Sound& sound) {
    resourceLoader(soundBuffer, filename);  // load the sound file into the buffer
    sound.setBuffer(soundBuffer);           // set the buffer to the sound object
    sound.setVolume(40.f);                  // set volume for the sound (optional)
}

void QuestionHandler::resize() {
    Vector2f scale = ResizeManager::getScale(window);  // get the scale based on window size

    // resize the next button
    nextButton.resize(
        Vector2f((window.getSize().x - 150 * scale.x) / scale.x, (window.getSize().y - 80 * scale.y) / scale.y),
        Vector2f(120, 60)
    );

    // resize the score text
    scoreText.setCharacterSize(ResizeManager::scaleText(36, scale.y));
    scoreText.setPosition(20 * scale.x, window.getSize().y - 40 * scale.y);
    // resize question and buttons by reloading
    loadCurrentQuestion();
}