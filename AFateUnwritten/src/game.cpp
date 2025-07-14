#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <chrono>
#include <thread>
#include <fstream>

#include "Game.h"
#include "RetryScreen.h"
#include "LoadScreen.h"
#include "LoadResources.h"

using namespace std;
using namespace sf;

// constructor: sets up the window and all the game scenes
Game::Game()
    : window(VideoMode(1408, 728), "A Fate Unwritten")
    , chapterTitleScreens(window)
    , currentScene(Scene::Menu)
    , menuScene(make_unique<Menu>(window))
    , introScene(make_unique<Intro>(window))
    , lionScene(make_unique<Lion>(window))
    , scarecrowScene(make_unique<Scarecrow>(window))
    , tinmanScene(make_unique<Tinman>(window))
    , outroScene(make_unique<Outro>(window))
    , pauseMenu(make_unique<PauseMenu>(window))
    , controlScreen(make_unique<ControlScreen>(window))
    , retryScreen(make_unique<RetryScreen>(window))
    , loadScreen(make_unique<LoadScreen>(window))
{
    // make sure the window was created successfully
    if (!window.isOpen()) {
        throw runtime_error("Error: Window failed to open!");
    }
    // initial scene music
    updateSceneMusic(); 
}

// writes the player's progress to a file
void Game::saveProgress() {
    std::ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        GameScore& score = GameScore::getInstance();
        saveFile << static_cast<int>(currentScene) << "\n";
        saveFile << score.getScore(GameScore::ScoreType::LION) << "\n";
        saveFile << score.getScore(GameScore::ScoreType::SCARECROW) << "\n";
        saveFile << score.getScore(GameScore::ScoreType::TINMAN) << "\n";
        saveFile.close();
    }
}

// loads saved progress from a file if available
void Game::loadProgress() {
    ifstream loadFile("savegame.txt");
    int savedScene = 0;
    int lionScore = 0, scarecrowScore = 0, tinmanScore = 0;

    if (loadFile.is_open()) {
        loadFile >> savedScene;
        loadFile >> lionScore;
        loadFile >> scarecrowScore;
        loadFile >> tinmanScore;
        loadFile.close();

        GameScore& score = GameScore::getInstance();
        score.setScore(GameScore::ScoreType::LION, lionScore);
        score.setScore(GameScore::ScoreType::SCARECROW, scarecrowScore);
        score.setScore(GameScore::ScoreType::TINMAN, tinmanScore);

    }
    // in case saved scene is out of bounds (corrupted save or manual tampering)
    // validate the loaded scene, fallback to intro if it's invalid
    if (savedScene >= static_cast<int>(Scene::Menu) && savedScene <= static_cast<int>(Scene::Outro)) {
        loadScene(static_cast<Scene>(savedScene));
    }
    else {
        // save file not found, start a new game
        currentScene = Scene::Intro;
        introScene->reset();
    }
}

// switch music based on scene
void Game::updateSceneMusic() {
    stopMusic();  // stop any current music before switching

    switch (currentScene) {
        case Scene::Menu:
            filepath = "resources/music/MenuMusic.ogg";
            volume = 80.f;
            break;
        case Scene::Intro:
            filepath = "resources/music/IntroMusic.ogg";
            volume = 130.f;
            break;
        case Scene::Lion:
            filepath = "resources/music/LionBackgroundMusic.ogg";
            volume = 60.f;
            break;
        case Scene::Scarecrow:
            filepath = "resources/music/ScareCrowBackgroundMusic.ogg";
            volume = 20.f;
            break;
        case Scene::Tinman:
            filepath = "resources/music/TinManBackgroundMusic.ogg";
            volume = 30.f;
            break;
        case Scene::Outro:
            passed = gameScore.Condition(GameScore::TOTALSCORE);
            if (passed) {
                filepath = "resources/music/GoodEndingBackgroundMusic.ogg";
            }
            else {
                filepath = "resources/music/BadEndingMusic.ogg";  
            }
            volume = 80.f;
            break;
        default:
            return; // no music for Menu or unsupported scenes
    }

    playMusic(filepath, volume);
}

// play music using filepath and volume
void Game::playMusic(string filepath, float volume) {
    resourceLoader(music, filepath);
    // ensure flatline audio does not loop
    if (filepath == "resources/music/flatline.ogg")
        music.setLoop(false);
    else
        music.setLoop(true);
    music.setVolume(volume);
    music.play();
}

// pause the currently playing music
void Game::pauseMusic() {
    if (music.getStatus() == sf::Music::Playing) { 
        music.pause();
    }
}

// resume music if it was paused
void Game::resumeMusic() {
    if (window.isOpen()) {
        if (music.getStatus() == sf::Music::Paused) {
            music.play();
        }
    }
}

// stop music currently playing
void Game::stopMusic() {
    if (music.getStatus() == sf::Music::Playing || music.getStatus() == sf::Music::Paused) {
        // fade out
        float currentVolume = music.getVolume();
        for (float volume = currentVolume; volume > 0; volume -= 10.f) {
            music.setVolume(volume);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Smooth fade
        }
        music.stop();
        music.setVolume(currentVolume); // reset volume for future play
    }
    music.setPlayingOffset(Time::Zero);
}

// loads a specific scene and resets it
void Game::loadScene(Scene scene) {
    switch (scene) {
        case Scene::Intro:
            currentScene = Scene::Intro;
            introScene->reset();
            break;
        case Scene::Lion:
            loadLion();
            break;
        case Scene::Scarecrow:
            loadScarecrow();
            break;
        case Scene::Tinman:
            loadTinman();
            break;
        case Scene::Outro:
            isFlatlinePlaying = false;
            currentScene = Scene::Outro;
            outroScene = make_unique<Outro>(window);
            break;
        default:
            currentScene = Scene::Intro;
            introScene->reset();
            break;
    }
}

// handles switching between scenes when certain conditions are met
void Game::switchScene() {
    if (currentScene == Scene::Menu && menuScene->isTextComplete()) {
        int selectedItem = menuScene->getPressedItem();
        this_thread::sleep_for(chrono::milliseconds(500));

        if (selectedItem == 2) {  // display control screen
            pauseMusic();  
            ControlScreen controls(window);
            controls.show();
            resumeMusic();
            resize();
            menuScene->reset();
            return;
        }
        updateSceneMusic();

        if (selectedItem == 0) {
            // clear save file if starting a New Game
            ofstream saveFile("savegame.txt", ofstream::trunc);
            if (saveFile.is_open()) {
                saveFile.close();  // empty the file
            }
            // start a new game
            currentScene = Scene::Intro;
            introScene->reset();
            updateSceneMusic();
        }
        else if (selectedItem == 1) {  // load existing game progress
            loadProgress();
            updateSceneMusic();
        }
        
        else {                         // any other option, exit game
            stopMusic();
            window.close();
        }
    }

    if (currentScene == Scene::Intro && introScene->isTextComplete()) {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            stopMusic();
            this_thread::sleep_for(chrono::milliseconds(500));
            chapterTitleScreens.showNextChapter("Chapter 1: The Lion");
            currentScene = Scene::Lion;
            lionScene->reset();
            updateSceneMusic();  
            saveProgress();
        }
    }

    if (currentScene == Scene::Lion && lionScene->isTextComplete() && lionScene->areQuestionsComplete()) {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            stopMusic();
            this_thread::sleep_for(chrono::milliseconds(500));
            chapterTitleScreens.showNextChapter("Chapter 2: The Scarecrow");
            currentScene = Scene::Scarecrow;
            scarecrowScene->reset();
            updateSceneMusic();  
            saveProgress();
        }
    }

    if (currentScene == Scene::Scarecrow && scarecrowScene->isTextComplete() && scarecrowScene->areQuestionsComplete()) {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            stopMusic();
            this_thread::sleep_for(chrono::milliseconds(500));
            chapterTitleScreens.showNextChapter("Chapter 3: The Tinman");
            currentScene = Scene::Tinman;
            tinmanScene->reset();
            updateSceneMusic(); 
            saveProgress();
        }
    }

    if (currentScene == Scene::Tinman && tinmanScene->isTextComplete() && tinmanScene->areQuestionsComplete() && tinmanScene->areRiddlesComplete()) {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            this_thread::sleep_for(chrono::milliseconds(500));
            currentScene = Scene::Outro;
            outroScene = make_unique<Outro>(window);
            updateSceneMusic();  
            saveProgress();
        }
    }
}

// resets the entire game to a clean state
void Game::resetGame() {
    // reset scores
    GameScore::getInstance().resetScore(GameScore::ScoreType::TOTALSCORE);
    // recreate all scenes
    menuScene = make_unique<Menu>(window);
    introScene = make_unique<Intro>(window);
    lionScene = make_unique<Lion>(window);
    scarecrowScene = make_unique<Scarecrow>(window);
    tinmanScene = make_unique<Tinman>(window);
    outroScene = make_unique<Outro>(window);
    // set the current scene back Intro
    currentScene = Scene::Intro;
    updateSceneMusic();
    saveProgress();
}

// resets everything needed to restart from lion (score, scenes and set currentScene)
void Game::loadLion() {
    GameScore::getInstance().resetScore(GameScore::ScoreType::TOTALSCORE);
    lionScene = make_unique<Lion>(window);  
    scarecrowScene = make_unique<Scarecrow>(window);
    tinmanScene = make_unique<Tinman>(window);
    outroScene = make_unique<Outro>(window);
    currentScene = Scene::Lion;
    updateSceneMusic();
    saveProgress();
}

// resets everything needed to restart from scarecrow
void Game::loadScarecrow() {
    GameScore::getInstance().resetScore(GameScore::ScoreType::SCARECROW);
    GameScore::getInstance().resetScore(GameScore::ScoreType::TINMAN);
    scarecrowScene = make_unique<Scarecrow>(window);
    tinmanScene = make_unique<Tinman>(window);
    outroScene = make_unique<Outro>(window);
    currentScene = Scene::Scarecrow;
    updateSceneMusic();
    saveProgress();
}

// resets everything needed to restart from tinman
void Game::loadTinman() {
    GameScore::getInstance().resetScore(GameScore::ScoreType::TINMAN);
    tinmanScene = make_unique<Tinman>(window);
    outroScene = make_unique<Outro>(window);
    currentScene = Scene::Tinman;
    updateSceneMusic();
    saveProgress();
}

// updates and draws the active scene
void Game::updateAndRender() {
    switch (currentScene) {
        case Scene::Menu:
            menuScene->update();
            menuScene->render();
            switchScene();
            break;
        case Scene::Intro:
            introScene->update();
            introScene->render();
            switchScene();
            break;
        case Scene::Lion:
            lionScene->update();
            lionScene->render();
            switchScene();
            break;
        case Scene::Scarecrow:
            scarecrowScene->update();
            scarecrowScene->render();
            switchScene();
            break;
        case Scene::Tinman:
            tinmanScene->update();
            tinmanScene->render();
            switchScene();
            break;
        case Scene::Outro:
            outroScene->update();
            outroScene->render();

            // special case that allows flatline to play without conflict
            if (outroScene->isPlayingGif()) {
                if (!isFlatlinePlaying) {
                    stopMusic(); 
                    playMusic("resources/music/flatline.ogg", 5.f); 
                    isFlatlinePlaying = true; 
                }
            }
            break;
    }
}

// displays and handles the pause menu choices
void Game::handlePauseMenu() {
    PauseMenuChoice choice = pauseMenu->show();
    switch (choice) {
        case PauseMenuChoice::Resume:   
            isPaused = false;  
            resize();
            break;
        case PauseMenuChoice::MainMenu:
            currentScene = Scene::Menu;  
            updateSceneMusic();
            menuScene->reset();
            isPaused = false;  
            resize();
            break;
        case PauseMenuChoice::Controls: 
            break;
        case PauseMenuChoice::Quit:
            stopMusic();
            window.close();              
            break;
    }
}

// processes window events like closing, resizing, and keypresses
void Game::handleEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            stopMusic();
            window.close(); 
        }
        if (event.type == Event::Resized) {
            FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(View(visibleArea));
            resize();
        }
        if (event.type == Event::KeyPressed) {
            switch (event.key.code) {
                case Keyboard::Escape:
                case Keyboard::Space:
                    if (currentScene != Scene::Menu) {   
                        isPaused = !isPaused;
                        this_thread::sleep_for(chrono::milliseconds(300));
                    }
                    break;
                case Keyboard::C: {
                    if (currentScene != Scene::Menu) {
                        pauseMusic();  
                        ControlScreen controls(window);
                        controls.show();
                        resumeMusic();
                        resize();
                    }
                    break;
                }
                default:
                    break;
                }
        }
    }
}

// shows the retry screen and acts on the user's choice
void Game::handleRetry() {
    isFlatlinePlaying = false;
    RetryScreen retryScreen(window);
    RetryResult result = retryScreen.show();

    while (true) {
        if (result == RetryResult::Restart) {
            resetGame();
            break;
        }
        if (result == RetryResult::Quit) {
            stopMusic();
            window.close();
            return;
        }
        if (result == RetryResult::Load) {
            LoadScreen loadScreen(window);
            LoadChoice loadChoice = loadScreen.show();

            if (!window.isOpen()) {
                return; // window was closed (ESC or X)
            }

            if (loadChoice == LoadChoice::Back) {
                result = retryScreen.show();
                continue;
            }

            switch (loadChoice) {
                case LoadChoice::Chapter1: loadLion(); break;
                case LoadChoice::Chapter2: loadScarecrow(); break;
                case LoadChoice::Chapter3: loadTinman(); break;
                default: break;
            }
            break;
        }
    }
}

void Game::run() {
    // main game loop
    while (window.isOpen()) {
        handleEvents();  // process input and system events
        // check if paused then handle pause menu
        if (isPaused) {
            pauseMusic();
            handlePauseMenu();
            resumeMusic();        
        }
        else {
            updateAndRender();  
        }
        // check for outro completion and show retry option
        if (currentScene == Scene::Outro && outroScene->outroFinished) {
            stopMusic();
            handleRetry();
        }

        // dynamically adjust framerate: no delay when holding Right arrow (fast-forwarding),
        // short delay otherwise to limit CPU usage
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            this_thread::sleep_for(chrono::milliseconds(0));
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(30));
        }
    }
}

// resize and reposition all UI and scenes to match current window dimensions
void Game::resize() {
    pauseMenu->resize();
    retryScreen->resize();
    controlScreen->resize();
    loadScreen->resize();
    menuScene->resize();
    introScene->resize();
    lionScene->resize();
    scarecrowScene->resize();
    tinmanScene->resize(); 
    outroScene->resize();
}

// create game instance and start main loop (for Release)
//int WinMain() {
//    Game game;
//    game.run();
//    return 0;
//}

// create game instance and start main loop (for Debug with terminal)
int main() {
    Game game;
    game.run();
    return 0;
}