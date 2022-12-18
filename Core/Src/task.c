/*
 * task.c
 *
 *  Created on: Dec 16, 2022
 *      Author: loing
 */

#include "task.h"

void fsm_auto() {
	if (currentMode == AUTO) {
		switch (state) {
		case STATE0:
			sprintf(msg, "LED1: RED - %d\r\n", (currentTick + timeGreen) / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			sprintf(msg, "LED2: GREEN - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 1);
			break;
		case STATE1:
			sprintf(msg, "LED1: RED - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			sprintf(msg, "LED2: YELLOW - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 1);
			break;
		case STATE2:
			sprintf(msg, "LED1: GREEN - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			sprintf(msg, "LED2: RED - %d\r\n", (currentTick + timeYellow) / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 0);
			break;
		case STATE3:
			sprintf(msg, "LED1: YELLOW - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			sprintf(msg, "LED2: RED - %d\r\n", currentTick / 10);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, sizeof(msg));
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 0);
			break;
		}
	}
}

void fsm_mode() {
//	if (timer0Flag == 1) {
	if (currentMode == MODIFIED) {
		switch (state) {
		case MOD_RED:
			sprintf(msg, "RED: %d\r\n", tempDuration);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 0);
			break;
		case MOD_GREEN:
			sprintf(msg, "GREEN: %d\r\n", tempDuration);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 1);
			break;
		case MOD_YELLOW:
			sprintf(msg, "YELLOW: %d\r\n", tempDuration);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), 50);
			HAL_GPIO_WritePin(GPIOB, Led_11_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_12_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_21_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, Led_22_Pin, 1);
			break;
		}
	}
//	}
}

void fsm() {
	fsm_auto();
	fsm_mode();
}

void init_task() {
	void (*pTask0)(void);
	void (*pTask1)(void);
	void (*pTask2)(void);
	void (*pTask3)(void);
	void (*pTask4)(void);

	pTask0 = getKeyInput;
	pTask1 = update_state;
	pTask2 = fsm;
	pTask3 = timer_run;
	pTask4 = pedestrian_fsm;

	SCH_add_task(pTask3, 1, 10);
	SCH_add_task(pTask0, 1, 10);
	SCH_add_task(pTask1, 1, 10);
	SCH_add_task(pTask2, 2, 100);
	SCH_add_task(pTask4, 1, 50);
}

void update_state() {
//	if (timer1Flag == 1) {
		currentTick--;
		if (currentMode == AUTO) {
			switch (state) {
			case STATE0:
				if (currentTick == 0){
					currentTick = pivot[1];
					state = STATE1;
				}
				break;
			case STATE1:
				if (currentTick == 0){
					currentTick = pivot[2];
					state = STATE2;
				}
				break;
			case STATE2:
				if (currentTick == 0) {
					currentTick = pivot[3];
					state = STATE3;
				}
				break;
			case STATE3:
				if (currentTick == 0) {
					currentTick = pivot[0];
					state = STATE0;
				}
				break;
			}
		}
		else if (currentMode == MODIFIED) {
			if (currentTick == 0) {
				currentTick = pivot[0];
				currentMode = AUTO;
				state = STATE0;
			}
		}
//	}
}
