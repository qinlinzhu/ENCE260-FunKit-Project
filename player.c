/**
 * @file player.c
 * @author Matthew Harper and Michael Zhu
 * @brief Handles the creation, reset, and movement of the player
 * @version 1.0
 * @date 2022-10-18
 */

#include <stdbool.h>
#include "system.h"
#include "player.h"
#include "tinygl.h"

#define PACER_RATE 100
#define INITAL_ROW 2
#define INITAL_COL 3
#define OFF 0
#define ON 1
#define MAX_X 5
#define MIN_X -5
#define MAX_Y 7
#define MIN_Y -1

/**
 * Stores the players x postition.
 */
int8_t playerX;

/**
 * Stores the players y postition.
 */
int8_t playerY;

/**
 * Called once when the game loads to initalise the player on the screen.
 */
void player_init(void) {
    tinygl_init(PACER_RATE);
    playerX = INITAL_ROW;
    playerY = INITAL_COL;
    tinygl_draw_point(tinygl_point (INITAL_COL, INITAL_ROW), ON);
}

/**
 * Updates the players position if its within the bounds of the screens.
 * 
 * @param newX how far the player will jump in the x direction can be negative for moving back
 * @param newY how far the player will jump in the y direction can be negative for moving back
 */
void movePlayer(int8_t newX, int8_t newY) {
    if (playerX + newX == MAX_X) {
        return;
    } 
    else if (playerX + newX == MIN_X) {
        return;
    } 
    else if (playerY + newY == MAX_Y) {
        return;
    } 
    else if (playerY + newY == MIN_Y) {
        return;
    } else {
        tinygl_draw_point (tinygl_point (playerX, playerY), OFF);
        playerX += newX;
        playerY += newY;
        tinygl_draw_point (tinygl_point (playerX, playerY), ON);
        tinygl_update ();
    }
}

void playerReset(void)
{
    tinygl_draw_point (tinygl_point (playerX, playerY), OFF);
    playerX = INITAL_ROW;
    playerY = INITAL_COL;
    tinygl_draw_point (tinygl_point (playerX, playerY), ON);

}

/**
 * Returns the players x coordinate on the screen.
 * 
 * @return int8_t holding the players x coordinate
 */
int8_t getPlayerX (void) {
    return playerX;
}

/**
 * Returns the players y coordinate on the screen.
 * 
 * @return int8_t holding the players y coordinate
 */
int8_t getPlayerY (void) {
    return playerY;
}

