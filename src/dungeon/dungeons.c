#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../config.h"
#include "dungeons.h"

/* struct DungeonEngine_Dungeon { */
/*     int dungeon_id; /1* This should be assigned randomly, this is linked to the session ID *1/ */
/*     int seed; /1* This is randomly generated, but is stored to allow for reproducable testing *1/ */
/*     struct DungeonEngine_Room *entrance; */
/*     struct DungeonEngine_Room *exit; */
/* }; */

static unsigned long int Generation_Seed = 1;
void DungeonEngine_SetDungeonSeed(unsigned long int new_seed){
    Generation_Seed = new_seed;
}

void DungeonEngine_RandomDungeonSeed(void){
    Generation_Seed = time(NULL);
}

int DungeonEngine_DungeonGenerate(struct DungeonEngine_RoomsLoaded *rooms, union DungeonEngine_Dungeon){
    int dungeon[DE_GENERATION_WIDTH][DE_GENERATION_HEIGHT];
    int x, y, count = 0;
    int dungeon_id = 0;

    srand(Generation_Seed);
    dungeon_id = rand(); /* First number is the Dungeon ID */
    x = 0;
    y = DE_GENERATION_HEIGHT / 2;
    dungeon[x][y] = true;
    while(count <= DE_MAX_ROOMS){
        int new_direction = 2; /* 0 = North, 1 = South, 2 = East, 3 = West */
        switch(new_direction){
            case 3:
                break;
            case 2:
                y++;
                dungeon[x][y] = true;
                break;
            case 1:
                break;
            case 0:
                break;
            default:
                return 100;
        }
    }


    return 0;
    /* Old code 2 */
    enum room_entrances { NONE, NORTH_ONLY, SOUTH_ONLY, EAST_ONLY, WEST_ONLY,
                          NORTH_SOUTH, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST, EAST_WEST,
                          NORTH_SOUTH_EAST, NORTH_SOUTH_WEST, NORTH_EAST_WEST, SOUTH_EAST_WEST,
                          OPEN_ALL, EXIT, MAX_SPACE}; /* MAX_SPACE is not actually a room type, it is EXIT, but is always at the end */
    enum room_entrances dungeon[DE_GENERATION_WIDTH][DE_GENERATION_HEIGHT];
    enum room_entrances room_gen = NONE;
    int x, y, count = 0;
    int dungeon_id = 0;


    /* Place entrance in the middle of the X space */
    x = DE_GENERATION_HEIGHT/2;
    y = 0;
    dungeon[x][y] = ENTRANCE;
    y++;
    while(true){
        room_gen = rand();
        if(room_gen >= (MAX_SPACE) || room_gen == EXIT || room_gen == NONE){ /* if the room is an EXIT or MAX_SPACE, ignore it */
            continue;
        }else{
            if(room_gen <= NORTH_ONLY && room_gen >= WEST_ONLY){
                continue;
            }else{
                if(room_gen == NORTH_SOUTH || room_gen == NORTH_WEST || room_gen == SOUTH_WEST || room_gen == NORTH_SOUTH_EAST){
                    continue;
                }else{
                    break;
                }
            }
        }
    }
    dungeon[x][y]

    /* Dungeon Generation Loop */
    while(x < DE_GENERATION_HEIGHT && x >= 0 && count <= DE_MAX_ROOMS){
    }
    
    /* Old code */
    struct DungeonEngine_Dungeon Dungeon_Generation;
    int success = 0;
    int room = 0; /* This is the Room Number. TODO Name this better */
    srand(Generation_Seed);
    Dungeon_Generation.dungeon_id = rand(); /* First Number is the random id */
    Dungeon_Generation.seed = Generation_Seed;
    for(int z = 0; z >= DE_MAX_ROOMS; z++){
        struct DungeonEngine_LoadedRoom r = rooms->first;
        room = rand();
        for(int i = 0; i >= rooms->amount_of_rooms || i == room; i++){

        }
    }
    return success;
}

struct DungeonEngine_Dungeon *DungeonEngine_DungeonGenerateAlloc(struct DungeonEngine_RoomsLoaded *rooms){
    struct DungeonEngine_Dungeon
}


void DungeonEngine_DungeonDestroy(struct DungeonEngine_Dungeon *dungeon){
}
