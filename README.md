# 实验3-通用定时器输出PWM波实验

本节实验主要是使用定时器的PWM模式输出指定频率,可以调节的PWM方波.

![image-20221104204537704](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104204537704.png)

## 参考

[(52条消息) 【STM32】HAL库 STM32CubeMX教程七---PWM输出(呼吸灯)_Z小旋的博客-CSDN博客_hal库pwm](https://blog.csdn.net/as480133937/article/details/99231677)

[(52条消息) 【STM32】系统时钟RCC详解(超详细，超全面)_Z小旋的博客-CSDN博客_rcc时钟](https://blog.csdn.net/as480133937/article/details/98845509)

## **实验内容**

1. 采用通用定时器TIM2，通过PA1端口输出频率为1000Hz的方波。

2. 采用通用定时器TIM2，通过PA0和PA1端口输出不同频率的方波（可选）。

## 实验原理



![image-20221026180215592](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026180215592.png)

引脚图:

![image-20221026180355205](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026180355205.png)

## 实验步骤

### CubeMx配置

![image-20221026175224472](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026175224472.png)

![image-20221026174212945](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026174212945.png)



![image-20221026175146479](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026175146479.png)

![image-20221026175204549](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221026175204549.png)

同一个定时器ARR都是相同的,但是只要对应频道的PSC不同,两个频道的频率就可以不同了.

### 代码

```c
/*使用cubemx配置后的初始化代码*/
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);

}
```

```c
#include "myAPI.h"  
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_TIM3_Init();
    MX_TIM2_Init();
    smg_init();
    /*使能定时器1中断*/
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    htim2.Instance->CCR2=800;
    TIM2->ARR=7999;//时钟是8mhz的,要得到1khz只需要设置为7999+1即可
    TIM2->PSC=0;  
    while (1)
  	{	
      play_num_it(1,3,TIM2->CCR2*100/TIM2->ARR);//使用数码管显示占空比
      play_num_it(4,7,8000000/((TIM2->PSC+1)*(TIM2->ARR+1)));//显示频率
      HAL_Delay(1);
      play_string_it(0,"C");
  	}

}
```

## 实验总结

这次实验主要是做pwm的实验,HAL库对相关的定时器,PWM的功能封装的都比较完善,修改占空比也非常方便,只需要修改对应的CCR寄存器即可.

