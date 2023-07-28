/**
 * @file house.h
 * @author Matthew Harper and Michael Zhu
 * @brief Header file for house.c used by game.c to create and monitor the house
 * @version 1.0
 * @date 2022-10-18
 */

#ifndef HOUSE_H
#define HOUSE_H

#include "system.h"
#include "tinygl.h"

/** Initalises the players house on the screen */
void house_init (void);

/** Checks if all the tiles of the house still exist */
bool is_house_exist(void);
#endif //HOUSE_H
