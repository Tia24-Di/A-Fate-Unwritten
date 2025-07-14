#include "Intro.h"
#include "IntroNarrative.h"

using namespace std;
using namespace sf;

// constructor: initialize Intro scene, load resources, and set up resizing
Intro::Intro(RenderWindow& window) : Scene(window) {
    loadResources(); 
    resize();       
}

// load background and dialogue lines for the intro
void Intro::loadResources() {
    bgManager.setDefault("resources/images/castle_before.png"); 
    auto start = IntroNarrative::getStartNarrative();           
    introDialogues.assign(start.begin(), start.end());
    loadCurrentDialogue();                                     
}

// load the current dialogue line into the dialogue system
void Intro::loadCurrentDialogue() {
    const auto& line = introDialogues[currentDialogueIndex];
    introDialogue = make_unique<Dialogue>(window, line.speaker, line.text);
}

// render the scene: background first, then dialogue
void Intro::render() {
    window.clear();         
    bgManager.draw(window); 
    introDialogue->render(); 
    window.display();       
}

// update the dialogue (for typing effect, speaker change, etc.)
void Intro::update() {
    introDialogue->update();
}

// reset the scene back to the beginning of the intro
void Intro::reset() {
    introDialogue->reset();  
    currentDialogueIndex = 0; 
    loadCurrentDialogue();    
}

// check if the current dialogue text has finished displaying
bool Intro::isTextComplete() const {
    return introDialogue->isTextComplete();
}

// resize background and dialogue UI if window size changes
void Intro::resize() {
    bgManager.resize(window);          // resize background properly
    if (introDialogue) {
        introDialogue->resize(window); // resize dialogue text and box if needed
    }
}
