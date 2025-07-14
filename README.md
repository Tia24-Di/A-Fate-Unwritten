# A Fate Unwritten 🎮
*Narrative driven, quiz-based game based on The Wizard of Oz using C++ and SFML.*

A Fate Unwritten is a 2D narrative-driven adventure game created using C++ and SFML. Developed as a final-year university project, the game blends interactive storytelling, player-driven decisions, and a quiz-based progression system into a visually engaging experience.

##📌 Features
-🧭 Branching Storyline – Player choices lead to multiple narrative paths and endings.
-🧠 Integrated Quiz Challenges – Advance by answering riddles and questions tied to story chapters.
-🎨 Custom Artwork & UI – Features AI generated backgrounds, characters, and UI elements.
-🎵 Dynamic Audio – Background music and sound effects that react to scene changes.
-⌛ Timer-Based Events – Some challenges are time-limited, increasing tension and immersion.
-🎮 Pause & Control Screens – In-game pause functionality and instructions screen.

# 🛠 Built With:
- **C++17**
- **SFML 2.6.1**
- Custom classes for:
  - `game`
  - `Menu`, `PauseMenu`, `ControlScreen`
  - `SceneManager`, `TextManager`, `QuestionHandler`, `BackgroundManager`, `Dialogue`, `GameScore`, `GifAnimator`, `ResizeManager`, `TextManager`, `Timer`, `Button`
  - `Lion`, `LionNarrative`, `Scarecrow`, `TinmanScene`
  - `Intro`, `LoadScreen`, `Outro`, `RetryScreen`, `ChapterTitleScreen`
    
# 📁 Folder Structure
A-Fate-Unwritten/ │ ├── assets/ # Textures, music, fonts ├── src/ # C++ source files │ ├── main.cpp │ ├── Menu.cpp / .hpp │ ├── PauseMenu.cpp / .hpp │ ├── ... ├── include/ # Header files ├── bin/Release/ # Compiled .exe and DLLs (for release) ├── README.md

# 🚀 How to Run

### 🧰 Requirements
- C++17-compatible compiler
- SFML 2.6.1
- Windows OS recommended (tested with Visual Studio)
### 🔧 Build Instructions (Visual Studio)
1. Clone this repository
2. Set up an SFML project using Visual Studio
3. Link SFML libraries and copy required DLLs
4. Build the solution and run the executable
   Or download the compiled .exe from the Debug section.

# 🎮 Controls
- **Enter** – Select / Confirm
- **Mouse** – Interact with GUI elements
- **Escape** – Open Pause Menu
  
# 🧠 Developer Insight
This project was developed to demonstrate:
- Working in a team
- Strong Object-Oriented Programming (OOP) principles
- Game loop management using SFML
- Dynamic UI rendering and interaction
- Scene-based architecture and resource management
- Debugging and exception safety
  
# 📫 Contact
Created by Tia Dindayal and other associates as part of a university Computers Module.
Email: tiadindayal@gmail.com
