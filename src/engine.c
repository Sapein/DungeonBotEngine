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
#include <stdbool.h>
#include <unistd.h>
#include "engine.h"

static pid_t child_pid = 0;
enum DungeonEngine_EnErrors DungeonEngine_Init(void){
    /* Right here is where world files and the like should be loaded,
     * along with connecting to/loading databases
     */
    return SUCCESS;
}

enum DungeonEngine_EnErrors DungeonEngine_Start(void){
    bool running = true;
    while(running){
    }
    return SUCCESS;
}

enum DungeonEngine_EnErrors DungeonEngine_ForkStart(void){
    pid_t fork_code = fork();
    switch(fork_code){
        case -1:
            return FORK_FAIL;
        case 0:
            break;
        default:
            child_pid = fork_code;
            goto end;
    }

end:
    return SUCCESS;
}
