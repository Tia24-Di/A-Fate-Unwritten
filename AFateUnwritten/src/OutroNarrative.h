#ifndef OUTRO_NARRATIVE_H
#define OUTRO_NARRATIVE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Dialogue.h"
#include "BackgroundManager.h"

namespace OutroNarrative {
    // -- get good ending narrative --
    // returns the narrative for the good ending where Oz is freed, and Dorothy leaves Oz
    inline std::vector<Dialogue::DialogueLine> getGoodEndNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
              "As the last traces of dark magic dissolved into the air, the skies above Oz clear, "
              "revealing a radiant, endless blue. You approach the Wizard's castle when suddenly the Wizard, "
              "now purified and free from the corruption that once consumed him, stands tall before you as a "
              "beacon of hope. The land of Oz, once shadowed by fear and despair, blossomed with vibrant colors "
              "and renewed life. Its people are now free from the corruption that plagued them for all these years. "
              "Together, they had overcome impossible odds, and now Oz was truly free."},

            { Dialogue::DialogueType::Wizard,"Dorothy... I owe you more than words can express. "
             "You've done what I could not - faced the darkness within me and set me free. I was lost, "
             "consumed by greed and pride. But you... you showed me what true strength looks like."},

            { Dialogue::DialogueType::Dorothy,"It wasn't just me. It was all of us - Scarecrow, Tinman, Lion, "
             "and even you, deep down. You just needed to remember who you really are."},

            { Dialogue::DialogueType::Wizard,"You're wiser than you know, Dorothy. Oz is free because of you. "
             "The skies are clear, the people are safe, and for the first time in years, I feel... peace. "
             "But tell me, what will you do now? Will you stay in Oz, where you've become a hero?"},

            { Dialogue::DialogueType::Dorothy,"I love Oz, and I'll always carry it in my heart. "
             "But my place is back in Kansas, with my family. I miss them more than anything."},

            { Dialogue::DialogueType::Wizard,"Then let me leave you with some lasting words; Oz will always "
            "be a part of you! These friends you've made, and the bonds you've formed will never break. We too "
            "shall always hold you dear in our hearts."},

            { Dialogue::DialogueType::Dorothy,"Thank you. I'll never forget you-or any of this. "
             "I will never forget about Oz."},

            { Dialogue::DialogueType::Wizard,"And we too shall never forget your kindness. Now, go home. "
             "Your family is waiting."},

            { Dialogue::DialogueType::Narrator,
              "Dorothy smiled, knowing her journey had come to an end. Suddenly, a strike of lightning hits "
              "her and in an instant, Dorothy felt herself spinning through a whirlwind of light and sound. "},

            { Dialogue::DialogueType::Narrator,
              "When she opened her eyes, she was lying in her bed, the familiar walls of her Kansas "
              "farmhouse surrounding her. The storm outside had passed, and the sun streamed through the "
              "windows, casting a warm glow over the room. Her family rushed to her side, their faces filled "
              "with relief and joy. Dorothy blinked; the memories of Oz still vivid in her mind. She looked "
              "at her family and smiled, knowing she was exactly where she belonged. Though her adventure "
              "in Oz had ended, the lessons she learned and the friendships she forged would stay with her" 
              "forever. And as she gazed out at the clear Kansas skies, she felt a quiet sense of peace, "
              "knowing that somewhere beyond the rainbow, her friends were thriving in a land of magic and wonder."},
        };
    }

    // -- get bad ending narrative --
    // returns the narrative for the bad ending where Oz is consumed by darkness and Dorothy fails
    inline std::vector<Dialogue::DialogueLine> getBadEndNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
              "Darkness ravaged the land of Oz, its chaotic depths consuming everything in its path. "
              "The lion, once triumphant in saving his family from poachers, now cowers in fear as another group "
              "threatens, his courage faltering. The scarecrow, despite his valiant efforts to boost his confidence "
              "and intellect, remains paralyzed by self - doubt, helpless against the crows that torment him. "
              "The Tinman lies motionless, his metallic body slowly rusting away, a poignant reminder of his lost "
              "vitality. Meanwhile, the Wizard's soul is consumed by the very evil he unleashed, his dark magic draining "
              "the last vestiges of joy and brightness from Oz. You stand outside the castle and spot the wizard from "
              "a distance."},

            { Dialogue::DialogueType::Wizard,
            "Dorothy, you've failed. Your attempts to save Oz have been...amusing, "
            "but ultimately futile. The darkness will consume everything, and chaos will reign."},

            { Dialogue::DialogueType::Dorothy,
            "No! I won't let that happen. I've seen the good in the people here, "
            "and I know they can be saved."},

            { Dialogue::DialogueType::Wizard,
            "Good? Ha! Your friends are still the same cowardly, foolish, and scrap creatures they've always been. "
            "You've changed nothing."},

            { Dialogue::DialogueType::Dorothy,
            "That's not true! They've grown, and they've learned. And I'll prove it to you, "
            "no matter what it takes. I'll spend my life fighting for Oz, and I won't give up."},

            { Dialogue::DialogueType::Wizard,
            "Your life? Ah, Dorothy... watch as the light fades from this land. "
            "Watch as my curse drains the life force from every living thing... including you. "
            "As your fate is much worse than any curse I cast upon the creatures of Oz."},

            { Dialogue::DialogueType::Dorothy,
            "What do you mean?"},

            { Dialogue::DialogueType::Wizard,
            "Goodbye forever, Dorothy."},

            { Dialogue::DialogueType::Narrator,
              "The silence around her deepens, becoming a palpable, oppressive force. The wind howls, its "
              "fury intensifying as the skies darken and clouds converge. Thunder booms, shaking the air, "
              "and Dorothy's eyes widen in terror as she gazes upward. A blinding flash of light hurtles "
              "toward her, and she squeezes her eyes shut, feeling the energy and heat coursing through her "
              "veins like a wild, unstoppable current. "},
        };
    }

    // -- get background triggers --
    // maps certain dialogue lines to specific background images
    inline void getBackgroundTriggers(BackgroundManager& bgManager) {
        bgManager["When she opened"] = "resources/images/dorothy_house.png";
    }
}

#endif

