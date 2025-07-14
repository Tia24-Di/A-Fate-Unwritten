#include "Timer.h"
#include "LoadResources.h"
#include "ResizeManager.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constructor: initializes the timer with a reference to the window and the starting time (in seconds)
Timer::Timer(RenderWindow& window, float seconds) {
    this->window = &window;            // save pointer to window
    this->seconds = seconds;           // total time for the timer
    this->initialTime = seconds;       // remember the original set time for reset
    this->complete = false;            // timer is not complete at start
    this->remainingTime = seconds;     // remaining time initially equals total time
    this->stopped = true;              // timer is initially stopped

    clock.restart();                   // restart the internal clock

    resourceLoader(font, "resources/fonts/INFROMAN.ttf"); // load font for displaying the timer

    // set up the appearance of the timer text
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);
}

// Updates the timer countdown
void Timer::update() {
    float elapsedTime = clock.getElapsedTime().asSeconds();  // get time elapsed since timer started
    remainingTime = (seconds + 1) - elapsedTime;             // calculate remaining time (+1 for smoother display)

    if (remainingTime <= 0.0f) {
        // Timer has expired
        remainingTime = 0.0f;
        complete = true;
    }
    else if (remainingTime <= (seconds / 6)) {
        // if time left is less than 1/6 of total, change text color to red (warning)
        timerText.setFillColor(Color::Red);
    }
}

// draws the timer on the screen
void Timer::render() {
    if (!stopped) {
        update(); // update the timer first before drawing

        // handle scaling for different window sizes
        Vector2f scale = ResizeManager::getScale(*window);
        float scaledSize = ResizeManager::scaleText(50.0f, scale.y);

        // apply scaling to timer text
        timerText.setCharacterSize(static_cast<unsigned int>(scaledSize));
        timerText.setPosition(ResizeManager::scalePosition({ 10.0f, 10.0f }, scale));
        timerText.setString("Time Left: " + to_string(static_cast<int>(remainingTime))); // update displayed text

        window->draw(timerText); // draw the timer text
    }
}

// returns true if the timer has completed
bool Timer::isComplete() {
    return complete;
}

// stops the timer (pauses updating and rendering)
void Timer::stop() {
    stopped = true;
}

// resets the timer back to its original time and starts counting again
void Timer::reset() {
    seconds = initialTime;
    remainingTime = initialTime;
    stopped = false;
    complete = false;
    clock.restart(); // restart the internal clock
    timerText.setFillColor(Color::White); // reset text color to white
    update(); // immediately update the displayed time
}
