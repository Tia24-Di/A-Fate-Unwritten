#ifndef TINMAN_NARRATIVE_H
#define TINMAN_NARRATIVE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Dialogue.h"
#include "BackgroundManager.h"

namespace TinmanNarrative {
    // -- get start narrative --
// returns the initial narrative introducing the Tinman and his predicament
    inline std::vector<Dialogue::DialogueLine> getStartNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
             "An abandoned house looms ahead, its windows shattered, walls overrun with ivy. "
             "You step forward, boots crunching softly on the gravel path, each step making the air feel colder. "
             "You push open the creaking door, hinges groaning in protest. The air is thick with dust and "
             "the faint scent of rust lingers. The room is in ruins with paint peeling off the walls and the "
             "scattered debris on the floor. Amid the wreckage stands the Tinman. He's perfectly still, his "
             "frame tall and weathered, limbs stiff with age and disuse."},

            { Dialogue::DialogueType::Dorothy, 
             "You must be the one Scarecrow told me about!" },

            { Dialogue::DialogueType::Tinman, 
             "Ahh yes, my old friend Scarecrow. I wish I could visit him again but you see "
             "...I cannot leave this place. " },

            { Dialogue::DialogueType::Dorothy, 
             "What? Why can't you leave? " },

            { Dialogue::DialogueType::Tinman, 
             "This house...it was once my home. I was forged here, raised by hands now "
             "long gone. Every corner holds a memory. But it's all crumbling now - broken, lost. "
             "My heart... It was here too. Not just the one I once had - but the one that made me feel. The "
             "one that reminded me I was alive. When the Wizard's curse fell upon this land, he didn't just "
             "rust my joints. He stole what I held most dear."},

            { Dialogue::DialogueType::Dorothy, 
             "Your heart..." },

            { Dialogue::DialogueType::Tinman, 
             "Yes. Without it, I cannot move freely. I am bound to this place, "
             "like a shadow nailed to a wall. I've searched through the dust and silence, but it's gone. "
             "Hidden...or broken. I cannot lift this curse until I find it."},

            { Dialogue::DialogueType::Narrator, "As you stand beside him, you realize this moment is different."
             "The Tinman isn't just trapped by rust - he's held in place by something deeper. His heart is missing, "
             "lost within the memories and ruins of his past. If you want to help him, you must guide him through "
             "the pieces of who he used to be, uncover the truth behind the Wizard's curse, and help him reclaim "
             "the heart that once gave him purpose." },

            { Dialogue::DialogueType::Narrator, "But be warned - once you answer the questions that will mend "
             "what was broken, the final test awaits." },

        };
    }
    // get the riddles for the tinman level
    inline std::vector<QuestionHandler::Question> getTinmanRiddles() {
        return {
            {"Once, I kept things off the floor,\nWith sturdy arms, I stood before.\n"
             "I held what hung with care and grace,\nNow forgotten in this hollow space. What am I ? ",
            {"A bookshelf","A mannequin","A coat rack","A grandfather clock"}, 2},

            {"I once helped clear the mess away,\nBut now I rest here every day.\n"
            "With bristles worn and handle light,\nI wait unseen, out of sight. What am I ? ",
            {"A vacuum cleaner","A mop","A dustpan","A broom"}, 3},

            {"I stood with strength, I bore the weight,\nA refuge for the weary to find rest.\n"
            "But time was cruel and sealed my fate,\nNow broken pieces echo what was best. What am I?",
            {"A broken chair","A bed frame","A park bench","A swing"}, 0},

            {"Through halls and steps, I led the way,\nHelping those whose legs would sway.\n"
            "Now tossed aside, I wait alone,\nForgotten where the dust has grown. What am I?",
            {"A crutch","A walking stick","An old shoe","A wheelchair"}, 1},
        };
    }
    // get the questions for the tinman level
    inline std::vector<QuestionHandler::Question> getTinmanQuestions() {
        return {
            {"Is hope worth holding onto... or will it only lead to disappointment?",
             {"Hope keeps us moving forward, even in the darkest times.",
              "Hope is a distraction. I will focus on what's real.",
              "I will hope... but only for small things"},
              0},

            {"What outlives everything: love or fear?",
             {"Fear. It commands respect.",
              "Love is safer but fear gets results.",
              "Love. Fear breaks; love rebuilds"},
              2},

            {"Friend or stranger who deserves to be saved?",
             {"I'd risk myself to save both. No one is 'less'.",
              "My friend. Bonds matter most",
              "Whoever I can reach first."},
              0},

            {"Should I feel... or calculate?",
             {"Calculate. Feelings mislead.",
              "Feeling is wisdom. Trust it.",
              "I'll lock away my feelings. They're too dangerous."},
              1},

            {"If peace meant losing the ability to feel... would it be worth it?",
             {"No. Peace without joy is just silence.",
              "Yes. Sacrificing emotion is a small price for peace",
              "Maybe... but I'd mourn what I lost"},
              0},

            {"If one life had to be sacrificed to save a thousand, would it be the right choice?",
             {"Sacrifices must be made for the greater good.",
              "No life should be weighed against another.",
              "I don't know... Some choices haunt you forever."},
              1},

            {"Should I show compassion... or enforce justice, even if it's harsh?",
             {"Compassion heals wounds. I will choose kindness over punishment.",
              "Justice must be served, no matter the cost.",
              "I'll be compassionate... but only if they deserve it"},
              0},

            {"Is it better to be vulnerable and open... or strong and guarded?",
             {"Strength is everything. I will guard my heart to protect myself.",
              "I will be strong... but let a few people in.",
              "Vulnerability is strength. It's how we connect with others."},
              2}
        };
    }
    // dialogue that appears before the riddles start
    inline std::vector<Dialogue::DialogueLine> getPreRiddleDialogue() {
        return{
            { Dialogue::DialogueType::Narrator, 
             "The Tinman's path has brought him here through questions that pieced together fragments of a forgotten heart. "
             "But the journey is not yet over. Ahead lies the final test: a series of riddles, bound by time. "
             "Answer swiftly, think clearly. In this final trial, hesitation may cost the Tinman his only chance at freedom. "
             "Though you may pause, the clock will not. The timer begins now, and time waits for no one..."
            }
        };
    }

    // -- get good end narrative --
// returns the narrative when the Tinman has successfully reclaimed his heart
    inline std::vector<Dialogue::DialogueLine> getGoodEndNarrative() {
        return {
            { Dialogue::DialogueType::Tinman, 
             "I... I can feel it. Something I thought I'd lost forever. It's not a heart "
             "of flesh... but something deeper. Compassion. Empathy. Humanity. Thank you, Dorothy! "
             "You've done more than free me - you've reminded me that love and loss are part of the same song. "
             "Now go, Oz still needs you! And this time, you are not alone." },

            {Dialogue::DialogueType::Narrator,
             "The Tinman is whole, his body restored, and his heart glowing with compassion and humanity. "
             "The abandoned house, once decaying and lifeless, has been renewed, reflecting the transformation within him. "
             "As Dorothy prepares to move forward, the Tinman understands that his journey takes him down a separate road. "
             "The empathy he has gained now fuels his desire to help others, guiding those lost in the Wizard's shadow with the compassion he once thought was lost."},
        };
    }
    // --get bad end narrative--
    // returns the narrative when the Tinman fails to reclaim his heart
    inline std::vector<Dialogue::DialogueLine> getBadEndNarrative() {
        return {
            { Dialogue::DialogueType::Tinman,
             "I... I can't feel it. Just hollow. Empty. I had hoped... "
             "perhaps this time would be different. But the curse still binds me. My heart remains lost,"
             "and I remain rooted here... just a hollow shell among memories." },

            { Dialogue::DialogueType::Dorothy,
              "Tinman, I'm sorry. But this isn't the end for you. You can still find your heart."},

            { Dialogue::DialogueType::Tinman,
             "... Maybe, one day, I'll find it again."},

            { Dialogue::DialogueType::Narrator,
             "The Tinman's body is whole, but the spark of life remains absent, "
             "leaving him cold and empty. Though he hasn't yet found the heart he seeks, the essence of his "
             "humanity still waits to be reignited within him. "
             "Dorothy holds onto the belief that the Tinman can one day rediscover what it means to truly live."},
        };
    }

// -- get background triggers --
// maps specific dialogue lines to background images
    inline void getBackgroundTriggers(BackgroundManager& bgManager) {
        bgManager["You must be the one"] = "resources/images/tinman_before.png";
        bgManager["I... I can feel it"] = "resources/images/tinman_after.png";
    }
}

#endif

