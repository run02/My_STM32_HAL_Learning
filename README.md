# 实验4 DMA串口

这里是实验4的分支,本章实验主要是使用串口+DMA与其它设备通信.

下载示例程序后打开串口调试助手,连续发送一百个字符后会得到回应.

完成这个实验无需其它实验写的驱动模块,只需要按照顺序再main.c中调用HAL库提供的函数即可.
## 实验内容

**USART1（采用DMA接收数据）与PC机通信。**

**用PC端串口调试助手，向开发板的USART1（开发板的COM口）每隔200ms周期性发送一个字符，STM32F103VBT6每收到100个字符，向串口调试助手发送一个字符串的提示信息。**

## 原理

#### 潦草但是易懂的原理描述

按照课程学习的顺序,大部分是先学的计算机组成原理,然后再是单片机,然后是嵌入式原理.

接收串口有很多种方法,有直接阻塞接收的(比如说当时写智能小车的时候我就是写的阻塞接收的),有中断接收的,比如C51的串口接收(STM32也可以中断接收),还有就是DMA接收.

简单谈一下我对DMA的理解:

阻塞接收是最好理解的,就是一次只能干一件事,这次轮到收串口数据了,就等着,然后接收完了或者是发送完了再干别的事.适用于特别简单的场景.

中断接收就是有消息来了,就中断出去,很快啊,然后再回来,不影响干别的事,也能接收数据.就像是边写作业边吃零食,写一会,突然吃一口,然后再回来写.这种方式就比较灵活了,能"同时"干两件事,但是CPU会比较"累".

DMA(Data Asset Memory),就是数据来了,让另一个芯片把数据搬到我的内存里.相当于边写作业边找另一个人帮我做家务,我只需要告诉他一声帮我做家务.这种方法最爽了,缺点估计是多来了个芯片,比较贵.

感觉还是DMA接收最简单好用.

#### 原理图

这里发现板子上有个COM口,顺藤摸瓜最后找到是PA9和PA10

![image-20221104195917987](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104195917987.png)



![image-20221104200107416](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104200107416.png)

![image-20221104200138356](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104200138356.png)

用CubeMx的好处这就出来了,直接自带原理图,带配置的各种框图,简直爽到飞起.

直接看原理图看手册和直接看cube的区别就像是直接查字典和从网上搜索单词.



## 实验步骤

用cube一开始畏首畏尾不敢乱动,但是用熟练了之后发现,还是很爽的,基本上全都是那一套配置的模式.简直爽到飞起!

配置的原理部分放在参考里了,这里不再赘述.

#### 配置串口1:

这里试了一下,连接com口的线不支持USART,只能支持UART.再USART传输的时候,可以收到单片机的数据,但是不能发送数据.

![image-20221104193447459](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104193447459.png)

![image-20221104193403596](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104193403596.png)

#### 配置中断:

![image-20221104193422069](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104193422069.png)

#### 配置DMA:

这里模式还有circular模式,就是自动装填,比如说接收了1000个字之后,它自己会再接收1000个字,一直循环.发送也是,这里因为实验的需求,需要等到他收到一百个做点什么,就用回调函数的方法.

一开始本来想看看DMA有什么回调函数,但是没找到,只有串口接收的回调函数,就只好用它了.

![image-20221104193321717](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104193321717.png)

#### 代码

经过了上述配置,代码反倒是比较好写,基本上只用得到这些就能完成本次实验:

```c
uint8_t tx_buffer[]="This is transmit by DMA\n";
uint8_t rx_buffer[400]={0};
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle == &huart1){
        /*接收数据完成后*/
        if(HAL_UART_Receive_DMA(&huart1,rx_buffer,100)==HAL_OK){
            uint8_t s[]="have sent 100 words";
            HAL_UART_Transmit_DMA(&huart1,s,sizeof s);
        }
    }
}
HAL_UART_Receive_IT(&huart1,rx_buffer,100);
HAL_UART_Receive_DMA(&huart1,rx_buffer,100);
```



```c
#include "main.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
uint8_t tx_buffer[]="This is transmit by DMA\n";
uint8_t rx_buffer[400]={0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle == &huart1){
        /*接收数据完成后*/
        if(HAL_UART_Receive_DMA(&huart1,rx_buffer,100)==HAL_OK){
            uint8_t s[]="have sent 100 words";
            HAL_UART_Transmit_DMA(&huart1,s,sizeof s);
        }
    }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart1,rx_buffer,100);
    HAL_UART_Receive_DMA(&huart1,rx_buffer,100);

    while (1)
    {
//        HAL_UART_Transmit_DMA(&huart1,tx_buffer,sizeof tx_buffer);
//        HAL_Delay(1000);
//        HAL_UART_Transmit_DMA(&huart1,rx_buffer,sizeof rx_buffer);
//        HAL_Delay(1000);
    }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}
```

## 实验结果

不停发送数据,直到发送长度加起来达到一百个,收到最近发送的一百条数据后回复:

![image-20221104195215952](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104195215952.png)

## 总结



## 参考

[【STM32】HAL库 STM32CubeMX教程十一---DMA (串口DMA发送接收)_Z小旋的博客-CSDN博客_stm32cubemx dma](https://blog.csdn.net/as480133937/article/details/104827639)

[STM32+ HAL+ DMA+ USART_戈 扬的博客-CSDN博客](https://blog.csdn.net/xuzhexing/article/details/104138920#:~:text=在HAL使用DMA方式进行串口数据传输时%2CDMA全局中断模式是必需打开的，因此在DMA方式进行数据传输时（收，发），在数据传输过半，完成均会触发DMA中断；HAL_DMA_IRQHandler,(%26hdma_usart2_tx)会根据中断标识%2C调用传输过半%2F完成%2F错误%2C回调函数)





