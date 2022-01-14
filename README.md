# ARM Cortex-M Practice
###### tags: `ARM Cortex-M`
根據線上課程所做的一些小練習，使用開發版為NUCLEO-F303ZE

## test
- Just a simple test code

## Inline Assembly Coding
- Example 1: Load 2 values from memory, add them and storage it to memory by using inline assembly statement
             地址0x20001000初始值: 6
             地址0x20001004初始值: 4
  1. 將地址 0x20001000 及 0x20001004 分別放入 R1 及 R2
     ![](https://i.imgur.com/7PWQ7iV.png)
  2. 將 R1 及 R2 對應地址的資料分別放進 R0 及 R1
     ![](https://i.imgur.com/Bp7hF0M.png)
  3. 將 R0 及 R1 資料相加後放進 R0
     ![](https://i.imgur.com/pweeB9E.png)
  4. 將 R0 存進 R2 的地址裡
     ![](https://i.imgur.com/0IVLbN3.png)
  5. 將地址 0x20001004 讀出來並存到R2
     ![](https://i.imgur.com/1hqpqwq.png)

- Example 2: 讀取"Control" Register
  - "="表示這個指令為寫入
  ```C=
  uint32_t control_reg;
	__asm volatile("MRS %0, CONTROL" : "=r"(control_reg));
  ```  
  
- Example 3: 把一個變數複製到另一個變數
  ```C=
  uint32_t va1 = 10, var2;
  __asm volatile("MOV %0,%1": "=r"(var2): "r"(var1));
  ```
  
- Example 4: 把一個位址的值寫到另一個變數裡
  ```C=
  uint32_t ptr1, *ptr2;
  ptr2 = (uint32_t*)0x20000008;
  __asm volatile("LDR %0,[%1]": "=r"(ptr1): "r"(ptr2)); //p1 = *p2
  ```
  
## Bit Band
- 目的: 給定bit band address 及 bit position ，並計算 bit band alias address，再利用 bit band alias address 修改其值
  - memory location: 0x2000_0200， 初始值: 0xff
  - bit position: 7

  1. 使用 bitwise method
  ![](https://i.imgur.com/2Jjxi3x.png)
  2. 使用 bit band method
  ![](https://i.imgur.com/WVnp9jf.png)
     
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


## Configurable fault exception
- Exercise 1


- Exercise 2

## How to use
- 只要改TARGET的目標檔案路徑，接著make即可
  ```makefile=
    # 目標編譯檔案
    TARGET = test/main
    # 編譯檔案資料夾
    BUILD_DIR = Debug
  ```
