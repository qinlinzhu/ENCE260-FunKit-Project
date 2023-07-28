/**
 * @file player.h
 * @author Matthew Harper and Michael Zhu
 * @brief Header file for player.c used by game.c to create and move the player
 * @version 1.0
 * @date 2022-10-18
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "tinygl.h"

/** Initalises the player on the screen */
void player_init (void);

/** Moves the player in the given x and y coordinate */
void movePlayer (int8_t, int8_t);

/** Returns the players x coordinate */
int8_t getPlayerX (void);

/** Returns the players y coordinate */
int8_t getPlayerY (void); 

/** Resets the player to their starting position */
void playerReset(void);

#endif //PLAYER_H