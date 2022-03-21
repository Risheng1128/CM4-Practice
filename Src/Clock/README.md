# Clock Implementation
###### tags: `ARM Cortex-M` `ARM Practice`

- STM32F303ZE Clock Tree
  ![](https://i.imgur.com/P1XjtyZ.png)

- Flash latency &rarr; 根據 `HCLK` 不同，存取 flash 的 cycle 會不同
  - Flash base address
    ![](https://i.imgur.com/ntSfkmY.png)

  ![](https://i.imgur.com/f9yqdBh.png)
  ![](https://i.imgur.com/oBm7jAx.png)

## Exercise `1`
- 目標: `SYSTICK` configuration
  1. Using `HSE` Configure the `SYSCLK` as `8MHz`
  2. `AHB` clock as `4MHz` (`HCLK`)
  3. `APB1` clock as `2MHz` (`PCLK1`)
  4. `APB2` clock as `2MHz` (`PCLK2`)
  5. 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

- 設定
  - `HSE` 預設為 `8Hz`
  - `HCLK` prescalar 設為 `2`
  - `APB1` Low-speed prescaler 設為 `2`
  - `APB2` Low-speed prescaler 設為 `2`

- 結果
  ![](https://i.imgur.com/geHSjpL.png)
  
## Exercise `2`
- 目標: `PLL` configuration via `HSI`
  - Write an application to generate below `HCLK` freqencies using `PLL`
    1. 20MHz
       - `APB1`: `20MHz`
       - `APB2`: `20MHz`
    2. 40MHz
       - `APB1`: `20MHz`
       - `APB2`: `40MHz`
    4. 60MHz
       - `APB1`: `30MHz`
       - `APB2`: `60MHz`
  - Use `PLL` as `PLL's` input source and repeat the exercise using `HSE` as input source
  - 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

- 結果(三者結果一樣)
  ![](https://i.imgur.com/cGCCI3P.png)
  
## Exercise `3`
- 目標: `PLL` configuration via `HSE`
  - Write an application to generate below `HCLK` freqencies using `PLL` (`APB` 和 `AHB` 的除頻都設為 `1`)
    1. 20MHz
       - `APB1`: `20MHz`
       - `APB2`: `20MHz`
    2. 40MHz
       - `APB1`: `20MHz`
       - `APB2`: `40MHz`
    3. 60MHz
       - `APB1`: `30MHz`
       - `APB2`: `60MHz`
  - Use `HSE` as `PLL's` input source and repeat the exercise using `HSE` as input source
  - 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

- 結果(三者結果一樣)
  ![](https://i.imgur.com/eNSDTMZ.png)
  
## Exercise `4`
- 目標: `PLL` configuration for `72MHz`
  - Write an application which does `PLL` configuration to boost the `HCLK` to maximum capacity
    1. `APB1`: `36MHz`
    2. `APB2`: `72MHz`
  - Use `HSE` and `HSI` as `PLL` Source respectively
  - 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

- 結果(兩者結果一樣)
  ![](https://i.imgur.com/eNSDTMZ.png)
  
## Exercise `5`
- 目標: `HSI` measurment
  - Write a program to output `HSI` clock on microcontroller pin and measure it using oscilloscope or logic analyzer
  - 將 `HSI` 預設降頻作為練習 (`1MHz`) ，表示 `MCO` 要除以 `8`
  
- 參考 STM32F303ZE 的 alternate function mapping
  - `MCO` 可以從 `PA8` 輸出(使用 `AF0`)
  ![](https://i.imgur.com/iwYAXMX.png)

- 結果 (將 `MCO` 除以 `8` 後輸出，即 `1MHz`)
  ![](https://i.imgur.com/BjwlALt.png)
  
## Excercise `6`
- 目標: `HSE` measurement
  - Write a program to switch `HSE` as system clock and measure it

- 結果 (將 `MCO` 除以 `8` 後輸出，即 `1MHz`)
  ![](https://i.imgur.com/uY4tHXm.png)
