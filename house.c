/**
 * @file house.c
 * @author Matthew Harper and Michael Zhu
 * @brief Handles drawing the players house on screen and checking it is all still onscreen
 * @version 1.0
 * @date 2022-10-18
 */

#include <stdbool.h>
#include "system.h"
#include "house.h"
#include "tinygl.h"

#define PACER_RATE 500
#define TOP_LEFT_X 4
#define TOP_LEFT_Y 2
#define BOTTOM_RIGHT_X 2
#define BOTTOM_RIGHT_Y 4
#define LED_ON 1
#define NUM_HOUSE_TILES 9
#define NUM_HOUSE_ROWS 3

/**
 * Called once when the game loads to initalise the house on the screen.
 * Draws a simple box to act as the players house
 */
void house_init(void) {
    tinygl_init(PACER_RATE);
    tinygl_draw_box(tinygl_point (TOP_LEFT_X, TOP_LEFT_Y), tinygl_point (BOTTOM_RIGHT_X, BOTTOM_RIGHT_Y), LED_ON);
}

/**
 * Returns true if the house is all together returns false if the house is completely destryoed
 * Gets called every time the timer is in the game state
 * @return bool false if house gone else true
 */
bool is_house_exist(void) {
    int8_t num_tiles_checked = 1;
    int8_t num_tiles_off = 0;
    int8_t current_x_to_check = TOP_LEFT_X;
    int8_t current_y_to_check = TOP_LEFT_Y;
    while ((num_tiles_checked <= NUM_HOUSE_TILES)) {
            if (tinygl_pixel_get (tinygl_point (current_x_to_check, current_y_to_check)) == 0) {
                num_tiles_off++;
            }
            if (num_tiles_checked % NUM_HOUSE_ROWS != 0) {
                current_x_to_check--;
            } else {
                current_x_to_check = TOP_LEFT_X;
                current_y_to_check++;
            }
        num_tiles_checked++;
    }

    if (num_tiles_off == NUM_HOUSE_TILES - 1) {
        return 0;
    }
    return 1;
}