/*
 * task_display.h
 *
 * Every time this task is executed, 'DISP_FSM_get_display_chars' is
 * called. The text returned from this function is then written to
 * the display by this module.
 */

#ifndef INC_TASK_DISPLAY_H_
#define INC_TASK_DISPLAY_H_


#include <stdbool.h>


/*
 * Call the 'display_fsm' function to get a string
 * for the current screen to display.
 */
void DISPLAY_execute(void);

void DISPLAY_init(void);


#endif /* INC_TASK_DISPLAY_H_ */
