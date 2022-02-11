/**
  ******************************************************************************
  * @file    Exercise1.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a simple clock config example 
  ******************************************************************************
  * @attention
  *     1. Using HSE Configure the SYSCLK as 8MHz
  *     2. AHB clock as 4MHz (HCLK)
  *     3. APB1 clock as 2MHz (PCLK1)
  *     4. APB2 clock as 2MHz (PCLK2)
  */

#include <stdio.h>
#include <stdint.h>
#define RCC_BASE_ADDR        (volatile uint32_t*)(0x40021000U)              // RCC peripheral base address
#define RCC_CR              *(volatile uint32_t*)((uint32_t)RCC_BASE_ADDR + 0x00U)  // Clock control register
#define RCC_CFGR            *(volatile uint32_t*)((uint32_t)RCC_BASE_ADDR + 0x04U)  // Clock configuration register

int main(void)
{   
    RCC_CR |= (1 << 18);   // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);   // HSE clock enable
    while(!((RCC_CR & 0x20000) >> 17)); // 等待HSE準備好
    
    RCC_CFGR |= (6 << 24); // Microcontroller clock output (HSE clock selected)
    RCC_CFGR |= (1 << 0);  // System clock switch (HSE selected as system clock)
    
    while(1);
    return 0;
}
