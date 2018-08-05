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
#include "players.h"

static pid_t child_pid = 0;
static _Bool turn_based = true;

enum DungeonEngine_EnErrors DungeonEngine_Init(_Bool turn_based){
    /* Right here is where world files and the like should be loaded,
     * along with connecting to/loading databases
     */
    int code = SUCCESS;
    turn_based = turn_based;
    if((code = DungeonEngine_PersonInit()) != SUCCESS){
        return code;
    }
    return SUCCESS;
}

enum DungeonEngine_EnErrors DungeonEngine_Turn(void){
    if(turn_based){
        return SUCCESS;
    }else{
        return NO_TURNS;
    }
}

enum DungeonEngine_EnErrors DungeonEngine_Start(void){
    if(!turn_based){
        bool running = true;
        while(running){
            ;
        }
        return SUCCESS;
    }else{
        return TURN_ONLY;
    }
}

enum DungeonEngine_EnErrors DungeonEngine_ForkStart(void){
    if(!turn_based){
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
    else{
        return TURN_ONLY;
    }
}
