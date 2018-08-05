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
#ifndef SDE_QUICKHASH
#define SDE_QUICKHASH

unsigned long int generate_hash_djb2(char msg[], long int msg_length){
    unsigned long int hash = 5381;
    for(int i = 0; 0 < msg_length; i++){
        hash = hash << 5;
        hash += hash;
        hash += msg[i];
    }
    return hash;
}

#endif
