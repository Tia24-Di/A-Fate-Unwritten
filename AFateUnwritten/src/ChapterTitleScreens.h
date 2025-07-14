#ifndef CHAPTER_TITLE_SCREENS_H
#define CHAPTER_TITLE_SCREENS_H

// SFML & Standard Library Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class ChapterTitleScreens {
public:
    ChapterTitleScreens(sf::RenderWindow& window);
    void showNextChapter(const std::string& chapterName);

private:
    sf::RenderWindow& m_window;  // reference to the game window
    sf::Font m_font;             // font for title text
    sf::Text m_text;             // title text
    sf::Text m_nextChapterText;  // text for next chapter prompt
    sf::Music m_chapterMusic;    // music for title screen only
};

#endif 
