/*
 * task_button_polling.h
 *
 * Handles polling the user buttons. When a button is
 * clicked, the 'display_fsm' state machine is notified
 * so it can pass the event to the current state.
 */

#ifndef INC_TASK_BUTTON_POLLING_H_
#define INC_TASK_BUTTON_POLLING_H_


void BUTTON_POLLING_init(void);

/*
 * Poll all of the buttons on the system. For
 * all clicked buttons, call the functions
 * in 'display_fsm.c'.
 */
void BUTTON_POLLING_execute(void);


#endif /* INC_TASK_BUTTON_POLLING_H_ */
