# 实验5 IIC读写EEPROM

本次实验主要是用IIC先向EEPROM写一些数据,然后再用IIC总线读出来,并且输出到串口.

实验效果大致如下:

>  按下复位键,每次重启会向EEOROM中连续的区域写一串字符,在主函数中每隔两秒读一次.
>
> ![image-20221109142957057](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109142957057.png)



## 实验目的

1.掌握I2C总线的工作原理；

2.掌握利用STM32单片机GPIO模拟I2C总线的程序设计方法；

3.掌握单片机读写AT24C02 EEPROM的程序设计方法。

## 实验内容

- 利用STM32F103VBT6单片机读写AT24C02 EEPROM。

  - 具体要求

    首先STM32F103VBT6单片机向AT24C02 EEPROM一次性连续写入若干个数，然后STM32F103VBT6单片机再从AT24C02 EEPROM中将之前面写入的全部数读出来，并通过串口输出到PC机上。

## 原理

### 原理图

#### IIC连线图

![image-20221109143822098](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109143822098.png)



![image-20221109143950799](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109143950799.png)



#### 串口连线图

![image-20221109144154904](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109144154904.png)

![image-20221109144309492](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109144309492.png)





### IIC-AT24C02原理概述

#### IIC

IIC是一种总线通讯协议,其中一根作为时钟线,另一根作为数据线.

IIC总线是一对多的总线,一个主机可以连接多个设备,像是一栋楼里边住很多人,主机可以通过门牌号(IIC的地址)来找到对应的人(IIC设备).

#### AT24C02 EEPROM芯片

这块芯片用于提供存储,存储结构大概是像一页纸,每次可以指定写在这张纸的第几行(存储器的地址),一行大概有8厘米宽(一个存储器可以写8个bit,也就是1B,一个字节,或者是1个char的大小).

读也是一样. 

简单了解后,发现在AT24C02某个寄存器写某个值,只需要直到IIC的地址以及寄存器的地址,就可以往寄存器里写东西了.

#### 如何使用IIC通信

刚才已经知道大致的流程了,想往指定的内存写一个字节,只需要准备好要写的东西,IIC设备的地址,寄存器的地址,然后按照IIC传输数据的规则来写数据就好了.

其实这里还没结束,因为读写数据两种操作不一样,在通信的时候还是需要区分这两种意图,如果仅按照上述方法,读写都在一个地方,至少需要再发一次来区分读或者是写的意图.

AT24C02采取的方案是读写分成两个地址,这样寻址完了接着传数据,省了一步区分意图的过程.



这里推荐一篇比较详细的文章,[HAL库 STM32CubeMX教程十二---IIC(读取AT24C02 )](https://blog.csdn.net/as480133937/article/details/105259075),详细的讲述了IIC的通信过程.这里仅把我认为的重点列出:



**写24C02的时候**，**从器件地址为10100000（0xA0）；
读24C02的时候，从器件地址为10100001（0xA1）。**

**在写数据的过程中，每成功写入一个字节，E2PROM存储空间的地址就会自动加1，当加到0xFF后，再写一个字节，地址就会溢出又变成0x00。**

写数据的时候需要注意，E2PROM是先写到缓冲区，然后再“搬运到”到掉电非易失区。所以这个过程需要一定的时间，AT24C02这个过程是不超过5ms！
所以，**当我们在写多个字节时，写入一个字节之后，再写入下一个字节之前，必须延时5ms才可以**

![image-20221109150459485](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109150459485.png)

#### HAL库读写IIC函数简介

HAL库对使用IIC读写连续内存做了更加上层的封装:

下面四个函数分别是接收IIC数据,读内存,写内存,发送IIC数据的函数.详情可以点进文档查看.

IIC还有DMA,中断等方式,使用方法与串口类似.

![image-20221109150853796](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109150853796.png)

![image-20221109150901290](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109150901290.png)

![image-20221109150909383](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109150909383.png)

![image-20221109150916575](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109150916575.png)

## 实验过程

![image-20221109151259971](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109151259971.png)

选择主模式:

![image-20221109151335493](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109151335493.png)

检查一下GPIO引脚是否与原理图一致

![image-20221109151318553](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109151318553.png)



![image-20221109151414863](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109151414863.png)

串口配置与上个实验一致,不再赘述.

## 代码

```c
#include "main.h"
#include "myAPI.h"

#define ADDR_EEPROM_Write 0xA0
#define ADDR_EEPROM_Read 0xA1

ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
//uint8_t tx_buffer[]="This is transmit by DMA\n";
uint8_t rx_buffer[400]={0};
int threshold=23;
uint8_t i2c_tx_buffer[]={'h','e','l','l','o','\0'};
uint8_t i2c_rx_buffer[8];
float temp;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle == &huart1){
        /*接收数据完成*/
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
  MX_I2C1_Init();
  MX_ADC1_Init();
 
    smg_init();
    buzz_init();
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_UART_Receive_IT(&huart1,rx_buffer,100);
    HAL_UART_Receive_DMA(&huart1,rx_buffer,100);
    HAL_UART_Transmit(&huart1,"write iic: \n",sizeof "write iic: \n",0xff);
    HAL_UART_Transmit(&huart1,i2c_tx_buffer,sizeof i2c_tx_buffer,0xff);
    HAL_UART_Transmit(&huart1,"\n",sizeof "\n",0xff);
    HAL_I2C_Mem_Write(&hi2c1,ADDR_EEPROM_Write,0,I2C_MEMADD_SIZE_8BIT,i2c_tx_buffer,sizeof i2c_tx_buffer,0xff);

    while (1)
    {
        HAL_Delay(2000);
        HAL_I2C_Mem_Read(&hi2c1,ADDR_EEPROM_Write,0,I2C_MEMADD_SIZE_8BIT,i2c_rx_buffer,sizeof i2c_tx_buffer,0xff);
        HAL_UART_Transmit(&huart1,"read iic: \n",sizeof "read iic: \n",0xff);
        HAL_UART_Transmit(&huart1,i2c_rx_buffer,sizeof i2c_tx_buffer,0xff);
  
}
/*//=========需要自己手写的大概到上边,下边的是通过配置CUBEMX生成的初始化代码==//*/

static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }


}

static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : temperature_Pin */
  GPIO_InitStruct.Pin = temperature_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(temperature_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE9 PE10 PE11
                           PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}


```

## 实验结果

 按下复位键,每次重启会向EEOROM中连续的区域写一串字符,在主函数中每隔两秒读一次.

> ![image-20221109142957057](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221109142957057.png)





## 参考

[【STM32】HAL库 STM32CubeMX教程十二---IIC(读取AT24C02 )_Z小旋的博客-CSDN博客_hal_i2c_master_transmit](https://blog.csdn.net/as480133937/article/details/105259075)
