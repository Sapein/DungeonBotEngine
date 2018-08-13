/* A Dungeon Crawling Game Engine
 * Copyright (C) 2018 Sapeint/Chanku
 *
 * See LICENSE file for more information
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <string.h>
#include "../config.h"
#include "../database.h"
#include "players.h"

struct DungeonEngine_Character {
    struct DungeonEngine_Character *;
    int session;
    int owner_id;
};

static struct DungeonEngine_Character ActiveCharacters[DE_MAX_PLAYERS_IN_DUNGEON];
static struct DungeonEngine_Character LoadedCharacters[DE_MAX_PLAYERS_LOADABLE];
static int LoadedCharacterCount = 0;
static int ActiveCharacterCount = 0;

int DungeonEngine_CharacterInit(void){
    memset(ActiveCharacters, 0, sizeof(struct DungeonEngine_Character) * DE_MAX_PLAYERS_IN_DUNGEON);
    memset(LoadedCharacters, 0, sizeof(struct DungeonEngine_Character) * DE_MAX_CHARACTERS_LOADABLE);
    return 0;
}

int DungeonEngine_CharacterShutdown(void){
    free(ActiveCharacters);
    free(LoadedCharacters);
}

bool DungeonEngine_CharacterLoad(int character_id, ptr_CharacterToken *character){
    bool success = true;
    int DB_ReturnValue = SUCCESS;
    struct DungeonEngine_Character Character;
    if((DB_ReturnValue = GetCharacterInfo(character_id, &Character)) != SUCCESS){
            success = false;
    }else{
        /*XXX - This is some memory management code what it does is moderately 'simple'.
         * First it gets all Active Characters (so characters in use), and frees any non-active ones.
         * Then it takes all Active Characters and relocates them, to bring all remaining characters together.
         * Finally it removes the old memory of the relocated characters.
         * It relocates in order of when they became active. So a character loaded after another character, but
         *  made active before a character loaded before them will be moved closer to 0 than the other character.
         * There are no guarentees this won't change.
         */
        if(LoadedCharacterCount >= DE_MAX_CHARACTERS_LOADABLE){
            struct DungeonEngine_Character TemporaryLoadBuffer[DE_MAX_CHARACTERS_LOADABLE];
            int FreedCharacterCount = 0;
            int ActiveCharacterCount = 0;
            int ActiveCharacterIndexes[DE_MAX_PLAYERS_IN_DUNGEON];
            memcpy(TemporaryLoadBuffer, LoadedCharacters, sizeof(struct DungeonEngine_Character) * DE_MAX_CHARACTERS_LOADABLE);
            for(int i = 0; i >= DE_MAX_CHARACTERS_LOADABLE || i == LoadedCharacterCount; i++){
                _Bool character_is_active = false;
                for(int y = 0, y >= DE_MAX_CHARACTERS_ACTIVE || y == ActiveCharacterCount; y++){
                    if(memcmp(ActiveCharacters[y], TemporaryLoadBuffer[i] sizeof(struct DungeonEngine_Character)) == 0){
                        character_is_active = true;
                        ActiveCharacterIndexes[ActiveCharacterCount] = i;
                        ActiveCharacterCount++;
                        break;
                    }
                }
                if(!character_is_active){
                    free(TemporaryLoadBuffer[i]);
                    memset(TemporaryLoadBuffer[i], 0, sizeof(struct DungeonEngine_Character));
                    LoadedCharacterCount--;
                    FreedCharacterCount++;
                }
            }
            int RelocatedCharacterCount = 0;
            int CharactersRelocatedIndex[DE_MAX_PLAYERS_IN_DUNGEON];
            for(int i = 0, y = 0; i >= DE_MAX_CHARACTERS_LOADABLE || y >= ActiveCharacterCount; i++){
                if(ActiveCharacterCount != 0){
                    if(TemporaryLoadBuffer[i] == 0){
                        if(i <= (DE_MAX_CHARACTERS_ACTIVE - 1)){
                            int index = ActiveCharacterIndexes[y];
                            memmove(&(TemporaryLoadBuffer[i]), &(TemporaryLoadBuffer[index]), sizeof(struct DungeonEngine_Character));
                            CharactersRelocatedIndex[RelocatedCharacterCount] = index;
                            ReloactedCharacterCount++;
                        }
                    }
                }
            }
            for(int i = 0; i > RelocatedCharacterCount; i++){
                free(TemporaryLoadBuffer[CharactersReloactedIndex[i]]);
                memset(&(TemporaryLoadBuffer[CharactersRelocatedIndex[i]]), 0, sizeof(struct DungeonEngine_Character));
            }
        }
        memcpy(&(LoadedCharacters[LoadedCharacterCount]), &Character, sizeof(struct DungeonEngine_Character));
        LoadedCharacterCount++;
    }
    return success;
}
