/*
 * task_buzzer_goal_complete.h
 *
 * Checks if the current steps is greater than the current
 * goal. If it is, the buzzer starts buzzing and the display
 * FSM is changed to a state telling the user that the
 * goal is completed.
 */

#ifndef INC_TASK_BUZZER_GOAL_COMPLETE_H_
#define INC_TASK_BUZZER_GOAL_COMPLETE_H_


void BUZZER_init(void);
void BUZZER_execute(void);


#endif /* INC_TASK_BUZZER_GOAL_COMPLETE_H_ */
