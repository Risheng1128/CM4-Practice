## Stack
- 觀察SP
  - Stack的起始位置
    從Linker script &rarr; 可以算出一開始stack的起始位置(_estack)
  
    _estack = 0x20000000 + 64 * 1024 = 0x20010000
    ```linker=
    /* Highest address of the user mode stack */
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */

    _Min_Heap_Size = 0x200; /* required amount of heap */
    _Min_Stack_Size = 0x400; /* required amount of stack */

    /* Memories definition */
    MEMORY
    {
      CCMRAM    (xrw)    : ORIGIN = 0x10000000,   LENGTH = 16K
      RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 64K
      FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 512K
    }
    ```
  - 驗證
    可以看到再startup file裡，sp的確為0x20010000(左下角)
    ![](https://i.imgur.com/DgCJEI8.png)
    
    而msp和sp值相同，表示ARM的預設為msp
    ![](https://i.imgur.com/cIDV5Fq.png)

- Exercise: SP的切換練習
  1. Thread Mode: 使用PSP
  2. Handler Mode: 固定為MSP
  3. STACK SPACE: 1KB
  4. MSP 和 PSP各用一半，MSP使用上半部，PSP使用下半部
  ![](https://i.imgur.com/SO8lSMv.png)

  - Register (from Cortex™-M4 Devices Generic User Guide)
    利用 Control Register, 可以將SP設定為PSP(bit 1)
    ![](https://i.imgur.com/iaGXo3J.png)

  - [MRS and MSR](https://www.itread01.com/content/1542308852.html)
    可利用MRS和MSR分別讀取和寫入PSP

  - Result
    1. 將初始值寫入PSP
    ![](https://i.imgur.com/Fb0jazt.png)
    2. 寫入CONTROL Register
    ![](https://i.imgur.com/NDGnfkI.png)
    3. 進到不同的副函式改變的為PSP
    
    原來
    ![](https://i.imgur.com/WygZi0l.png)
    
    後來
    ![](https://i.imgur.com/mljo3SB.png)
    4. 進Handler mode後使用MSP
    ![](https://i.imgur.com/a3x3Sdk.png)
    5. 回到Thread mode後變回使用PSP
    ![](https://i.imgur.com/v5djfEv.png)

     
