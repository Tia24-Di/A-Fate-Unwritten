#ifndef GAME_H
#define GAME_H

// core SFML modules
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// project includes
#include "Scene.h"
#include "Menu.h"
#include "Intro.h"
#include "Lion.h"
#include "Scarecrow.h"
#include "Tinman.h"
#include "Outro.h"
#include "GameScore.h"

// UI and utility headers
#include "ChapterTitleScreens.h"
#include "PauseMenu.h"
#include "ControlScreen.h"
#include "RetryScreen.h"
#include "LoadScreen.h"

class Game {
public:
    Game();     // constructor: sets up the game window and initial state
    void run(); // main game loop
private:
    sf::RenderWindow window;                     // main SFML window for rendering the game
    ChapterTitleScreens chapterTitleScreens;     // handles display of chapter title screens

    // -- enumeration of all possible game scenes -- 
    enum class Scene { Menu, Intro, Lion, Scarecrow, Tinman, Outro };
    Scene currentScene;                          // currently active scene

    // -- smart pointers to each scene instance -- 
    std::unique_ptr<Menu> menuScene;              // Menu scene
    std::unique_ptr<Intro> introScene;            // Intro scene
    std::unique_ptr<Lion> lionScene;              // Lion scene
    std::unique_ptr<Scarecrow> scarecrowScene;    // Scarecrow scene
    std::unique_ptr<Tinman> tinmanScene;          // Tinman scene
    std::unique_ptr<Outro> outroScene;            // Outro scene

    // -- core game systems -- 
    std::unique_ptr<PauseMenu> pauseMenu;         // pause menu 
    std::unique_ptr<ControlScreen> controlScreen; // control screen (shows controls/instructions)
    std::unique_ptr<RetryScreen> retryScreen;     // retry screen (shown when player fails and retries)
    std::unique_ptr<LoadScreen> loadScreen;       // loading screen (shown when loading scenes)
    bool isPaused = false;                        // tracks if game is currently paused

    // -- scene and window management -- 
    void switchScene();                           // switch between scenes when needed
    void resize();                                // adjust UI and game objects to match new window size
    void handlePauseMenu();                       // handle input and logic when the game is paused
    void handleEvents();                          // poll and process window events (input, window close, etc.)
    void updateAndRender();                       // update game state and render the current frame
    void handleRetry();                           // handle retry logic when player fails (e.g., Tinman or Lion scene)

    // -- game progress persistence -- 
    void loadProgress();                          // load saved game state from storage
    void loadScene(Scene scene);                  // load a specific scene based on saved or selected state

    // -- game state management -- 
    void saveProgress();  // save the current game state to storage
    void resetGame();     // reset the game progress and state

    // -- scene-specific loading helpers -- 
    void loadLion();      // load Lion scene
    void loadScarecrow(); // load Scarecrow scene
    void loadTinman();    // load Tinman scene

    // -- music handling -- 
    sf::Music music;                              // music player for background music
    std::string filepath;                         // filepath of the currently loaded music
    float volume;                                 // volume level of the music
    void updateSceneMusic();                      // update and play music based on current scene
    void playMusic(std::string filePath, float volume); // play music from file with specified volume
    void pauseMusic();                            // pause currently playing music
    void resumeMusic();                           // resume paused music
    void stopMusic();                             // stop music completely

    // -- outro-specific variables for music handling --
    GameScore& gameScore = GameScore::getInstance(); // singleton reference to manage scores
    bool passed;                                  // true if player passed final challenges
    bool isFlatlinePlaying = false;               // tracks if "flatline" (failure) music is playing
};

#endif
