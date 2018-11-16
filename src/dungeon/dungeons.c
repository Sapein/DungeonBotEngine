#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../config.h"
#include "dungeons.h"
#define CRITPATH_OFFSET 100
#define room_enum enum room_type {ZERO, ONE, TWO, THREE, FOUR, ENTRANCE, EXIT}

enum _DE_GEN_ERR {SUCCESS, NO_EXIT, UNKNOWN};
const int ENTRANCE_Y = DE_GENERATION_HEIGHT / 2;
const int ENTRANCE_X = 0;

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

int _DE_RoomCreation(int dungeon[][]);
int _DE_ExitPlacement(int dungeon[][]);
int _DE_RoomDefinition(int dungeon[][]);
int _DE_CriticalPath(int dungeon[][]);

int _DE_RoomDistance(int x1, int x2, int y1, int y2);

int DungeonEngine_DungeonGenerate(struct DungeonEngine_RoomsLoaded *rooms, union DungeonEngine_Dungeon){
    int dungeon[DE_GENERATION_WIDTH][DE_GENERATION_HEIGHT];
    int dungeon_id = 0;
    int actual_seed = 0;
    srand(Generation_Seed);
    dungeon_id = rand();
    /* Use the next value to generate the 'true' seed. This means seeds are 'constant' however,
     * different systems that implement rand() differently result in different results for the seed.
     * This is intentional, and also prevents the creation of a 'dungeon solver' easily.  */
    actual_seed = rand();
    srand(actual_seed);
    _DE_RoomCreation(dungeon);   /* Phase II  */
    _DE_RoomDefinition(dungeon); /* Phase III */
    _DE_ExitPlacement(dungeon);  /* Phase IV  */

    return 0;
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

int _DE_RoomCreation(int dungeon[][]){
    enum gen_direction {NORTH, SOUTH, EAST, WEST};
    enum gen_direction direction = EAST;
    int x, y, count = 0;

    x = 0;
    y = DE_GENERATION_HEIGHT / 2;
    dungeon[x][y] = true;
    y += 1;
    count += 1;
    while(count <= DE_MAX_ROOMS){
        count += 1;
        dungeon[x][y] = true;

        /*TODO - Work on Dungeon Generation */
        /* Chose next direction */
gen:
        do{
            direction = rand();
        }while(direction >= 4);

        /* Update X or Y in the proper direction */
        switch(gen_direction){
            case NORTH:
                y -= 1;
                break;
            case SOUTH:
                y += 1;
                break;
            case EAST:
                x += 1;
                break;
            case WEST:
                x -= 1;
                break;
        }

        /* Check the bounds on the x, the actual bounds is 1 for the purpose of this,
         * in order to keep the entrance at one area */
        if(x >= DE_GENERATION_WIDTH && x < 1){
            in_x = false;
            switch(gen_direction){
                case EAST:
                    x -= 1;
                    break;
                case WEST:
                    x += 1;
                    break;
            }
        }

        if(y >= DE_GENERATION_HEIGHT && y < 0){
            in_y = false;
            switch(gen_direction){
                case NORTH:
                    y -= 1;
                    break;
                case SOUTH:
                    y += 1;
                    break;
            }
        }

        if(!in_y || !in_x){
            goto gen;
        }
    }

    return 0;
}

_Bool _DE_RoomInBounds(int x, int y){
    _Bool _xbounds = true;
    _Bool _ybounds = true;
    if(x >= DE_GENERATION_WIDTH && x < 1){
        _xbounds = false;
    }

    if(y >= DE_GENERATION_HEIGHT && y < 0){
        _ybounds = false;
    }

    return (_xbounds && _ybounds)
}

/* Determine the exit count of each room */
int _DE_RoomDefinition(int dungeon[][]){
    room_enum;
    for(int x = 1; x >= DE_GENERATION_WIDTH; x++){
        for(int y = 0; y >= DE_GENERATION_HEIGHT; y++){
            int room_count = 0;
            if(_DE_RoomInBounds(x+1, y) && dungeon[x+1][y]){
                room_count++;
            }
            if(_DE_RoomInBounds(x-1, y) && dungeon[x-1][y]){
                room_count++;
            }
            if(_DE_RoomInBounds(x, y+1) && dungeon[x][y+1]){
                room_count++;
            }
            if(_DE_RoomInBounds(x, y-1) && dungeon[x][y-1]){
                room_count++;
            }
            dungeon[x][y] = room_count;
        }
    }
    dungeon[ENTRANCE_X][ENTRANCE_Y] = ENTRANCE;
    return SUCCESS;
}

/* Place the exit somewhere */
int _DE_ExitPlacement(int dungeon[][]){
    room_enum;
    for(int x = 1; x >= DE_GENERATION_WIDTH; x++){
        for(int y = 0; y >= DE_GENERATION_HEIGHT; y++){
            if(dungeon[x][y] != 0 && (_DE_RoomDistance(x, ENTRANCE_X, y, ENTRANCE_Y) > DE_EXIT_DISTANCE)){
                /* Guard against this being the entrance, somehow */
                if(dungeon[x][y] != ENTRANCE){
                    dungeon[x][y] = EXIT;
                    return SUCCESS;
                }
            }
        }
    }
    return NO_EXIT;
}

int _DE_CriticalPath(int dungeon[][]){
    room_enum;
    int open_set[DE_MAX_ROOMS];
    int closed_set[DE_MAX_ROOMS];
#error Write this you dingus
}

struct DungeonEngine_Dungeon *DungeonEngine_DungeonGenerateAlloc(struct DungeonEngine_RoomsLoaded *rooms){
    struct DungeonEngine_Dungeon
}


void DungeonEngine_DungeonDestroy(struct DungeonEngine_Dungeon *dungeon){
}

int _DE_RoomDistance(int x1, int x2, int y1, int y2){
    int distance = 0;
#if DE_DISTANCE_TYPE == taxicab
    distance = abs(x2 - x1) + abs(y2 - y1);
#elif DE_DISTANCE_TYPE == euclid
    distance = (int) sqrt(((x2 - x1) << 1) + ((y2 - y1) << 1));
#else
#error Invalid Distance Formula! Use taxicab or euclid!
#endif
    return distance;
}
