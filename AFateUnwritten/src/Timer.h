#ifndef TIMER_H
#define TIMER_H

#include <SFML/Graphics.hpp>
#include <sstream>

// handles countdown timing, updating, rendering, and control (start/stop/reset)
class Timer {
public:
    // -- constructor --
    Timer(sf::RenderWindow& window, float seconds);  // sets up timer with window reference and starting time

    // -- core functionality --
    void update();       // updates the countdown timer each frame
    void render();       // renders the timer text onto the window
    bool isComplete();   // returns true if the timer has reached zero

    // -- control methods --
    void stop();         // stops the timer
    void reset();        // resets the timer to its initial time

private:
    // -- core components --
    sf::RenderWindow* window;   // pointer to the window for rendering the timer text
    sf::Clock clock;            // SFML clock to measure elapsed time
    sf::Font font;              // font used to display the timer
    sf::Text timerText;         // text object showing the remaining time
    std::stringstream ss;       // used to format the timer text

    // -- timing variables --
    float seconds;              // number of seconds since last frame (delta time)
    float remainingTime;        // how much time is left in the countdown
    float initialTime;          // the original starting time (for resets)

    // -- state flags --
    bool complete;              // true if timer has finished counting down
    bool stopped;               // true if timer is currently paused
};

#endif
