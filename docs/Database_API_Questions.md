Database API Questions
======================
   This file mainly exists as a collection of questions that might arise, or that do arise, from DungeonEngine's API Design in some areas.


### Database API Design

##### Why do you require the password for character changes?  
   Security mainly. You will almost never want to allow a character, or person, who _ISN'T_ the creator modify or change another character. Additionally, in the event of someone wanting to change the a character's data, the person would more than likely be an admin who has Database Access anyway. 

   In the event that you don't want to do this, there will _EVENTUALLY_ be a set of functions that bypasses this requirement, albeit it will generate a warning and it _WILL_ return a special code when it works to ensure that it's only used in functions where security isn't an issue. For now, just use the function to run an SQL Statement. 

##### Why do you have your own Database Design?   
   The custom Database design used here, is mainly for areas where using sqlite3 or something else is unnecessary or you just can't (for example, you don't have a version of sqlite3 for your platform). Albeit it isn't very good, and isn't recommended to be used unless absolutely necessary.
