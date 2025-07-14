#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

#include "ChapterTitleScreens.h"
#include "LoadResources.h"
#include "ResizeManager.h"

using namespace std;
using namespace sf;

ChapterTitleScreens::ChapterTitleScreens(RenderWindow& window)
    : m_window(window), m_font(), m_text(), m_nextChapterText() {
    resourceLoader(m_font, "resources/fonts/CALIFR.ttf");
}

void ChapterTitleScreens::showNextChapter(const string& chapterName) {

    // load and play chapter screen music
    resourceLoader(m_chapterMusic, "resources/music/ChapterTitleMusic.ogg");
    m_chapterMusic.setVolume(60); // adjust as needed
    m_chapterMusic.play();

    m_nextChapterText.setString(chapterName);
    m_nextChapterText.setFont(m_font);
    m_nextChapterText.setFillColor(Color::White);

    // resize the text
    Vector2f scale = ResizeManager::getScale(m_window);
    float scaledTextSize = ResizeManager::scaleText(100, scale.y);
    m_nextChapterText.setCharacterSize(static_cast<unsigned int>(scaledTextSize));

    FloatRect textBounds = m_nextChapterText.getLocalBounds();
    m_nextChapterText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    m_nextChapterText.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 2);

    m_window.clear();
    m_window.draw(m_nextChapterText);
    m_window.display();

    Clock clock; //starts timer to measure how much time has passed
    const float fadeDuration = 1.0f; // the fade in lasts for 1 second
    const float holdDuration = 2.0f; // how long to stay fully visible

    while (clock.getElapsedTime().asSeconds() < fadeDuration) {     // calculates transperency of text
       
        // FADE IN //
        float alpha = (clock.getElapsedTime().asSeconds() / fadeDuration) * 255;
        
        if (alpha > 255) //safety check
        {
            alpha = 255;
        }
        
        m_nextChapterText.setFillColor(Color(255, 255, 255, static_cast<Uint8>(alpha)));

        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::Closed || event.type == Event::KeyPressed) {
                return;
            }
        }

        m_window.clear();
        m_window.draw(m_nextChapterText);
        m_window.display();

    }

    // HOLD FULLY VISIBLE
    clock.restart();    // restarts to start from 0 again after fade in
    const float shiftDuration = 2.0f;   // how long it takes for one full color cycle
    bool isReversing = false;   // reverses the animation back to white
    float t;
    while (clock.getElapsedTime().asSeconds() < holdDuration) {
        float elapsed = clock.getElapsedTime().asSeconds();

        // Oscillating time
        t = fmod(elapsed / shiftDuration, 3.0f);  // This ensures it loops every 'shiftDuration' seconds
        
        if (isReversing) { // if isReversing true, the color shift is flipped, going back to original color
            t = 1.0f - t;  // Reverse time (from orange to white)
        }

        //The use of sine function is to smoothly oscillate between color values
        int red = 255;
        int green = static_cast<int>(165 + 90 * sin(t * 3.14159f));  // Shift from 165 to 255 and back
        int blue = static_cast<int>(255 - 255 * sin(t * 3.14159f));  // Shift from 255 to 0 and back

        // t * 3.14159f, t is a value that progresses over time during the effect
        // it causes the sin function to go from sin(0) = 1 , sin(pi/2)=1 , sin(pi) = 0
        // so over the time interval t: At t=0, you're at the beginning of the wave
        // at t = 0.5, you're at the peak(max color shift)
        // at t = 1.0 , you're back to the start
        // this gives a symmetric back and forth pulse

        m_nextChapterText.setFillColor(Color(red, green, blue));
        
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::Closed || event.type == Event::KeyPressed) {
                return;
            }
        }

        m_window.clear();
        m_window.draw(m_nextChapterText);
        m_window.display();
    }

    // FADE OUT //
    clock.restart();
    while (clock.getElapsedTime().asSeconds() < fadeDuration) { // this loop will run until the team reaches fadeDuration

        //calculate transparency(which is Alpha value)
        float elapsed = clock.getElapsedTime().asSeconds();
        float alpha = 255 - (elapsed / fadeDuration) * 255;
        if (alpha < 0) alpha = 0;   // safety to avoid negatives

        //As time increases alpha decreases from 255 to 0
        //At elapsed = 0, alpha will be 255, which is fully visible
        //At elapsed = fadeduration , alpha = 0 which is fully transparent
 
        m_nextChapterText.setFillColor(Color(255, 255, 255, static_cast<Uint8>(alpha)));

        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::Closed || event.type == Event::KeyPressed) {
                return;
            }
        }

        m_window.clear();
        m_window.draw(m_nextChapterText);
        m_window.display();
    }

    m_chapterMusic.stop(); // stop the chapter music
}

