/*
 * pedestrian.c
 *
 *  Created on: Dec 17, 2022
 *      Author: loing
 */


#include "pedestrian.h"

int timeLeft = 0;
int currentTrafficLight;
int timeFSM = 0;
uint32_t PWM = 0;

void pedestrian_fsm(){
	if (pedestrianFlag == 1){
//	void (*pTask)(void);
//	pTask = pedestrian_fsm;
		switch (state) {
		case STATE0:
			HAL_GPIO_WritePin(GPIOB, PED0_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, PED1_Pin, 0);
//			timeFSM=0;
			sprintf(msg, "PED0 %d\r\n", currentTick);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
//			SCH_add_task(pTask, 1, 0);
			break;
		case STATE1:
//			HAL_GPIO_WritePin(GPIOB, PED0_Pin, 1);
//			HAL_GPIO_WritePin(GPIOA, PED1_Pin, 1);
			sprintf(msg, "PED1 %d\r\n", currentTick);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
//			SCH_add_task(pTask, 1, 0);
			break;
		case STATE2:
			HAL_GPIO_WritePin(GPIOB, PED0_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, PED1_Pin, 1);
			sprintf(msg, "PED2 %d\r\n", currentTick);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
//			SCH_add_task(pTask, 1, 0);
			break;
		case STATE3:
//			timePWM = currentTick%6;
			HAL_GPIO_WritePin(GPIOB, PED0_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, PED1_Pin, 1);
			sprintf(msg, "PED3 %d\r\n", currentTick);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
//			if (currentTick>1)
//				SCH_add_task(pTask, 1, 0);
//			if (timeLeft<5) {
//				timePWM++;
//				if (timePWM>=timeLeft) {
//					timePWM = 0;
//					__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3, (5 - timeLeft)*20);
//				}
//			}
			PWM += 5;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM);
			if (currentTick<=5) {
				HAL_GPIO_WritePin(GPIOB, PED0_Pin, 1);
				HAL_GPIO_WritePin(GPIOB, PED1_Pin, 1);
				pedestrianFlag = 0;
				PWM = 0;
			}
			break;
		}
	}
}
