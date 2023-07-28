/**
 * @file game.c
 * @author Matthew Harper and Michael Zhu
 * @brief Main file of the game handles all timing, game states and user input
 * @version 1.0
 * @date 2022-10-18
 */

#include <stdbool.h>
#include <stdint.h>
#include "system.h"
#include "task.h"
#include "player.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "pio.h"
#include "led.h"
#include "house.h"

#define DISPLAY_TASK_RATE 250
#define GAME_TASK_RATE 100
#define IR_TASK_RATE 100
#define NAVSWITCH_TASK_RATE 100
#define SCALE_FACTOR 5
#define BUFFER 50
#define SOUTH 1
#define NORTH -1
#define EAST 1
#define WEST -1
#define WON 1
#define VERTICAL_OFFSET 2


#define PACER_RATE 500
#define MESSAGE_RATE 50

/**
 * Holds the previous x of the enemy player
 */
int8_t prevX;
/**
 * Holds the previous y of the enemy player
 */
int8_t prevY;

/**
 * Holds the different states possible in the game
 */
typedef enum {STATE_INIT, STATE_PLAYING,
              STATE_OVER} State_t;

/**
 * Current state of the game based on user input
 */
State_t game_state = STATE_INIT;

/**
 * If true the game will resest next cycle
 */
bool reset = false;

/**
 * The task that displays the leds on the screen
 * Can be in three states, init (home page), playing and the game over screen
 * 
 *  @param data not used by this function hence the __unused__ however nessesary to pass in in order to get task.h functioning
 */
void display_task (__unused__ void *data)
{
    static bool init = false;
    static bool game_init = false;

    if (!init) {
        tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
        tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_speed_set(MESSAGE_RATE);
        tinygl_text("HOUSE");
        init = true;
    }

    switch (game_state) {
        case STATE_INIT:
            tinygl_update();
            break;
        case STATE_PLAYING:
            if (!game_init) {
                player_init();
                house_init();
                game_init = true;
            }
            tinygl_update();
            break;
        case STATE_OVER:
        reset = true;
        tinygl_clear();
        playerReset();
        tinygl_text("PRESS TO RESTART");
        tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_speed_set(MESSAGE_RATE);
        game_init = false;
        break;
    }
}

/**
 * Sends an encoded message to the other player containing the x and y coordinates of the player to show on their screen
 */
void send_encoded_message(void)
{
    int8_t reversedPlayersY = TINYGL_HEIGHT - getPlayerY() - VERTICAL_OFFSET;
    int8_t playersX = TINYGL_WIDTH - getPlayerX();
    int8_t encodedMessage = ((reversedPlayersY * SCALE_FACTOR) + playersX + BUFFER);
    ir_uart_putc(encodedMessage);
}

/**
 * The task that checks for the players input in the navstick
 * The input it is looking for vaires based on the current state of the game
 * 
 * @param data required parameter to get task.h scheduling all the tasks
 */
void navswitch_task (__unused__ void *data)
{
    navswitch_update ();

    switch (game_state) {
        case STATE_INIT:
            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                game_state = STATE_PLAYING;
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
                tinygl_clear();
            }
            break;
        case STATE_PLAYING:
            if (!is_house_exist()) {
                ir_uart_putc(WON);  // sends signal to other player to stop the game
                playerReset();
                game_state = STATE_OVER;
            }

            if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                if (getPlayerX() <= 0 && getPlayerX() > -6) {
                    movePlayer(0, SOUTH);
                    send_encoded_message();
                } else {
                    movePlayer(0, SOUTH);
                }
            }
                
            if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                if (getPlayerX() <= 0 && getPlayerX() > -6) {
                    movePlayer(0, NORTH);
                    send_encoded_message();
                } else {
                    movePlayer(0, NORTH);
                }
            }
            
            if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                if (getPlayerX() <= 0 && getPlayerX() > -6) {
                    movePlayer(EAST, 0);
                    send_encoded_message();
                } else {
                    movePlayer(EAST, 0);
                }
                
            }
                
            if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                if (getPlayerX() <= 1 && getPlayerX() > -6) {
                    
                    movePlayer(WEST, 0);
                    send_encoded_message();
                } else {
                    movePlayer(WEST, 0);
                }
            }
            break;
        case STATE_OVER:
            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                reset = true;
            }
            break;
    }
}

/**
 * The task that checks for incoming transmission from the enemy player
 * Will vary the information sent based on the game state but in most cases will just be recieiving an encoded message contating the enemy players lat and long coords to display on the users screen
 * This information is decoded to extract the x and y coordinate from just one number
 * 
 * @param data required parameter to get task.h scheduling all the tasks
 */
void recv_task (__unused__ void *data)
{
    switch(game_state) {
        case STATE_INIT:
            break;
        case STATE_PLAYING:
             if (ir_uart_read_ready_p()) {
                int8_t received = ir_uart_getc();
                if (received == WON) { //Then the other player has lost and the player recieveing has won
                    game_state = STATE_OVER;
                    tinygl_clear();
                    playerReset();
                }

                int8_t enemyX = received % 5;
                int8_t enemyY = (received - 50 - enemyX) / 5;

                tinygl_draw_point(tinygl_point (prevX, prevY), 0);
                prevX = enemyX;
                prevY = enemyY;
                tinygl_draw_point(tinygl_point (prevX, prevY), 1);
       

             }
            break;
        case STATE_OVER:
            game_state = STATE_INIT;
            break;
    }
}

/**
 * Main method of game gets called once when the program initalises.
 * Sets up the tasks schedule and initalises many variables
 */
int main (void)
{
    task_t tasks[] =
    {
            {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = 0},
            {.func = recv_task, .period = TASK_RATE / IR_TASK_RATE, .data = 0},
            {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE, .data = 0},
    };
    system_init ();
    led_init ();
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    navswitch_init();
    ir_uart_init ();
    led_set(LED1, 0);

    task_schedule (tasks, ARRAY_SIZE (tasks));
}
