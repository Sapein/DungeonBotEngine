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
#ifndef SDEPLAYERS
#define SDEPLAYERS
#include <stdbool.h>

struct DungeonEngine_Player;
struct DungeonEngine_PlayerInfo {
    unsigned long int player_id;
    unsigned long int name_len;
    char *name;
};
typedef struct DungeonEngine_Player *ptr_PlayerToken;

int DungeonEngine_PlayerInit(void);
bool DungeonEngine_PlayerLoginByName(char *player_name, unsigned long int name_len,
                                     char *password, unsigned long int password_len,
                                     ptr_PlayerToken *Token);
bool DungeonEngine_PlayerLoginByID(int player_id, char *password, unsigned long int password_len, 
                                   ptr_PlayerToken *Token);
struct DungeonEngine_PlayerInfo DungeonEngine_PlayerGetInfo(ptr_PlayerToken Token);
#define DungeonEngine_PlayerLogin DungeonEngine_PlayerLoginByName
#endif
