Dungeon Engine
===============
   Dungeon Engine is the back-end engine for the Dungeon Bot. This section handles all of the background dungeon handling, character management, and the like.

### Requirements
1. A standard conforming C99/C11 compiler. 
2. SQLite3 C Headers/Libraries (Optional in Future)

### Development
[To Be Written]

## License and Copyright
This software project is licensed under the GNU Affero General Public License 3.0, if you wish to obtain exceptions or an alternative license you may ask to do so. The Software License is found in the LICENSE file.

All copyright belongs to their respective owners, see the AUTHORS file for all owners of Copyright.

## QnA  

#### Why is this written in pure C?  
   Mainly for speed, also what the back-end was written in doesn't necessarily matter that much?

#### Why is this licensed as under the AGPL?  
   Because of the fact that this is mostly designed as a back-end engine. As such a person could, concievably, modify the engine and _NOT_ release the modifications if it were just released under GPL if they used a bot front-end. I am open to an exception allowing for front ends to _NOT_ be AGPL, provided that the Dungeon Engine code itself is released and accessible from the bot.

#### Why does Player/Character modification require the password?  
   Because of the fact that player/character modifications would _always_ want to be verified first, to potentially close Security Holes in the future, see the file "Database\_API\_Questions.md" in docs for more information.
