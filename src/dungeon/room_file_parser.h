#ifndef SDE_DERFPARSE
#define SDE_DERFPARSE
#include <stdio.h>

union DungeonEngine_Flags;

union DungeonEngineFlags;

struct DungeonEngine_Room {
    unsigned long int room_id; /* Is mainly used in room loading, it is unpredictable */
    union DungeonEngine_Flags flags;
    char room_name[500];
    char short_description[1000];
    char long_description[9999];
}

struct DungeonEngine_LoadedRoom {
    struct DungeonEngine_Room *;
    struct DungeonEngine_Room *next;
    struct DungeonEngine_Room *prev;
};

struct DungeonEngine_RoomsLoaded {
    int amount_of_rooms;
    struct DungeonEngine_LoadedRoom *first;
    struct DungeonEngine_LoadedRoom *last;
};

/* Memory Functions */
int DungeonEngine_RoomFileParser_Init(void);
int DungeonEngine_RoomFileParser_Shutdown(void);

/* Load all files in a directory */
struct DungeonEngine_RoomsLoaded DungeonEngine_RoomFileParser_ReadDirectory(char *path, long int path_len);

/* Invididual Room File parsers */
struct DungeonEngine_Room DungeonEngine_RoomFileParser_ParseFileAtPath(char *path, long int path_len);
struct DungeonEngine_Room DungeonEngine_RoomFileParser_ParseFileAtFD(int file_descriptor);
struct DungeonEngine_Room DungeonEngine_RoomFileParser_ParseFile(FILE *Room_File);

/* Compiled Room Files */
int DungeonEngine_RoomFileParser_WriteCompiled(struct DungeonEngine_Room Loaded_Room);
struct DungeonEngine_Room DungeonEngine_RoomFileParser_LoadCompiled(char *path, long int path_len);

/*TODO: Write Code that better conforms to the C99/C11 Standard */
/*WARNING: This code shoudl not be here, but only is because I'm lazy */
union DungeonEngine_Flags {
    struct {
        unsigned int euclidian: 1;  /* 0 = Euclidian      ;   1 = Non-Euclidian */
        unsigned int magic: 1;      /* 0 = No Magic       ;   1 = Magic Allowed */
        unsigned int traps: 1;      /* 0 = No Traps       ;   1 = Traps Allowed */
        unsigned int north_exit: 1; /* 0 = No North Exit  ;   1 = North Exit    */
        unsigned int south_exit: 1; /* 0 = No South Exit  ;   1 = South Exit    */
        unsigned int east_exit: 1;  /* 0 = No East Exit   ;   1 = East Exit     */
        unsigned int west_exit: 1;  /* 0 = No West Exit   ;   1 = West Exit     */
        unsigned int entrance_or_exit: 2; /* 0 = Regular ; 1 = Dungeon Entrance ; 2 = Dungeon Exit     */
        unsigned int boss: 1;            /* 0 = No Boss Spawn      ;    1 = Boss Spawn Allowed         */
        unsigned int monster_spawn: 1;   /* 0 = No Monster Spawn   ;    1 = No Monster Spawn Allowed   */
        unsigned int mspawn_restrict: 1; /* 0 = No Restrictions    ;    1 = Monster Spawn Restrictions */
        unsigned int bspawn_restrict: 1; /* 0 = No Restrictions    ;    1 = Boss Spawn Restrictions    */
        unsigned int padding: 3; /* Make Bitfield 16 bytes */
    };
    uint16_t raw_bits;
};
#endif
