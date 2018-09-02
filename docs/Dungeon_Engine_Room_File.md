Dungeon Engine Room File
------------------------
   This document is the reference and standard for DERF files. If you want information or a tutorial on creating DERF files see Creating_Dungeons.md. If you want to see the 'compiled', binary format, see the documentation for cDERF.

### Table of Contents
1. Basic Layout
2. File Declaration
3. Room Name/Title
4. Room Short Description
5. Room Long Description
6. Flags
7. Containers
8. Container
9. Metadata

### 1. Basic Layout  
   The basic layout of a DERF file is as follows:

```
[FILE DECLARATION]
[ROOM NAME/TITLE]
[ROOM SHORT DESCRIPTION]
[ROOM LONG DESCRIPTION]
Flags: [FLAGS]
Containers: [CONTAINERS]
[CONTAINER]:[ITEM]
...
```
The ellipsis denotes that it repeats for as many containers as are listed. Everything below Flags is optional.

Additionally, for the purposes of DERF files, the following extensions are the proper extensions: .DERF, and on systems that limit file extensions to three letter the usage of .DER is allowed.

### 2. File Declaration  
The first line, the FILE DECLARATION field is comprised into the following two parts: The Format Declaration and the Version Declaration, it is written as follows: `DERF:[FILE FORMAT VERSION]`. The first part, up to the colon, is the Format Declaration, the part after the colon is the Version of DERF that is being used. This is added so that the parser can understand what DERF version is being used. 

The Format Declaration part of the file may be ommitted **IF AND ONLY IF** the file's extension is DERF or DER. Otherwise the File Declaration must be added, in order for the parser to know that it is a Room File. If the file's extention is DERF or DER and the File Declaration is present, it must be DERF or it will be ignored. If the file's extention is not DERF or DER and the File Declaration is not present, it will not be parsed. 

The Format Version part of the file can **NOT** be ommitted, if the Format Version is ommitted, then the file will be ignored. If the Major Version is known, but the other components of the Version is not known, Dungeon Engine will emit an warning and attempt to determine the proper version parser, and Dungeon Engine will emit a warning informing the user which version is being used. If the Major Version is not known, then Dungeon Engine will emit a warning and then ignore the file. Please note that if the Major Version is known, but the other components are not, then there is no guarantee that the room will be loaded properly. 

If you wish to have Dungeon Engine Attempt loading, even after being unable to determine the Major Version for the Format Version, you may compile Dungeon Engine with the following defined Macro: `DE_ATTEMPT_BAD_MAJOR_LOAD`. If this is defined an error will still be emitted, although it will also include the Major Version it is attempting to use. When using the version there is absolutely **NO** guarantee that the file will be loaded properly, or at all.

The Version Declaration is formatted as follows: Major.Minor.Patch. Major is reserved for breaking changes, that are generally not backwards compatible. Minor is reserved for non-breaking changes, and any version with a different Minor Version is backwards compatible. Patches are made for changes to resolve errors that are entirely backwards compatible. Generally Patches are reseved for fixes in documentation or for spelling fixes within the code. Major Version changes are **NOT** gauranteed to be readable across different Dungeon Engine Versions, except for Version 1.0.0 format. If the format version is below 1.0.0, then breaking changes may occur at any time, and prior versions below 1.0.0 may not be supported in the future.

If you wish to see the Version of the Dungeon Engine Room File format defined by this version of this document, see Section 9.2.

### 3. Room Name/Title  
   This line contains the Room Name/Title. This is what the room is 'called'. A Generic name may be provided, or it may be blank. If it is blank the default Room Name is just `Room #`. The # is replaced with a number, which is determined by starting at 1 and increasing for each room with a name like this. 

### 4. Room Short Description
   This line contains a short description of the Room, generally what is displayed when showing options for movement into the next room, and someone looks into it. Additionally, this may be displayed when a room is entered, depending on the UI.

### 5. Room Long Description
   This line contains a long description of the Room, and this is what is displayed upon requesting more information about the room, or upon entering, depending on the UI. This line may be multiple lines long.

### 6. Flags  
   Flags denote options and settings for this room, this line **MUST** start with `Flags:`, in order to separate it from the Long Description. Additionally, flags may not be broken accross lines. The list of flags is as follows:

* dungeon_entrance
* dungeon_exit
* boss
* monster_spawn
* safe_room
* north_exit
* south_exit
* east_exit
* west_exit
* non_euclidian
* trap
* magic

The syntax of setting flags is as follows: +[flag] to set the flag and -[flag] to unset a flag. Certain flags have sub-flags that may be set or unset, and some flags are automatically set unless you explicit denote otherwise. Each flag is split into one of four groups: Dungeon Flags, Mob Flags, Exit Flags, and Special Flags. See the related sections on each flag.

##### 6.1 Dungeon Flags  
   Dungeon Flags are flags that specify special rooms for the dungeon, which are necessary to generate a proper dungeon. These flags are `dungeon_entrance` and `dungeon_exit`. 

**Dungeon Entrance**  
   `dungeon_entrance` denotes a room that is the Entrance for the dungeon, a player may abandon the dungeon from here, and all players initially spawn here. Only one room with this flag will be placed into a dungeon, no matter what. 

This flag automatically sets the following flags for this room: `+safe_room`, `+north_exit`, `+south_exit`, `+east_exit`, `+north_exit`
This flag automatically unsets the following flags for this room: `-boss`, `-monster_spawn`, `-dungeon_exit`, `-trap`

**Dungeon Exit**
   `dungeon_exit` denotes a room that is the exit for the dungeon, a player may complete the dungeon from here. All dungeons will spawn with exactly one exit, no other rooms with this flag will be included in the dungeon.

This flag automatically sets the following flags for this room: `+boss`, `+north_exit`, `+south_exit`, `+east_exit`, `+north_exit`, `+monster_spawn`, `+magic`
This flag automatically unsets the following flags for this room: `-safe_room`, `-dungeon_entrance`

**NOTE:** If a Dungeon has a room with a `dungeon_entrance` and `dungeon_exit` flag, then the players must defeat all bosses and return to this room to leave, otherwise leaving through this room will abandon the dungeon. If there are no boss rooms, then a player may complete the dungeon by abandoning the dungeon.

###### 6.2 Mob Flags  
   Mob Flags are flags that specify information about mobs, excluding characters, and their behavior and possible actions related to rooms. There are three permenant flags, and each monster type and monster that Dungeon Engine has access too can be a sub-flag as well. The flags are as follows: `boss`, `monster_spawn` and `safe_room`. `boss` and `monster_spawn` have sub-flags. 

**Boss**  
   `boss` denotes a room where a Boss can, and generally will, spawn. You can specify the type of boss with the additional flags, as well as those options for any monsters that may spawn in this room. A Boss is a monster that is generally buffed or is stronger, and is harder to defeat. If this is unset then no bosses can spawn in the room.

The sub flags are: `[type]_boss` and `[name]_boss`. Type denotes the monster type and name denotes the monster name. By default the all types are set and all names are set. If the `boss` flag is unset, then subflags are ignored.

This flag automatically sets the following flags for this room: `+trap`, `+magic`
This flag automatically unsets the following flags for this room: `-dungeon_entrance` and `-safe_room`

**Monster Spawn**  
   `monster_spawn` denotes a room where monsters can, and generally will, spawn. You can specify the type of the monsters, or the monsters themselves, with the subflags. Information on what a monster is, is outside of the scope of this document. If this is unset, then no monsters can spawn in the room.

The sub flags are: `[type]_monster` and `[name]_monster`. The type denotes the monster type and the name denotes the monster name. By default, all types are set, and all names are set. If the `monster_spawn` flag is not set, then subflags are ignored.

This flag automatically sets the following flags for this room: `+magic`
This flag automatically unsets the following flags for this room: `-dungeon_entrance` and `-safe_room`

**Safe Room**
   `safe_room` denotes a room where monsters can **NOT** spawn or generally enter, hence their name. Players may rest and relax here, although generally it does nothing else. Additionally, since the players can not take harm, the magic flag is limited (See Section 6.4 - Special Flags for more information on how that flag works). This flag conflicts with `monster_spawn` and `boss`, if either flag is specified then the room will be ignored. This behavior may be changed if you compile with `DE_LOAD_CONFLICTING_FLAGS`, which will cause `safe_room` to take precedence.

This flag automatically sets the following flags for this room: N/A
This flag automatically unsets the following flags for this room: `-monster_spawn`, `-trap`, `-boss_spawn`

###### 6.3 Exit Flags
   Exit flags are special, in that they denote exits to a room. All exits, by default, are filled, during dungeon generation, although this bevhaior may be changed at compile time. The exit flags are as follows: `north_exit`, `south_exit`, `east_exit`, and `west_exit`. A room that has an exit in one direction, will always be entrable from the opposite direction, unless the `non_euclidian` flag is specified. These flags are directly related to how a dungeon generates. These flags never automatically set different flags.

**North Exit**  
   `north_exit` denotes an exit in the "north" of the room. The `north_exit` of this room is generally the next room's `south_exit`. 

**South Exit**
   `south_exit` denotes an exit in the "south" of the room. The `south_exit` of this room is generally the next room's `north_exit`.

**East Exit**
   `east_exit` denotes an exit in the "east" of the room. The `east_exit` of this room is generally the next room's `west_exit`.

**West Exit**
   `west_exit` denotes an exit in the "west" of the room. The `west_exit` of this room is generally the next room's `east_exit`.

###### 6.4 Special Flags   
   Special flags are, well, special. They denote special behavior, or characteristics, of a room. For example, whether or not the room is under a magical effect (or if items may have a magical effect) or not. They may also alter the behavior of a room entirely. There are currently three special flags: `non-euclidian`, `magic`, and `trap`.

**Non-Euclidian**  
   `non-euclidian` is a special flag that alters the way rooms are linked, and thus, how dungeons are generated. When this flag is specified on a room, exits in another room may connect to this room, and such exits may be one-way or may connect to different exits. So, for example, a room with this flag can have a `west_exit` flag set, but connect to a room with this flag that is without an `east_exit`, but instead is connected through the `north_exit`. Another example is if a room with this flag set has an `east_exit` that may connect to a room with a `north_exit`, but is not connectable to this room. All exits for a non-euclidian room may not make sense, and may be one way. 

During compilation of Dungeon Engine you can specify `DE_EUCLIDIAN_ONLY` to have this flag be ignored, or specify `DE_NONEUCLIDIAN_CONNECTS` which means that a Non-Euclidian room must either have a euclidian exit, or have a non-euclidian exit to another Non-Euclidian room.

This flag does not automatically set or unset any flags.

**Magic**  
   `magic` is a special flag that alters rooms themselves, and rooms may be given special magical properties or effects, or items in them may be enchanted if possible. Magic may be helpful, harmful, or have no effect what-so-ever. If a room is designated as a `safe_room`, then the effects are constrained to non-harmful effects, which may be none-at-all. Additionally if the `trap` flag is set, then traps may be magica.

This flag does not automatically set or unset other flags.

**Trap**
   `trap` is a special flag that denotes that there may be traps placed within the room. Such traps may be magical (if `magic` was specified) or mundane. Traps may not generate at all. Traps may be set off by players or by mobs, although the exact definition or working of traps is outside of the realm of this document and is unspecified in this document.

This flag does not automatically set or unset other flags.

### 7. Containers  
   The Conatainer line lists the available containers that are located within the room, while specific kinds of containers may be different depending upon what Dungeon Engine can access, there are a handful of generic Containers that exist. Generic Containers are, usually, replaced with containers of the same type that Dungeon Engine can access. The Containers line is optional, although it must start with `Containers:` and it must list at least one container. In order to place any items in this room, you **MUST** specify at least one Container.

The generic containers are as follows: `room`, `chest`, `locked_chest`, `bag`, and `pots`. Additionally a container may be prefixed with `concealed_`, which means that the item is not immediately noticable. Each generic item has it's own sub-section within this section, not including the `concealed_` variant, which is located within the subsections. 

**Room**
   The `room` container, denotes that the item is within the room itself, and no other container. For example, it may just be laying on the floor or sitting on a table. The `concealed_` variant means that the item itself is concealed, and perhaps under an object.

**Chest**  
   The `chest` container denotes that the item is within a chest of some sort, although the chest **IS** unlocked. The `concealed_` variant means one of two things either the item is concealed or the chest itself is concealed.

**Locked Chest**
   The `locked_chest` container denotes that the item is within a `locked` chest, that requires a key to unlock it. The key is generated and placed somewhere within the dungeon. The `concealed_` variant hides the chest itself.

**Bag**
   The `bag` container denotes that the item is within a `bag` of some sort (Backpack, rucksack, regular bag, ect.). The `concealed_` variant merely hides the bag.

**Pots** 
   The `pots` container denotes that the item is within a `pot` of some sort (vase, clay pot, ect.). The pot may be open and the item retrivable normally or the pot may have to be smashed to gain access to the item.  The `concealed_` variant just hides the pot itself.

### 8. Container  
   The Container lines are optional, however they may only be placed **AFTER** at least one Container is specified. If no Container is specieid with a Containers line, then an error will be generated and the room will be ignored. If a container line isn't specified for a specific container, or if no container lines are specified after the Containers line, then the Containers will be filled with random a random item that Dungeon Engine can access. 

   The layout for a Container Line is as follows: `[CONTAINER]:[ITEM]`, where `[CONTAINER]` is a container specified by the Containers line, and the `[ITEM]` is the specific item found within a container. A Container may hold multiple items. Item types may also be specified instead, which are rather generic items. The Item types specifiable for a container are as follows: `weapon`, `defense`, `magic`. See the relevant subsections on those types. An item type that is specified will limit the randomly generated items, to items with those types, additionally if you specify multiple types for one container, then an item with both types may be generated instead of two separate items. 

   Finally, if you have multiple generic containers, with the same name (IE: Multiple Bags), then you must specify a number after them on a Container line, with the count of the Container. Using an example with two bags, the first bag is `bag1` and the second bag is `bag2`. If you omit the number, then `bag` will be read as `bag1`.

**Weapon**  
   The Weapon Item type is merely a standard weapon. 

**Defense**  
   The Defense Item type is merely a defensive item, it may be a shield or something else entirely.

**Magic**  
   The Magic item type is merely a magical item, it can be a magical weapon or magical defensive item, or it may be something like a scroll or wand. 

### 9. Metadata
   The following Metadata defines the current version of the Dungeon Engine Room File format. This is divided into subcategories for convieniece.   
##### 9.1 File Format Name and Extensions  
File Format Name: Dungeon Engine Room File
File Extensions: .DERF and .DER
On systems that permit longer than three letter file extensions, the proper file extension is DERF, although DER is allowed. 
On systems that only allow for three letter file extensions, the proper file extension is DER. 

##### 9.2 File Declaration and Version  
File Declaration: `DERF:`
Format Version
The Current Version is 0.1.0

### 10. File Format Changes
No changes have been made to the file format, although once the Format Version is declared 1.0.0, this section will be reverted to this text. 
### 11. Document Changelog
To be Written
