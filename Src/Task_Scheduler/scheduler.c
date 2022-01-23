/**
  ******************************************************************************
  * @file    scheduler.c
  * @author  Ri-Sheng Chen
  * @brief   This file implements a simple tasks scheduler example.
  ******************************************************************************
  * @attention
  * 	 1. Implement a schedular which schedules multiple user tasks in a round-robin fashion by
  *         carring out the context switch operation
  *      2. Round robin scheduling method is, time slices are assigned to each task in equal portions 
  *         and in circular order
  *      3. First will use systick handler to carry out the context switch operation between multiple tasks 
  *      4. Later will we change the code using PendSV handler
  */

#include <stdio.h>
#include "myusart.h"

// stack memory calculate
#define TASK_STACK_SIZE         1024U // 1KB
#define HANDLER_Stack_SIZE      1024U // 1KB

#define SRAM_START_ADDR          0x20000000U
#define SRAM_SIZE               ((64) * (1024)) // 64KB
#define SRAM_END_ADDR           ((SRAM_START_ADDR) + (SRAM_SIZE))
// define User task
#define TASK1_START_ADDR        SRAM_END_ADDR
#define TASK1_END_ADDR          ((TASK1_START_ADDR) - (TASK_STACK_SIZE))
#define TASK2_START_ADDR        TASK1_END_ADDR
#define TASK2_END_ADDR          ((TASK2_START_ADDR) - (TASK_STACK_SIZE))
#define TASK3_START_ADDR        TASK2_END_ADDR
#define TASK3_END_ADDR          ((TASK3_START_ADDR) - (TASK_STACK_SIZE))
#define TASK4_START_ADDR        TASK3_END_ADDR
#define TASK4_END_ADDR          ((TASK4_START_ADDR) - (TASK_STACK_SIZE))
// define Handler
#define Handler_START_ADDR      TASK4_END_ADDR
#define Handler_END_ADDR        ((Handler_START_ADDR) - (TASK_STACK_SIZE))

// define register
#define SYST_CSR                *(uint32_t*)0xE000E010 // SysTick Control and Status Register
#define SYST_RVR                *(uint32_t*)0xE000E014 // SysTick Reload Value Register
#define SYST_CVR                *(uint32_t*)0xE000E018 // SysTick Current Value Register

// 一些設定的macros
#define TICK_HZ                 1000U // 計時中斷頻率1000Hz

void task1_handler(void);  // Task 1
void task2_handler(void);  // Task 2
void task3_handler(void);  // Task 3
void task4_handler(void);  // Task 4
void SysTick_Timer_Init(); // SysTick Timer Initial

int main(void) {
    MYUSART_Init();
    printf("Start\n");
    SysTick_Timer_Init();

    while(1);
    return 0;
}

void SysTick_Handler(void) {
    
}

void SysTick_Timer_Init() {
    uint32_t reload_val = DEFAULT_F_CLK / TICK_HZ - 1;
    SYST_RVR = reload_val; // set reload_val to SYST_RVR
    SYST_CSR |= (1 << 0) | (1 << 1) | (1 << 2); // enable ENABLE TICLINT CLKSOURCE

}

void task1_handler(void) {
    while(1) {
        printf("In task1\n");

    }

}

void task2_handler(void) {
    while(1) {
        printf("In task2\n");
        
    }
}

void task3_handler(void) {
    while(1) {
        printf("In task3\n");
        
    }
}

void task4_handler(void) {
    while(1) {
        printf("In task4\n");
        
    }
}