#include "GifAnimator.h"
#include "LoadResources.h" 

using namespace std;
using namespace sf;

// constructor: initialize prefix for frames, total number of frames, and frame rate
GifAnimator::GifAnimator(const string& prefix, int total, float frameRate)
    : framePrefix(prefix), totalFrames(total), frameDuration(1.0f / frameRate),
    currentFrame(0), framesLoaded(false), looping(false), finished(false), currentLoopCount(0) {}

// load all frame textures based on naming convention
bool GifAnimator::loadFrames() {
    for (int i = 0; i < totalFrames; ++i) {
        Texture texture;
        string filename = framePrefix + to_string(i) + ".png";
        resourceLoader(texture, filename); 
        frames.push_back(texture);
    }

    // if frames were loaded successfully, set up the initial sprite
    if (!frames.empty()) {
        sprite.setTexture(frames[0]);
        FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2, bounds.height / 2); // center origin
        framesLoaded = true;
    }

    return framesLoaded;
}

// enable or disable looping behavior
void GifAnimator::setLooping(bool shouldLoop) {
    looping = shouldLoop;
}

// update the animation frame based on elapsed time
void GifAnimator::update() {
    if (frames.empty() || finished) return;

    if (clock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame++;

        if (currentFrame >= frames.size()) {
            if (looping) {
                currentFrame = 0;  // restart animation from the beginning
                currentLoopCount++;
            }
            else {
                currentFrame = frames.size() - 1;  // stay on the last frame
                finished = true;
            }
        }

        clock.restart(); // reset timer for next frame
    }
}

// draw the current frame, scaled to fit the window size
void GifAnimator::draw(RenderWindow& window) {
    if (frames.empty()) return;

    Sprite sprite;
    sprite.setTexture(frames[currentFrame]);

    // get window size and texture size
    Vector2u windowSize = window.getSize();
    Vector2u textureSize = frames[currentFrame].getSize();

    // calculate scale factors to fill the window
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    sprite.setScale(scaleX, scaleY); // apply scaling
    window.draw(sprite);
}

// check if the animation has completed
bool GifAnimator::isAnimationComplete() const {
    return finished;
}

// return the current frame index
int GifAnimator::getCurrentFrameIndex() const {
    return currentFrame;
}
