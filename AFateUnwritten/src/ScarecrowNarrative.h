#ifndef SCARECROW_NARRATIVE_H
#define SCARECROW_NARRATIVE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Dialogue.h"
#include "BackgroundManager.h"

namespace ScarecrowNarrative {
    // -- get start narrative --
    // returns the initial narrative introducing the Scarecrow and his self-doubt
    inline std::vector<Dialogue::DialogueLine> getStartNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
              "Your journey through the world of Oz leads you to a vast, golden cornfield, stretching far into "
              "the horizon. The atmosphere is heavy with sorrow. Amidst the rustling stalks, you hear faint "
              "cries of distress. Following the sound, you stumble upon the Scarecrow: a tall, lifeless figure "
              "perched atop a wooden post, his face covered with a sorrowful expression. A murder of "
              "crows are perched around him, pecking at his clothes and mocking him for his apparent lack "
              "of intelligence."},

            { Dialogue::DialogueType::Narrator,
              "You step forward and he looks at you, his hollow eyes full of doubt."},

            { Dialogue::DialogueType::Scarecrow, "I-I can't do anything. I don't know enough... "
             "I'm just a stupid scarecrow"},

            { Dialogue::DialogueType::Dorothy, "Don't worry, you're not stupid. "
             "You just need to believe in yourself and unlock the knowledge within you. "
             "You're smarter than you think."},

            { Dialogue::DialogueType::Scarecrow, "How can I prove that? "},

            { Dialogue::DialogueType::Dorothy, "I can help you. We'll face this together. "
             "Let's answer some questions - tests of logic and knowledge. If you get enough right, "
             "your mind will awaken, and the crows won't be able to torment you any longer."},

            { Dialogue::DialogueType::Narrator,
             "The Scarecrow's expression shifts from despair to a flicker of hope. "
             "He nods slowly, but the doubt is still there nagging at him. Help Scarecrow answer some "
             "questions that will re-awaken his mind and prove to the crows that he is not a dummy after all."},

            { Dialogue::DialogueType::Scarecrow, "Okay, I'm ready. Let's do this!"}
        };
    }

    // -- get scarecrow's questions --
    // returns a set of logic and knowledge-based questions for the Scarecrow
    inline std::vector<QuestionHandler::Question> getScarecrowQuestions() {
        return {
              {"What is the value of y in the equation 2y + 8 = 18?", {"4", "5", "3", "10"}, 1},
              {"What is the next number in the sequence: 2, 5, 10, 17, ...?", {"26", "30", "24", "20"}, 0},
              {"What is the area of a triangle with base 12 cm and height 8 cm?", {"48 cm²", "60 cm²", "72 cm²", "36 cm²"}, 0},
              {"A $120 pair of shoes has 25% discount. What's the sale price?", {"$90", "$95", "$100", "$85"}, 0},
              {"What is the remainder of 167 ÷ 15?", {"12", "2", "6", "3"}, 1},
              {"Which is NOT a prime number?", {"1", "163", "79", "177"}, 3},
              {"What is the average of 16, 21 and 89?", {"42", "54.3", "31", "48.6"}, 0},
              {"What are the first 6 digits of pi?", {"3.14138", "3.14159", "3.14156", "3.14148"}, 1},
              {"What is the colour of emerald?", {"Blue", "Green", "Red", "Yellow"}, 1},
              {"What protects crops from birds?", {"Fertilizer", "Scarecrow", "Watering can", "Rake"}, 1},
              {"What is a tornado called?", {"Twister", "Cyclone", "Hurricane", "Blizzard"}, 0},
              {"How many hearts does an octopus have?", {"1", "3", "5", "6"}, 1},
              {"Which of these represents a journey or adventure?", {"Odyssey", "Routine", "Nap", "Hibernation"}, 0},
              {"Which of these is commonly found at the end of a rainbow in legends?", {"Pot of gold", "Treasure chest", "Magic sword", "Castle"}, 0},
              {"Which tool would you use to find your way if you're lost?", {"Shovel", "Compass", "Paintbrush", "Net"}, 1},
              {"Which animal is known as the King of the Jungle?", {"Elephant", "Lion", "Tiger", "Bear"}, 1},
              {"Which element has the symbol 'O'?", {"Gold", "Oxygen", "Osmium", "Opal"}, 1},
              {"What do you call a group of crows?", {"Flock", "Pack", "Murder", "Swarm"}, 2},
              {"What gas do plants use for photosynthesis?", {"Oxygen", "Nitrogen", "Hydrogen", "Carbon Dioxide"}, 3},
              {"Which bird can mimic speech?", {"Parrot", "Eagle", "Owl", "Sparrow"}, 0},
              {"How many legs does a spider have?", {"12", "10", "6", "8"}, 3},
              {"What is the value of 7 squared minus 4 squared?", {"33", "45", "49", "21"}, 0},
              {"Which season comes after summer?", {"Winter", "Autumn", "Spring", "Monsoon"}, 1},
              {"What do bees collect and use to make honey?", {"Sap", "Pollen", "Nectar", "Water"}, 2},
              {"Which material is often used to make small metal objects like food cans?", {"Steel", "Tin", "Iron", "Bronze"}, 1},
              {"What do you use to measure temperature?", {"Barometer", "Thermometer", "Caliper", "Compass"}, 1},
              {"How many sides does a hexagon have?", {"5", "6", "7", "8"}, 1},
              {"Which number is a multiple of both 8 and 7?", {"42", "35", "56", "24"}, 2},
              {"Which is greater: 3/4, 2/3, or 9/25?", {"3/4", "2/3", "They're equal", "9/25"}, 0},
              {"What does DNA stand for?", {"Deoxyribonucleic Acid", "Dichlorodiphenyltrichloroethane", "Dimethylaminoethanol", "Deuterium Nucleic Acid"}, 0},
              {"How many minutes are in two and a half hours?", {"120", "130", "150", "160"}, 2},
              {"Which of these is a leap year?", {"1198", "1904", "782", "2022"}, 1},
              {"How many bits are in a byte?", {"4", "8", "16", "32"}, 1},
              {"Which number is the square root of 81?", {"7", "9", "8", "6"}, 1},
              {"What is the Roman numeral for 50?", {"L", "C", "D", "V"}, 0},
              {"How many degrees are in a right angle?", {"90", "45", "180", "60"}, 0},
              {"What footwear is best for a long walk?", {"Slippers", "Boots", "High heels", "Sandals"}, 1},
              {"What do you call a magical person who casts spells?", {"Knight", "Wizard", "Merchant", "Pilot"}, 1},
              {"What do you call a small shelter made from sticks and leaves?", {"Igloo", "Cabin", "Hut", "Tent"}, 2},
              {"What grows from an acorn?", {"Pine tree", "Sunflower", "Oak tree", "Rose"}, 2},
              {"What do you call a path made of stones?", {"Highway", "Trail", "Brick road", "Paved road"}, 2},
        };
    }

    // -- get good end narrative --
    // returns the narrative when the Scarecrow has proven his intelligence and overcome his self-doubt
    inline std::vector<Dialogue::DialogueLine> getGoodEndNarrative() {
        return {
            { Dialogue::DialogueType::Scarecrow,
             "I did it! I feel so alive. I can think! I know things I never thought I could!"},

            { Dialogue::DialogueType::Dorothy, 
             "See? I told you, you had it in you."},

            { Dialogue::DialogueType::Narrator,
              "The crows grow interested in the Scarecrow's intelligence, befriending him "
              "and allowing him to now join their conversations."},

            { Dialogue::DialogueType::Scarecrow,
              "Thank you, Dorothy. The field needed care, and now I understand how to help it grow. "
              "Now go and save Oz! You definitely have the wits for it. Speaking of wits, the crows "
              "tell me there's an abandoned house somewhere around here. It's in need of a pickup but "
              "the one who serves there is unable to move! Please, help him like you helped me"},
        };
    }

    // -- get bad end narrative --
    // returns a narrative to be shown when the player doesn't help the Scarecrow successfully
    inline std::vector<Dialogue::DialogueLine> getBadEndNarrative() {
        return {
            { Dialogue::DialogueType::Scarecrow,
              "I... I tried, but I'm still... not sure. I can think a little, but it's not "
              "like I thought it would be. I don't feel... completely different."},

            { Dialogue::DialogueType::Narrator,
              "The crows, sensing his doubt, circle above them, mocking and cawing."},

            { Dialogue::DialogueType::Dorothy,
              "It's okay, Scarecrow. We didn't get everything right, but that doesn't mean "
              "you're not smart. You're still learning. You're not alone."},

            { Dialogue::DialogueType::Scarecrow,
              "But... the crows... they still make fun of me. How can I ever be smart enough if I can't prove it?"},

            { Dialogue::DialogueType::Dorothy,
              "You are smart, Scarecrow. It'll take time. Every question and challenge helps you grow. You're not "
              "broken, you're just in the middle of your journey."},

            { Dialogue::DialogueType::Scarecrow,
              "Thanks, Dorothy. Speaking of journey, the crows tell me there's an abandoned house somewhere around here. "
              "It's in need of a pickup but the one who serves there is unable to move... they're saying "
              "he's defective like me. Please help him, don't let him be like me."},
        };
    }

    // -- get background triggers --
    // maps certain dialogue lines to specific background images
    inline void getBackgroundTriggers(BackgroundManager &bgManager) {
        bgManager["You step forward"] = "resources/images/scarecrow_before.png";
        bgManager["I did it!"] = "resources/images/scarecrow_after.png";
    }
}

#endif
