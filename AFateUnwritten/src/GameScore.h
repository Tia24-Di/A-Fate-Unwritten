#ifndef GAME_SCORE_H
#define GAME_SCORE_H

// handles the management and tracking of scores for different game levels and total score
class GameScore {
public:
    static GameScore& getInstance();    // singleton pattern: provides a single instance of GameScore

    // enum representing the different score categories
    enum ScoreType {
        LION,           // score for the Lion level
        SCARECROW,      // score for the Scarecrow level
        TINMANQUIZ,     // score for the Tinman quiz section
        TINMANRIDDLE,   // score for the Tinman riddle section
        TINMAN,         // score for the Tinman level
        TOTALSCORE      // total score accumulated across all levels
    };

    void setScore(ScoreType type, int score); // sets the score for a specific level
    int getScore(ScoreType type) const;       // retrieves the score for a specific level
    bool Condition(ScoreType type);           // checks if a specific condition for a level is met
    void resetScore(ScoreType type);          // resets the score for a specific category (type)
private:
    // constructor: ensure singleton pattern (only one instance of GameScore can exist)
    GameScore();

    // score variables for each game level and the total score
    int lion, scarecrow, tinman, tinmanquiz, tinmanriddle, total_score;
    bool ending_condition;  // flag to manage the ending condition of the game
    void updateTotal();     // update the total score whenever individual scores change
    void updateTinman();    // handle any specific updates needed for the Tinman level
};

#endif // GAME_SCORE_H
