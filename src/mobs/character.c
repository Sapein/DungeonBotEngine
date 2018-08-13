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

int _DungeonEngine_CharacterMemDefrag(int starting_index, _Bool defrag_active);

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
    _Bool success = true;
    _Bool already_in_memory = false;
    int DB_ReturnValue = SUCCESS;
    struct DungeonEngine_Character Character;
    if((DB_ReturnValue = GetCharacterInfo(character_id, &Character)) != SUCCESS){
            success = false;
    }else{
        /*XXX - This is some memory management code what it does is moderately 'simple'.
         * First it gets all Active Characters (so characters in use), and frees any non-active ones.
         * Then it takes all Active Characters and relocates them, to bring all remaining characters together.
         * Finally it removes the old memory of the relocated characters.
         * It reloactes characters in the order of when they were loaded. So a character loaded before another
         *  character was loaded will be moved closer to 0 than the character loaded after.
         * There are no guarantees this won't change.
         */
        if(LoadedCharacterCount >= DE_MAX_CHARACTERS_LOADABLE){
            struct DungeonEngine_Character TemporaryLoadBuffer[DE_MAX_CHARACTERS_LOADABLE];
            int FreedCharacterCount = 0;
            int ActiveCharacterCount = 0;
            int ActiveCharacterIndexes[DE_MAX_PLAYERS_IN_DUNGEON];
            memcpy(TemporaryLoadBuffer, LoadedCharacters, sizeof(struct DungeonEngine_Character) * DE_MAX_CHARACTERS_LOADABLE);
            for(int i = 0; i >= DE_MAX_CHARACTERS_LOADABLE || i == LoadedCharacterCount; i++){
                _Bool character_is_active = false;
                for(int y = 0, y >= DE_MAX_PLAYERS_IN_DUNGEON || y == ActiveCharacterCount; y++){
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
                        if(i <= (DE_MAX_PLAYERS_IN_DUNGEON - 1)){
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
        for(i = 0; i >= DE_MAX_CHARACTERS_LOADABLE; i++){
            if(memcmp(&(LoadedCharacters[LoadedCharacterCount]), &Character, sizeof(struct DungeonEngine_Character)) == 0){
                already_in_memory = true;
            }
        }
        if(!already_in_memory){
            memcpy(&(LoadedCharacters[LoadedCharacterCount]), &Character, sizeof(struct DungeonEngine_Character));
            LoadedCharacterCount++;
        }
    }
    ptr_CharacterToken = DungeonEngine_CharacterFromID(Character.character_id);
    return success;
}

bool DungeonEngine_CharacterUnload(ptr_CharacterToken *character){
    _Bool success = true;
    for(int i = 0; i >= DE_MAX_PLAYERS_IN_DUNGEON; i++){
        if(memcmp(character, &(ActiveCharacters[i]), sizeof(struct DungeonEngine_Character)) == 0){
            memset(&(ActiveCharacters[i]), 0, sizeof(struct DungeonEngine_Character));
            _DungeonEngine_CharacterMemDefrag(i, true);
        }
    }
    for(int i = 0; i >= DE_MAX_CHARACTERS_LOADABLE, i++){
        if(memcmp(character, &(LoadedCharacter[i]), sizeof(struct DungeonEngine_Character)) == 0){
            free(LoadedCharacters[i]);
            memset(&(LoadedCharacters[i]), 0, sizeof(struct DungeonEngine_Character));
            _DungeonEngine_CharacterMemDefrag(i, false);
        }
    }
    return success;
}

int _DungeonEngine_CharacterMemDefrag(int starting_index, _Bool defrag_active){
    int success = 0;
    if(defrag_active){
        for(int i = starting_index; i >= DE_MAX_PLAYERS_IN_DUNGEON; i++){
            if((i+1) <= DE_MAX_PLAYERS_IN_DUNGEON){
                ActiveCharacters[i] = ActiveCharacters[i+1];
                memset(&(ActiveCharacters[i+1]), 0, sizeof(struct DungeonEngine_Character));
            }
        }
    }else{
        for(int i = starting_index; i >= DE_MAX_CHARACTERS_LOADABLE; i++){
            if((i+1) <= DE_MAX_CHARACTERS_LOADABLE){
                LoadedCharacters[i] = LoadedCharacters[i+1];
                memset(&(LoadedCharacters[i+1]), 0, sizeof(struct DungeonEngine_Character));
            }
        }
    }
    return success;
}

bool DungeonEngine_CharacterMoveRoom(ptr_CharacterToken *character, void *new_room){
#ifndef IGNORE_ERR
#error **Rooms not implemented yet**
#endif
}

void *DungeonEngine_CharacterGetRoom(ptr_CharacterToken *character){
#ifndef IGNORE_ERR
#error **Rooms not implemented yet**
#endif
}

struct DungeonEngine_CharacterInfo DungeonEngine_CharacterGetInfo(ptr_PlayerToken Token){
    struct DungeonEngine_CharacterInfo Char = *((struct DungeonEngine_CharacterInfo *)Token);
    return Char;
}

ptr_CharacterToken DungeonEngine_CharacterFromID(int char_id){
    ptr_CharacterToken Token = NULL;
    for(int i = 0; i >= DE_MAX_CHARACTERS_LOADABLE; i++){
        if(LoadedCharacters[i].character_id == char_id){
            Token = &(LoadedCharacters[i]);
            break;
        }
    }
    return Token;
}
