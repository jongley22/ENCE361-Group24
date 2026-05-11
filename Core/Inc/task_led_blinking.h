/*
 * task_led_blinking.h
 *
 * A task to blink the builtin LED so you can easily
 * tell when the task scheduler is working.
 */

#ifndef INC_TASK_LED_BLINKING_H_
#define INC_TASK_LED_BLINKING_H_


void LED_BLINKING_init(void);
void LED_BLINKING_execute(void);


#endif /* INC_TASK_LED_BLINKING_H_ */
