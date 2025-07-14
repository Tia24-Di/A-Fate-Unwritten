#ifndef GIFANIMATOR_H
#define GIFANIMATOR_H

// SFML and Standard Library Includes
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// manages the animation of a GIF-like sequence of frames
class GifAnimator {
public:
    // constructor: Initializes the animator with the frame prefix, total number of frames, and frame rate (default 10.0f)
    GifAnimator(const std::string& framePrefix, int totalFrames, float frameRate = 10.0f);
    
    bool loadFrames();                   // loads the frames from the provided frame prefix (returns true if successful)
    void update();                       // updates the current frame of the animation based on elapsed time
    void draw(sf::RenderWindow& window); // draws the current frame to the provided window

    void setLooping(bool shouldLoop);    // sets whether the animation should loop (true to loop, false to play once)
    bool isAnimationComplete() const;    // returns whether the animation is complete (all frames played)
    int getCurrentFrameIndex() const;    // gets the current frame's index

private:
    int currentLoopCount;           // counter to track how many times the animation has looped
    bool hasPlayedOnce() const {    // returns whether the animation has played at least once
        return currentLoopCount >= 1;
    }
    bool isFinished() const {       // checks if the animation is finished (last frame reached)
        return finished;
    }

    std::string framePrefix;            // frame prefix used to generate the frame filenames
    int totalFrames;                    // total number of frames in the animation
    int currentFrame;                   // current frame index
    float frameDuration;                // duration of each frame (in seconds)
    std::vector<sf::Texture> frames;    // vector to store the loaded frames as textures

    sf::Sprite sprite;                  // sprite used to display the current frame
    sf::Clock clock;                    // clock to keep track of time for frame updates

    bool framesLoaded;                  // flag indicating if the frames have been successfully loaded
    bool finished = false;              // flag indicating whether the animation has finished
    bool looping = false;               // flag indicating whether the animation should loop
};

#endif 
