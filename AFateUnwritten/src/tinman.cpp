#include "Tinman.h"
#include "TinmanNarrative.h"
#include "ResizeManager.h"
#include "LoadResources.h"

using namespace std;
using namespace sf;

// constructor: initializes the Tinman scene
Tinman::Tinman(RenderWindow& window)
    : Scene(window), timer(window, 60){ // timer set to 60 seconds
    loadResources();
    initializeHandlers();
    initializeUI();
    resize();
}

// sets up question and riddle handlers
void Tinman::initializeHandlers() {
    // load Tinman quiz questions
    tinmanQuestions = TinmanNarrative::getTinmanQuestions();
    questionHandler = make_unique<QuestionHandler>(window);
    questionHandler->setQuestions(tinmanQuestions);
    questionHandler->setButtonColor(LevelType::Tinman);

    // load Tinman riddle questions
    tinmanRiddles = TinmanNarrative::getTinmanRiddles();
    riddleHandler = make_unique<QuestionHandler>(window);
    riddleHandler->setQuestions(tinmanRiddles);
    riddleHandler->setButtonColor(LevelType::Tinman);
}

// initializes UI components like result and failure boxes
void Tinman::initializeUI() {
    Vector2f scale = ResizeManager::getScale(window);

    // set up the result box
    resultBox.setSize(Vector2f(window.getSize().x * 0.7f, window.getSize().y * 0.7f));
    resultBox.setFillColor(Color(100, 100, 100, 180));
    resultBox.setOutlineThickness(2 * scale.x);
    resultBox.setOutlineColor(Color::White);
    resultBox.setPosition(
        (window.getSize().x - resultBox.getSize().x) / 2,
        (window.getSize().y - resultBox.getSize().y) / 2
    );

    // load font and set up result text
    resourceLoader(resultFont, "resources/fonts/INFROMAN.ttf");
    resultText.setFont(resultFont);
    float scaledTextSize = ResizeManager::scaleText(40, scale.y);
    resultText.setCharacterSize(static_cast<unsigned int>(scaledTextSize));
    resultText.setFillColor(Color::White);
    resultText.setPosition(
        resultBox.getPosition().x + 20 * scale.x,
        resultBox.getPosition().y + 20 * scale.y
    );

    // set up the failure box for timer expiration
    float boxWidth = window.getSize().x * 0.8f;
    float boxHeight = window.getSize().y * 0.08f;

    failureBox.setSize(Vector2f(boxWidth, boxHeight));
    failureBox.setFillColor(Color(139, 0, 0, 150)); // transparent dark red
    failureBox.setPosition((window.getSize().x - boxWidth) / 2, 20 * scale.y);

    // load font and set up failure text
    resourceLoader(failureFont, "resources/fonts/INFROMAN.ttf");
    failureText.setFont(failureFont);
    failureText.setCharacterSize(static_cast<unsigned int>(scaledTextSize));
    failureText.setFillColor(Color::White);
    failureText.setStyle(Text::Bold);
    failureText.setString("Time's up! The Tinman score has been reset to 0.");

    // center the failure text inside the box
    FloatRect textBounds = failureText.getLocalBounds();
    failureText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    failureText.setPosition(failureBox.getPosition().x + boxWidth / 2.f, failureBox.getPosition().y + boxHeight / 2.f);
}

// creates the final summary text based on performance
void Tinman::progressReport() {
    ss.str("");
    ss.clear();

    if (gameScore.Condition(GameScore::TINMAN)) {
        ss << "Congratulations, Brave Adventurer!\n\n";
        ss << "You answered " << quizScore << " / 8 Tinman quiz questions,\n";
        ss << "and solved " << riddleScore << " / 4  final trail riddles.\n";
        ss << "Emotional Calibration complete. \nThe Tinman feels a spark of humanity reignite.\n";
        ss << "Purification Progress: The dark magic weakens,\nand the Wizard senses a shift.\n\n";
    }
    else {
        ss << "A Heavy Silence Falls...\n\n";
        ss << "You answered " << quizScore << " / 8 Tinman quiz questions,\n";
        ss << "and solved " << riddleScore << " / 4 final trial riddles.\n";
        ss << "Emotional Calibration: Incomplete.\nThe Tinman's heart remains rusted,";
        ss << " his humanity distant.\n\nPurification Progress: The Wizard's curse remains strong.\n\n";
    }

    ss << "Press Enter to continue your journey.";
    resultText.setString(ss.str());
}

// loads backgrounds and start dialogue
void Tinman::loadResources() {
    bgManager.setDefault("resources/images/tinman_sad.png");
    TinmanNarrative::getBackgroundTriggers(bgManager);

    auto start = TinmanNarrative::getStartNarrative();
    tinmanDialogues.assign(start.begin(), start.end());
    loadCurrentDialogue();
}

// loads the current dialogue line
void Tinman::loadCurrentDialogue() {
    if (currentDialogueIndex >= tinmanDialogues.size()) return;

    const auto& line = tinmanDialogues[currentDialogueIndex];
    tinmanDialogue = make_unique<Dialogue>(window, line.speaker, line.text);
    tinmanDialogue->setBackgroundChanges(bgManager.getTriggerMap());

    // set a callback for background changes
    tinmanDialogue->setBackgroundChangeCallback([this](const string& key) {
        bgManager(key);
    });
}

// main update loop for the scene
void Tinman::update() {
    switch (currentState) {
    case CurrentState::PreQuizDialogue:
        if (tinmanDialogue) {
            tinmanDialogue->update();
            if (tinmanDialogue->isTextComplete()) {
                currentDialogueIndex++;
                if (currentDialogueIndex < tinmanDialogues.size()) {
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
    case CurrentState::QuizActive:
        updateQuiz();
        break;
    case CurrentState::PreRiddleDialogue:
        if (tinmanDialogue) {
            tinmanDialogue->update();
            if (tinmanDialogue->isTextComplete()) {
                currentDialogueIndex++;
                if (currentDialogueIndex < tinmanDialogues.size()) {
                    loadCurrentDialogue();
                }
                else {
                    currentState = CurrentState::RiddlesActive;
                    riddleHandler->reset();
                    riddleHandler->loadCurrentQuestion();
                    timer.reset();
                }
            }
        }
        break;
    case CurrentState::RiddlesActive:
        updateRiddles();
        break;
    case CurrentState::ShowingResults:
        updateResults();
        break;
    case CurrentState::PostQuizDialogue:
        if (tinmanDialogue) {
            tinmanDialogue->update();
            if (tinmanDialogue->isTextComplete()) {
                currentDialogueIndex++;
                if (currentDialogueIndex < tinmanDialogues.size()) {
                    loadCurrentDialogue();
                }
            }
        }
        break;
    default:
        break;
    }

    if (!Keyboard::isKeyPressed(Keyboard::Enter)) {
        enterPressed = false;
    }
}

// update handling for quiz section
void Tinman::updateQuiz() {
    questionHandler->update();
    gameScore.setScore(GameScore::TINMANQUIZ, questionHandler->getScore());
    quizScore = gameScore.getScore(GameScore::TINMANQUIZ);

    // transition to pre-riddle dialogue when quiz finishes
    if (questionHandler->isComplete()) {
        currentState = CurrentState::PreRiddleDialogue;
        currentDialogueIndex = 0;
        tinmanDialogues = TinmanNarrative::getPreRiddleDialogue();
        loadCurrentDialogue();
    }
}

// update handling for riddles and timer
void Tinman::updateRiddles() {
    // time ran out during riddles
    if (timer.isComplete() && !riddleHandler->isComplete()) {
        timer.stop();   
        quizScore = 0;
        gameScore.resetScore(GameScore::TINMAN);
        riddleHandler->setComplete(true);
        tinmanDialogues = TinmanNarrative::getBadEndNarrative();
        currentState = CurrentState::ShowingResults;
        showFailureBox = true;
    }
    // player finished riddles in time
    else if (!timer.isComplete() && riddleHandler->isComplete()) {
        timer.stop();
        gameScore.setScore(GameScore::TINMANRIDDLE, riddleHandler->getScore());
        riddleScore = gameScore.getScore(GameScore::TINMANRIDDLE);
        gameScore.setScore(GameScore::TINMAN, quizScore + riddleScore);

        tinmanDialogues = gameScore.Condition(GameScore::TINMAN)
            ? TinmanNarrative::getGoodEndNarrative()
            : TinmanNarrative::getBadEndNarrative();

        currentState = CurrentState::ShowingResults;
    }
    else {
        riddleHandler->update();
    }
}

// update handling for showing the result screen
void Tinman::updateResults() {
    progressReport();
    if (Keyboard::isKeyPressed(Keyboard::Enter) && !enterPressed) {
        enterPressed = true;
        currentState = CurrentState::PostQuizDialogue;
        currentDialogueIndex = 0;
        loadCurrentDialogue();
    }
}

// renders all visuals based on the current state
void Tinman::render() {
    window.clear();
    bgManager.draw(window);

    switch (currentState) {
    case CurrentState::QuizActive:
        questionHandler->draw(window);
        break;
    case CurrentState::RiddlesActive:
        riddleHandler->draw(window);
        timer.render();
        break;
    case CurrentState::ShowingResults:
        window.draw(resultBox);
        window.draw(resultText);
        if (showFailureBox) {
            window.draw(failureBox);
            window.draw(failureText);
        }
        break;
    default:
        if (tinmanDialogue) tinmanDialogue->render();
        break;
    }

    window.display();
}

// helper: check if all quiz questions are completed
bool Tinman::areQuestionsComplete() const {
    return questionHandler && questionHandler->isComplete();
}

// helper: check if all riddles are completed
bool Tinman::areRiddlesComplete() const {
    return riddleHandler && riddleHandler->isComplete();
}

// resets the scene state to beginning
void Tinman::reset() {
    bgManager.setDefault("resources/images/tinman_sad.png");
    currentState = CurrentState::PreQuizDialogue;
    currentDialogueIndex = 0;
    quizScore = 0;
    riddleScore = 0;
    enterPressed = false;
    showFailureBox = false;

    tinmanDialogues = TinmanNarrative::getStartNarrative();
    loadCurrentDialogue();

    if (questionHandler) questionHandler->reset();
    if (riddleHandler) riddleHandler->reset();
}

// helper: Check if current dialogue is complete
bool Tinman::isTextComplete() const {
    return tinmanDialogue && tinmanDialogue->isTextComplete();
}
 
// resize and scale UI elements when window size changes
void Tinman::resize() {
    bgManager.resize(window);
    if (tinmanDialogue) tinmanDialogue->resize(window);
    if (questionHandler) questionHandler->resize();
    if (riddleHandler) riddleHandler->resize();
    initializeUI();  // reinitialize UI elements with new scale
}