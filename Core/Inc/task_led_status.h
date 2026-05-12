/*
 * task_led_status.h
 *
 * A task for representing the goal progress made with the 4 LEDs
 * RGB_UP, RGB_RIGHT, RBG_DOWN, RGB_LEFT
 * moving clockwise to activate the LEDS.
 */

#ifndef INC_TASK_LED_STATUS_H_
#define INC_TASK_LED_STATUS_H_


void LED_STATUS_init(void);
void LED_STATUS_execute(void);


#endif /* INC_TASK_LED_STATUS_H_ */
