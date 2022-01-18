## Interrupt Priority Configuration
- 目的: 使用 NVIC interrupt pending register 產生以下中斷並分別觀察 priorities 相同及不同時，ISRs 的執行情況
        Interrupt 1: TIM2 global interrupt
        Interrupt 2: I2C1 event interrupt

- 情況1: 當兩個interrupt的優先度相同時
  假設: TIM2和I2C1優先度都為8
  1. 完成interrupt設定
     ![](https://i.imgur.com/RQnhbvR.png)
  2. 進入TIM2 interrupt
     ![](https://i.imgur.com/anwhLCQ.png)
  3. 將 I2C Priority Register enable
     ![](https://i.imgur.com/kCmzevM.png)
  4. 因為I2C優先度和TIM2相等，因此不進I2C中斷函式，停在TIM2函式裡
     ![](https://i.imgur.com/9Sn1KXf.png)
  5. 結果
     ![](https://i.imgur.com/DHSQP6r.png)

- 情況2: 當兩個interrupt的優先度不同時
  假設: TIM2優先度為8，I2C1優先度為7
  1. 完成interrupt設定
     ![](https://i.imgur.com/RQnhbvR.png)
  2. 進入TIM2 interrupt
     ![](https://i.imgur.com/anwhLCQ.png)
  3. 將 I2C Priority Register enable
     ![](https://i.imgur.com/kCmzevM.png)
  4. 因為I2C優先度比TIM2高，因此跳進I2C中斷函式
     ![](https://i.imgur.com/zkuqGM5.png)
  5. 結果
     ![](https://i.imgur.com/RXKGtBP.png)

## Analyzing stack contents during exception entry and exit
- 目的: 使用 RTC_WKUP interrupt分析進入中斷後，觀察sp的變化及儲存之暫存器為何
  - ARM cortex-M4 exception stack frame
    ![](https://i.imgur.com/5yCCEB4.png)
    
  1. 進中斷前sp的位址: 0x2000fff8, xPSR: 0x1000000
     ![](https://i.imgur.com/K98Tap6.png)
     ![](https://i.imgur.com/3V39r09.png)
  2. 進中斷後sp的位址: 0x2000ffd0
     ![](https://i.imgur.com/ODEbt4H.png)
  3. 根據上圖，查看儲存資料，並可以得知ARM為little-endian
     ![](https://i.imgur.com/yJXWxjL.png)

     | Stack Pointer (MSP) | Saved Registers | Saved contents |
     | ------------------- | --------------- | -------------- |
     | 0x2000fff4          | xPSR            | 0x1000000      |
     | 0x2000fff0          | PC              | 0x8000480      |
     | 0x2000ffeC          | LR              | 0x80005F7      |
     | 0x2000ffe8          | R12             | 0x0            |
     | 0x2000ffe4          | R3              | 0xE000EF00     |
     | 0x2000ffe0          | R2              | 0x3            |
     | 0x2000ffdc          | R1              | 0x1            |
     | 0x2000ffd8          | R0              | 0xA            |
     | 0x2000ffd4          | Exc Return      | 0xFFFFFFF9     |
     | 0x2000ffd0          | 上一個 sp 的值    | 0x2000FFF8     |
  
  4. EXC Return
     從assembly code可以看到進中斷前會先執行`push {r7, lr}`，將r7和lr的值放進MSP的Stack space
     離開中斷則是會執行`pop {r7, pc}`，從stack取兩個值並分別給r7和pc
     ```asm=
     08000490 <RTC_WKUP_IRQHandler>:
     8000490:	b580      	push	{r7, lr}
     8000492:	af00      	add	r7, sp, #0
     8000494:	4802      	ldr	r0, [pc, #8]	; (80004a0 <RTC_WKUP_IRQHandler+0x10>)
     8000496:	f000 f8cf 	bl	8000638 <puts>
     800049a:	bf00      	nop
     800049c:	bd80      	pop	{r7, pc}
     800049e:	bf00      	nop
     80004a0:	08000f8c 	.word	0x08000f8c
     ```
     ![](https://i.imgur.com/AoEHX8F.png)
  
  5. 改成使用PSP，並觀察其結果
     1. 進中斷之前各個暫存器的數值(已啟用PSP)
        
        ![](https://i.imgur.com/GafhPmi.png)
        ![](https://i.imgur.com/dz8erJF.png)
     
     2. 進中斷後各個暫存器的數值
        
        ![](https://i.imgur.com/pb13SZL.png)
        ![](https://i.imgur.com/YMZJso3.png)
        
        1. 觀察PSP空間之數值
           ![](https://i.imgur.com/OTiVkGV.png)
        2. 觀察MSP空間之數值
           進中斷和離開中斷都和先前一樣，只差在因為中斷裡使用MSP，sp和EXEC會存到MSP的空間裡，其他則儲存在PSP空間裡。
           
           因為Thread Mode是使用PSP，因此EXC Return的數值也不同
           ```asm=
             080004a8 <RTC_WKUP_IRQHandler>:
             80004a8:	b580      	push	{r7, lr}
             80004aa:	af00      	add	r7, sp, #0
             80004ac:	4802      	ldr	r0, [pc, #8]	; (80004b8 <RTC_WKUP_IRQHandler+0x10>)
             80004ae:	f000 f8cf 	bl	8000650 <puts>
             80004b2:	bf00      	nop
             80004b4:	bd80      	pop	{r7, pc}
             80004b6:	bf00      	nop
             80004b8:	08000fa4 	.word	0x08000fa4
           ```
           ![](https://i.imgur.com/8lS5EHV.png)
