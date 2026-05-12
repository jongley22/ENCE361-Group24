/*
 * fsm_states.h
 *
 * Defines the states for this particular project.
 *
 * For information on how the state machine works, see 'display_fsm.c'
 */

#ifndef FSM_STATES_H
#define FSM_STATES_H


/*
 * Go to the starting state. This is called by app.c on program
 * start to set 'display_fsm' to a working state.
 */
void FSM_STATES_to_initial_state(void);


#endif /* FSM_STATES_H */
