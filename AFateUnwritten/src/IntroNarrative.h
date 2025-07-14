#ifndef INTRO_NARRATIVE_H
#define INTRO_NARRATIVE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Dialogue.h"

namespace IntroNarrative {
    // -- get start narrative --
    // returns the introductory narrative of the story that sets the stage for Dorothy's quest
    inline std::vector<Dialogue::DialogueLine> getStartNarrative() {
        return {
            { Dialogue::DialogueType::Narrator,
               "The land of Oz, once vibrant and full of magic, has been cast into darkness. "
               "The Wizard, once a hero who brought hope to the people, sought even more power to help the land grow. "
               "But when his ambitious spell went horribly wrong, it didn't just increase his strength - it twisted his very soul. "
               "Now, the Wizard is no longer the protector of Oz, but its most dreaded villain. "
               "A sinister force has swept across the land, leaving despair and misery in its wake. "
               "The once joyful inhabitants of Oz are now trapped in a world of hopelessness, "
               "their spirits crushed under the weight of the Wizard's dark magic. No one is immune to the curse... except one. "
               "You, Dorothy, are the only one who hasn't fallen victim to the Wizard's twisted spell. Why? Because you don't belong to Oz. "
               "You were pulled into this strange world by a freak tornado from your home - and now, you're trapped in a coma in the real world, "
               "with your only chance to wake up lying in the fate of Oz. Now, it's up to you to take action. "
               "Your quest: Purify the Wizard, free your friends - the courageous Lion, the thoughtful Scarecrow, and the steadfast Tinman - "
               "and restore peace to the land so you can return home. "
               "But beware - the Wizard's forces are everywhere, and his magic is as dangerous as it is deceptive. "
               "Only you can break the curse and save Oz... but every step will bring you closer to the dark heart of the Wizard himself. "
               "Are you ready to face the darkness? Your adventure begins now." }
        };
    }
}
#endif

