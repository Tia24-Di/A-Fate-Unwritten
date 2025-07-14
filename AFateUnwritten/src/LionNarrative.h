#ifndef LION_NARRATIVE_H
#define LION_NARRATIVE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Dialogue.h"

namespace LionNarrative {
    // -- get start narrative --
    // returns the initial narrative introducing the Lion and his predicament
    inline std::vector<Dialogue::DialogueLine> getStartNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
              "The Barren Forest is eerily quiet, but that silence is soon shattered by a sharp, desperate cry. "
              "It's the Cowardly Lion. You find him in a painful predicament, his mighty paw caught in a deadly bear trap. "
              "The weight of the trap is too heavy, and he's struggling to free himself. "
              "But it's not the trap that terrifies him - it's the knowledge that his family is still in danger. "
              "An army of poachers are nearby, and his loved ones are vulnerable, lost somewhere in the forest, being hunted down. "
              "The Lion is torn. His instinct is to stay still, to hide from the world and wait until the poachers move on. "
              "But he knows he can't let his family suffer. He wants to escape but the fear of failure, of getting hurt holds him back."},
            { Dialogue::DialogueType::Lion, "I... I don't know if I can. "
              "If I try to free myself, "
              "I will make things worse... but if I don't, they are gone forever."},
            { Dialogue::DialogueType::Narrator, "As you approach, you realize that this moment is crucial. The Lion needs your help. "
              "He's trapped, both physically and emotionally. If you want to free him, "
              "you must help him confront his deepest fears and decide whether to face the danger for the sake of his family "
              "or give in to his cowardice and stay trapped." }
        };
    }

    // -- get lion's questions --
    // returns a set of questions the Lion will face in the story
    inline std::vector<QuestionHandler::Question> getLionQuestions() {
        return {
              {"Lion, what do you think bravery really means?", {"Bravery isn't about never feeling fear - it's about acting despite it.", "Bravery is never being afraid. If you feel fear, you're weak.",  "Bravery means fighting anything that stands in your way, no matter what."}, 0},
              {"Lion! You're trapped in a bear trap, and the poachers are getting closer! \nYour family is still out there, and they need your help. You're scared, \nbut you know you have to make a choice. What will you do?", {"Stay quiet and wait for someone else to rescue me.", "Struggle to free myself and go save my family, even if it means greater pain for me.", " Give up and accept my fate; there's no hope for my family."}, 1},
              {"Lion, do you think it's possible to be afraid and still be brave? ", {"Yes! True courage is doing what's right, even when fear tries to stop you.",  "Maybe... but sometimes fear is a sign that you should run away instead of fighting." , "No, fear means you're a coward. If you were truly brave, you wouldn't be scared at all."}, 0},
              {"Lion, can you hear the poachers? They're getting closer! And you're hiding\nin the trees!? Your family is in danger, but you're afraid of being caught. \nDo you choose to stay hidden and save yourself?", {"Yes","No"}, 1},
              {"You have jumped out of hiding from the tree and attacked a poacher, disarming him. \nWould you kill him, or let him run away?",  {"Kill him", "Let him go"}, 1},
              {"You're almost there Lion! A huge storm is going to hit, with vicious lightning and \nheavy rain. Your original plan to reach safety has become increasingly difficult. \nWhat will you do? ", {"Panic and run away as far as possible from the storm to not get struck by lightning.", "Take shelter under the nearest tree and hope the storm will pass quickly.", "Find a new route and adapt to the situation, its the only way forward for now."}, 2},
              {"Think, Lion, have you ever done something brave before? Maybe protect someone,\neven when you were scared ? ", {"No, I usually freeze when I'm afraid. I let others take the lead.", "Yes... I once shielded my cubs from a storm, even though the thunder made me tremble.", "I don't remember... but if I had, it probably wasn't enough."}, 1},
              {"The final test lion! You have to cross a narrow, rickety bridge to reach your family. \nThe bridge creaks and looks like it might collapse at any moment. You're terrified,\nbut time is running out. What will you do?", {"Turn around and look for another way, no matter how long it takes.", "Wait on the edge and hope someone else comes to help me cross.", "Cross the bridge slowly, step by step, even though you're trembling."}, 2},
        };
    }

    // -- get retry narrative --
    // returns a narrative to be shown when the player doesn't help the Lion successfully
    inline std::vector<Dialogue::DialogueLine> getRetryNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
              "I'm afraid you didn't quite manage to help the Lion face his fears. "
              "Bravery isn't about guessing - it's about understanding. Let's try again and see if you can guide him better this time." }
        };
    }

    // -- get end narrative --
    // returns the narrative when the Lion has successfully overcome his fears
    inline std::vector<Dialogue::DialogueLine> getGoodEndNarrative() {
        return {
            { Dialogue::DialogueType::Lion, "I did it, I stood my ground! I fought for my family! And you Dorothy... you helped "
            "me see the strength that was in me all along, removing all my fears. I saved my family "
            "because of you. Thank you, my friend, your bravery far exceeds any human I know. Now, go "
            "show that wicked wizard what a real hero looks like! But wait, what's that sound? Hmmm... sounds like some crows are nearby - "
            "that can't be good. You should probably go check it out. Farewell and good luck on the rest of your journey!"}
        };
    }

    // -- get background triggers --
    // maps certain dialogue lines to specific background images
    inline void getBackgroundTriggers(BackgroundManager& bgManager) {
        bgManager["As you approach"] = "resources/images/lion_before.png";
        bgManager["I did it"] = "resources/images/lion_after.png";
    }
}

#endif
