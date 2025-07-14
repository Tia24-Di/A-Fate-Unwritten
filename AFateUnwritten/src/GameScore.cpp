#include "GameScore.h"

// singleton: Get the single instance of GameScore
GameScore& GameScore::getInstance() {
    static GameScore instance;
    return instance;
}

// constructor: initialize all scores and ending condition
GameScore::GameScore()
    : lion(0), scarecrow(0), tinman(0),
    total_score(0), tinmanquiz(0), tinmanriddle(0),
    ending_condition(false) {}

// retrieve a specific score based on ScoreType
int GameScore::getScore(ScoreType type) const {
    switch (type) {
    case LION: return lion;
    case SCARECROW: return scarecrow;
    case TINMAN: return tinman;
    case TINMANQUIZ: return tinmanquiz;
    case TINMANRIDDLE: return tinmanriddle;
    case TOTALSCORE: return total_score;
    default: return -1; // invalid type
    }
}

// check if a specific score meets the condition to complete that level
bool GameScore::Condition(ScoreType score) {
    switch (score) {
    case LION:
        ending_condition = (lion >= 4);
        return ending_condition;
    case SCARECROW:
        ending_condition = (scarecrow >= 8);
        return ending_condition;
    case TINMAN:
        ending_condition = ((tinmanquiz >= 6 && tinmanriddle >= 2) || (tinmanriddle == 4));
        return ending_condition;
    case TOTALSCORE:
        ending_condition = (total_score >= 24);
        return ending_condition;
    default:
        return false; // Invalid type
    }
}

// reset a specific score to 0 (or all scores if TOTALSCORE is selected)
void GameScore::resetScore(ScoreType type) {
    switch (type) {
    case LION:
        lion = 0;
        break;
    case SCARECROW:
        scarecrow = 0;
        break;
    case TINMAN:
        tinman = tinmanquiz = tinmanriddle = 0;
        break;
    case TOTALSCORE:
        lion = scarecrow = tinman = tinmanquiz = tinmanriddle = total_score = 0;
        break;
    default:
        return;
    }
    updateTotal(); // update total after any reset
}

// update the total score based on lion, scarecrow, and tinman scores
void GameScore::updateTotal() {
    total_score = lion + scarecrow + tinman;
}

// update the tinman score based on quiz and riddle parts
void GameScore::updateTinman() {
    tinman = tinmanquiz + tinmanriddle;
}

// set a specific score manually
void GameScore::setScore(ScoreType type, int score) {
    switch (type) {
    case LION:
        lion = score;
        updateTotal();
        break;
    case SCARECROW:
        scarecrow = score;
        updateTotal();
        break;
    case TINMAN:
        tinman = score;
        updateTotal();
        break;
    case TOTALSCORE:
        updateTotal(); // just recalculate total, don't set directly
        break;
    case TINMANQUIZ:
        tinmanquiz = score;
        updateTinman();
        updateTotal();
        break;
    case TINMANRIDDLE:
        tinmanriddle = score;
        updateTinman();
        updateTotal();
        break;
    default:
        break;
    }
}
