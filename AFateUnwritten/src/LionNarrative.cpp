// LionNarrative.cpp
# include "LionNarrative.h"

LionNarrative::LionNarrative() {
    startNarrative = {
        { Dialogue::DialogueType::Narrator, "The Barren Forest is eerily quiet, but that silence is soon shattered by a sharp, desperate cry." },
        { Dialogue::DialogueType::Narrator, "It's the Cowardly Lion. You find him in a painful predicament, his mighty paw caught in a deadly bear trap." },
        { Dialogue::DialogueType::Narrator, "The weight of the trap is too heavy, and he's struggling to free himself." },
        { Dialogue::DialogueType::Lion, "\"I... I don't know if I can,\" the Lion whispers, pain and doubt in his voice." },
        { Dialogue::DialogueType::Narrator, "His instinct is to stay still, to hide from the world, to wait until the poachers move on." },
        { Dialogue::DialogueType::Lion, "\"If I try to free myself, I will make things worse... but if I don't, they are gone forever.\"" },
        { Dialogue::DialogueType::Narrator, "The Lion needs your help. He's trapped, both physically and emotionally." }
    };

    endNarrative = {
        { Dialogue::DialogueType::Narrator, "The lion reaches his family, ready to die and protect his loved ones, battling as many poachers to protect them." },
        { Dialogue::DialogueType::Lion, "\"I... I did it! I stood my ground! I fought for my family!\"" },
        { Dialogue::DialogueType::Lion, "\"And you Dorothy... you helped me see the strength that was in me all along.\"" },
        { Dialogue::DialogueType::Lion, "\"Now, go show that wicked wizard what a real hero looks like!\"" }
    };

    backgroundChanges = {
        {3, "resources/free_lion.png"},
        {1000, "resources/bye_lion.png"}
    };
}

const std::vector<std::pair<Dialogue::DialogueType, std::string>>& LionNarrative::getStartNarrative() const {
    return startNarrative;
}

const std::vector<std::pair<Dialogue::DialogueType, std::string>>& LionNarrative::getEndNarrative() const {
    return endNarrative;
}

std::unordered_map<size_t, std::string> LionNarrative::getBackgroundChanges() const {
    return backgroundChanges;
}
