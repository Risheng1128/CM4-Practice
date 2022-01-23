## Configurable fault exception
- Exercise 1: Exception_cfg.c
  - 目標: Write a program to enable all configurable fault exceptions, implement the fault exception handlers and cause the fault by following method.
    1. Execute an undefine instruction
    2. Divide by zero
    3. Try executing instruction from peripheral region
    4. Executing SVC inside the SVC handler
    5. Executing SVC instruction inside the interrupt handler whose priority whose priority is same or lesser than SVC handler
  - System Handler Control and State Register (SHCSR) : 參考Cortex-M4 Devices Generic User Guide Table 4-24
    ![](https://i.imgur.com/fyKGeGd.png)
  
  - Example 1: 執行未定義指令
    1. 在地址0x20000501放進未定義的指令0xFFFFFF
       ![](https://i.imgur.com/nkNJXBe.png)
    2. 利用函數指標指到0x20000501並且執行。發現會進Usage Fault
       ![](https://i.imgur.com/sPQB25d.png)
    3. Result
       
       ![](https://i.imgur.com/MUilZma.png)
  
    - 偵測fault產生的原因
      1. 可以從`Fault Status and Fault Address Register`得知產生原因
         ![](https://i.imgur.com/JKnoouv.png)
      2. 查看UsageFault Status Register的值可以知道發生什麼問題
         ![](https://i.imgur.com/Mnj8ir6.png)
         ![](https://i.imgur.com/9R8ssEA.png)
         ![](https://i.imgur.com/YE3LvjW.png)
      3. 結果: Undefined Instruction
         對照後可以看出為UNDEFINSTR &rarr; Undefined instruction UsageFault
         ![](https://i.imgur.com/GGwCU5L.png)
      4. 更改存放指令的地址: 0x20000500 (T bit為0);
         對照後可以看出為INVSTATE &rarr; Invalid state UsageFault
         ![](https://i.imgur.com/KN394ck.png)
      
    - Analyzing stack frame
      
      ![](https://i.imgur.com/LSCFpWI.png)
      1. 結果
         程式結果
         
         ![](https://i.imgur.com/UxblVyQ.png)
         查看memory view
         
         ![](https://i.imgur.com/YLIfElE.png)

    - 查看LR及PC
      - 從上面的結果可以看到LR為0x8000547，查看assmembly可以發現指令為以下表示:
        ```asm=
         8000540:	603b      	str	r3, [r7, #0]
         8000542:	683b      	ldr	r3, [r7, #0]
         8000544:	4798      	blx	r3
         8000546:	bf00      	nop
         8000548:	3708      	adds	r7, #8
        ```
        `nop`為回傳之後要執行的指令
      
      - 從上面的結果可以看到PC為0x2000501，查看assmembly可以發現指令為以下表示:
        rarr; 0x2000501為0xFFFFFFFF，屬於未定義指令
    
  - Example2: Divide by 0
    - Enable divide by 0 trap
      1. 使用Configuration and Control Register (CCR)
         ![](https://i.imgur.com/x6rDduI.png)
      2. 可利用CCR的bit[4]來trap divide by 0
         ![](https://i.imgur.com/EWHvCD6.png)
         ![](https://i.imgur.com/gILKPqu.png)

    - Result
      ![](https://i.imgur.com/owDzF87.png)

    - 查看UFSR(UsageFault Status Register)
      可以看出產生Exception的原因為除以0
      ![](https://i.imgur.com/Vgi6fpt.png)

    - 查看LR及PC
      - 從上面的結果可以看到LR為0x8000525，查看assmembly可以發現指令為以下表示:
        ```asm=
         800051e:	6013      	str	r3, [r2, #0]
         8000520:	f000 f804 	bl	800052c <Example2>
         8000524:	e7fe      	b.n	8000524 <main+0x18>
         8000526:	bf00      	nop
         8000528:	e000ed24 	and	lr, r0, r4, lsr #26
        ```
        最後回傳的地址為`b.n    8000524 <main+0x18>`
      
      - 從上面的結果可以看到PC為0x8000542，查看assmembly可以發現指令為以下表示:
        ```asm=
         800053e:	2201      	movs	r2, #1
         8000540:	2300      	movs	r3, #0
         8000542:	fb92 f3f3 	sdiv	r3, r2, r3
         8000546:	607b      	str	r3, [r7, #4]
         8000548:	bf00      	nop
        ```
        `sdiv	r3, r2, r3`為進Exception最後執行的指令
        
        [sdiv instruction](https://developer.arm.com/documentation/dui0473/m/arm-and-thumb-instructions/sdiv?lang=en)
   
    - 把Usage fault的enable拿掉
      ![](https://i.imgur.com/3xWvfBy.png)
      1. 查看HardFault Status Register(HFSR)
         ![](https://i.imgur.com/zAKJvUX.png)
         ![](https://i.imgur.com/RQ8SB5S.png)
         ![](https://i.imgur.com/sZ0Fzlb.png)
      
      2. 可以看到為bit[30]為1

  - Example3: 從 peripheral region 執行指令
    - Peripheral Region
      從下面的圖，可以看到Peripheral Region位於0x40000000 ~ 0x5FFFFFFF   
      ![](https://i.imgur.com/1ZbPknF.png)

    - Result
      
      ![](https://i.imgur.com/Fm8YK1E.png)

    - 查看MMSR
      ![](https://i.imgur.com/15bNQPH.png)
      ![](https://i.imgur.com/oMLtDwA.png)
      查看後可以發現原因為bit[0] &rarr; 對XN region進行instruction fetch
   
    - 查看LR及PC
      - 從上面的結果可以看到LR為0x800053d，查看assmembly可以發現指令為以下表示:
        ```asm=
         8000536:	607b      	str	r3, [r7, #4]
         8000538:	687b      	ldr	r3, [r7, #4]
         800053a:	4798      	blx	r3
         800053c:	bf00      	nop
         800053e:	3708      	adds	r7, #8
        ```
        `adds	r7, #8`為回傳之後第一個執行的指令
        
      - 從上面的結果可以看到PC為0x40000000，表示PC已經跳到0x40000000要執行指令
        
  - Example4: Executing SVC inside the SVC handler
    - 結果: 進入Hard Fault
      
      ![](https://i.imgur.com/PYE1gVR.png) 

    - 查看LR及PC
      - 從上面的結果可以看到LR為0xfffffff9，為EXC Return值，表示返回Thread mode時使用MSP
        
      - 從上面的結果可以看到PC為0x8000552，查看assmembly可以發現指令為以下表示
        ```asm=
         08000550 <SVC_Handler>:
         8000550:	df05      	svc	5
         8000552:	bf00      	nop
        ```
        執行完svc後執行到nop就進exception
    
  - Example5: Executing SVC instruction inside the interrupt handler whose priority whose priority is same or lesser than SVC handler
    - 使用I2C1_EV(I2C1 event interrupt & EXTI Line23 interrupt)作範例
      - IRQ number: 31
      - Priority: 38
       
      ![](https://i.imgur.com/h3qzsXE.png)
    
    - SVC exception
      - Priority: 3
       
      ![](https://i.imgur.com/BIvoM1q.png)
    
    - 結果: 進入Hard Fault
      
      ![](https://i.imgur.com/4FpAyYK.png)

    - 查看LR及PC
      - 從上面的結果可以看到LR為0xfffffff9，為EXC Return值，表示返回Thread mode時使用MSP
        
      - 從上面的結果可以看到PC為0x800058e，查看assmembly可以發現指令為以下表示
        ```asm=
         0800058c <I2C1_EV_EXTI23_IRQHandler>:
         800058c:	df05      	svc	5
         800058e:	bf00      	nop
        ```
        執行完svc後執行到nop就進exception
    
## SVC exception
- Exercise1: SVC_number.c
  - 目標: Write a program to execute an SVC instruction from thread mode, implement the SVC handler to print the SVC number used. Also, increment the SVC number by 4 and return it to the thread mode code and print it.
    1. Write a main() function where you should execute the SVC instruction with an argument. Using SVC #05 in this case.
    2. Implement the SVC handler function
    3. In the SVC handler extract the SVC number and print it using printf
    4. Increment the SVC number by 4 and return it to the thread mode.
  - 流程
    ![](https://i.imgur.com/nVqVLw4.png)

  1. 進入SVC_Get_Number之前，將MSP的值存在r0中
     - 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
     ![](https://i.imgur.com/Ancv5pK.png)
  2. 可以發現return address指到的位址為跳進中斷前(0x8000552)
     ![](https://i.imgur.com/JcBB7Zu.png)
     從assembly可以看到SVC instruction(0x8000550)和pc(0x8000552)差了2byte
     ```asm=
         08000548 <main>:
         8000548:	b580      	push	{r7, lr}
         800054a:	af00      	add	r7, sp, #0
         800054c:	f7ff fe98 	bl	8000280 <MYUSART_Init>
         8000550:	df05      	svc	5
         8000552:	4603      	mov	r3, r0
         8000554:	4619      	mov	r1, r3
         8000556:	4802      	ldr	r0, [pc, #8]	; (8000560 <main+0x18>)
         8000558:	f000 f866 	bl	8000628 <iprintf>
         800055c:	e7fe      	b.n	800055c <main+0x14>
         800055e:	bf00      	nop
         8000560:	08001578 	.word	0x08001578
     ```
  3. 根據AAPCS，回傳資料時順序為r0,r1，因此使用r0回傳資料，並將r0和暫存器data相關
     ![](https://i.imgur.com/8m2QPNz.png)
  
  4. 也可以使用inline asm回傳資料
     ![](https://i.imgur.com/GYsI7yY.png)

  5. Result
     
     ![](https://i.imgur.com/Jp7Aw2v.png)

- Exercise2: SVC_math.c
  - 目標: Write a program to add, subtract, multiply, and divide 2 operands using SVC handler and return the result to the thread mode code and print the result. Thread mode code should pass 2 operands via the stack frame.
    1. Use the SVC number to decide the operation
    
       | Service number | operation      |
       | -------------- | -------------- |
       | 36             | Addition       |
       | 37             | Subtraction    |
       | 38             | Multiplication |
       | 39             | Division       |
       
  1. 進入SVC前，將變數a & b分別存到r1, r2暫存裡
     
     ![](https://i.imgur.com/sfS4XWw.png)

  2. Result
     
     ![](https://i.imgur.com/vybbghM.png)
