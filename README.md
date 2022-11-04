# 嵌入式原理实验2 按键和电子音乐

这一节主要是实现数码管显示按键计数,按键播放音乐.

效果图大致如下:

![image-20221104215923676](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104215923676.png)

电子音乐的目前暂时还是阴乐.放出来的不准的原因估计是没有精确到微秒的延时,想要精确又需要启用精度高的RCC时钟,这里暂时还每配出来,另外文档排版也有点问题,坑先欠一下,后续再填.

## 实验内容

1.  按键计数（4学时）。开发板上电后，数码管左边三位点亮，为000；当K1每按一次后，则显示的三位十进制数值加1，当加到999时，再按一次，值变为000，且L0闪烁3次；当K2每按下一次后，显示的值减1，当减到000时，无源蜂鸣器发出短暂的报警声，再按一次，则值变为999。
2.  电子音乐（4学时）。程序运行后通过按键K1切换功能，轮换播放歌曲。即按K1键，播放《两只老虎》，再按K1键，切换成播放另一首自选歌曲。

## 实验原理 

 

![img](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//wps14.jpg) 

 

**由第一章图得知PB3是LED灯的开关,**

**一二张图得知LED1-LED8共阳极连接在PE8-PE15上,**

**只需要打开PB3,然后操作PE8-PE15的输出电平即可控制LED灯.**

![image-20221019220431318](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220431318.png)



![image-20221019220451248](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220451248.png)



## 实验步骤:

### 引脚配置

![image-20221019220718892](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220718892.png)

### 时钟配置

![image-20221019220823964](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220823964.png)



### 中断配置

![image-20221019220648683](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220648683.png)

### 时钟树

这里用不到很高频率,默认的8MHz应该也是够用了.

![](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019220854932.png)

### 下载到开发板运行

![image-20221019221017494](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019221017494.png) 

看到类似以上信息,即证明程序已经下载成功,观察开发板的变化,,发现程序符合预期,实验成功.

## 代码

### `myAPI.h`

这个头文件用于提供基于`HAL库`对所有板载资源的上层驱动.

```c
#ifndef _myAPI_h_
#define _myAPI_h_
#include "main.h"
//extern char Tube_String8[8][2];

//LED灯
void led_init(void);
void led_display_bits(uint32_t value);//LED位带操作
void led_display_write_bit(int pos,int value);//设置指定LED灯的状态
void test_led(void);//放在大循环中用于测试LED的两个函数

//数码管
void smg_init(void);   //数码管初始化
void digital_tube_display(int pos,int num); //按照字母表里的数字显示
void digital_tube_display_char(int pos,const char *c); //可显示0-9,a-z的字符,带小数点的.
void digital_tube_display_string(int pos,char *s);//从指定位置开始显示任意值,可显示浮点数,最多显示的数量取决于剩下多少数码管
void test_smg_in_while1(void);//放在大循环中用于测试数码管
/*以下也是关于数码管的部分好用且常用*/
void digital_tube_display_string_IT(void);//在中断中刷新数码管
void play_string_it(int pos,const char *s);/*从指定位开始显示字符串*/
void play_num_it(int start,int end,int num);/*在指定范围内显示整数*/
void play_float_it(int start,int end,float num,int len_after_point);/*在指定空间内显示小数,可设置保留几位小数点*/
//蜂鸣器
void play();//播放阴乐
void enable_play(void);
void disable_play(void);
#endif
```

### `main.c`

```c
char Tube_String8[8][2]={0};
extern int cnt_press;
int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();

    smg_init();
 
    /*使能定时器3中断*/
    HAL_TIM_Base_Start_IT(&htim3);



  while (1)
  {
     cnt_press=cnt_press>=1000?0:cnt_press;
      cnt_press=cnt_press<0?999:cnt_press;
      play_num_it(1,3,-1*cnt_press);
      play_float_it(5,7,1.23,2);
      HAL_Delay(1);
      play_string_it(0,"C");
  }

}
```



```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
        digital_tube_display_string_IT();
    }
}
```

### `smg.c`

```c
//
// Created by Ryan on 2022/10/12.
//
#include "main.h"
#include "myAPI.h"
#include "stdio.h"

#define TUBE_OFF 37 //数码段表熄灭的下标
#define SEL0 GPIO_PIN_0 //译码器A0
#define SEL1 GPIO_PIN_1 //译码器A1
#define SEL2 GPIO_PIN_2 //译码器A2
#define LED_SEL GPIO_PIN_3 //LED灯开关
#define L0 GPIO_PIN_8  //LED0 (数码段的A)
#define L1 GPIO_PIN_9
#define L2 GPIO_PIN_10
#define L3 GPIO_PIN_11
#define L4 GPIO_PIN_12
#define L5 GPIO_PIN_13
#define L6 GPIO_PIN_14
#define L7 GPIO_PIN_15
#define LOCK_GROUP GPIOB //LED灯开关的引脚的组
#define LIGHT_GROUP GPIOE //LED灯的引脚的组
#define ALL_LED L0|L1|L2|L3|L4|L5|L6|L7

const short int code_table[]={
        0x3F,  //"0"
        0x06,  //"1"
        0x5B,  //"2"
        0x4F,  //"3"
        0x66,  //"4"
        0x6D,  //"5"
        0x7D,  //"6"
        0x07,  //"7"
        0x7F,  //"8"
        0x6F,  //"9"
        0x77,//a
        0x7c,//b
        0x39,//c
        0x5e,//d
        0x79,//e
        0x71,//f
        0x3d,//g
        0x76,//h
        0x0f,//i
        0x0e,//j
        0x75,//k
        0x38,//l
        0x37,//m
        0x54,//n
        0x5c,//o
        0x73,//p
        0x67,//q
        0x31,//r
        0x49,//s
        0x78,//t
        0x3e,//u
        0x1c,//v
        0x7e,//w
        0x64,//x
        0x6e,//y
        0x59,//z
        0x40,  //"-"
        0x00,  //熄灭,下边是带小数点的0-9
        0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0xf7,0xfc,0xb9,0xde,0xf9,0xf1
};
static char Tube_String8[8][2]={0};
void smg_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, ALL_LED, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LED_SEL|SEL0|SEL1|SEL2, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = ALL_LED;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED_SEL|SEL0|SEL1|SEL2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void lock(){
    HAL_GPIO_WritePin(LOCK_GROUP,LED_SEL,0);
}
static void unlock(){
    HAL_GPIO_WritePin(LOCK_GROUP,LED_SEL,1);
}
void light_group_writePin(uint16_t L, GPIO_PinState PinState){
    HAL_GPIO_WritePin(LIGHT_GROUP,L,PinState);
}

void digital_tube_display(int pos,int num){
    unlock();
    HAL_GPIO_WritePin(LOCK_GROUP,SEL0|SEL1|SEL2,0);
    HAL_GPIO_WritePin(LOCK_GROUP,pos&0x07,1);
    light_group_writePin(ALL_LED,0);
    light_group_writePin(code_table[num]<<8,1);
    lock();
}
void digital_tube_display_char(int pos,const char *c){
        if (*c >= '0' && *c <= '9' && *(c+1)=='.')
            digital_tube_display(pos, *c - '0'+TUBE_OFF+1);
        else if (*c >= '0' && *c <= '9')
            digital_tube_display(pos, *c - '0');
        else if (*c >= 'A' && *c <= 'Z')
            digital_tube_display(pos, *c - 'A' + 10);
        else if (*c >= 'a' && *c <= 'z')
            digital_tube_display(pos, *c - 'a' + 10);
        else if(*c=='-')
            digital_tube_display(pos,TUBE_OFF-1);
        else
            digital_tube_display(pos, TUBE_OFF);
}
void digital_tube_display_string(int pos,char *s){
    char temp[2];
    while (*s&&pos!=8){
        temp[0]=*s;
        if(*(s+1)=='.'){
            temp[1]='.';
            digital_tube_display_char(pos++,temp);
            s+=2;
        }else{
            temp[1]='\0';
            digital_tube_display_char(pos++,temp);
            s++;
        }
    }
}
/*简单粗暴看一下效果的函数*/
void test_smg_in_while1(void){
    digital_tube_display_string(0,"Hell012.3");
}
/*这个函数应该是使用最多的了,需要放在定时器中,进入一次刷新一个数码管,定时器中断最好在几毫秒内,这样显示起来看着舒服*/
void digital_tube_display_string_IT(void){
    static int pos=0;
    char s[2];
    s[0]=Tube_String8[pos][0];
    s[1]=Tube_String8[pos][1];
    digital_tube_display_char(pos++,s);
    pos=pos>7?0:pos;
}
/*从指定位开始显示字符串*/
void play_string_it(int pos,const char *s){
    while(*s&&pos!=8){
        Tube_String8[pos][0]=*(s++);
        if(*s=='.'){
            Tube_String8[pos++][1]='.';
            s++;
        }else
            Tube_String8[pos++][1]='\0';
    }
}
/*在指定范围内显示整数*/
void play_num_it(int start,int end,int num){
    char fmt[]={'%',end+1-start+'0','d'};
    char n[9]={0};
    sprintf(n,fmt,num);
    n[end+1]='\0';
    play_string_it(start,n);
}
/*在指定空间内显示小数,可设置保留几位小数点*/
void play_float_it(int start,int end,float num,int len_after_point){
    char buffer[9];
    char fmt[]={'%',end+1-start+'0','.',len_after_point+'0','f'};
    sprintf(buffer, fmt, num);
    buffer[end+1]='\0';
    play_string_it(start,buffer);
}

```

### `stm32f1xx_it.c`

```c
#include "myAPI.h"

#include "main.h"
#include "stm32f1xx_it.h"

void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

    cnt_press++;
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

    led_display_bits(0xf0);

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
static int i=0;
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */
    led_display_bits(0xff);
    i++;
    if(i%2)
        disable_play();
    else
    {enable_play();
        play();

    }
  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_IRQn 1 */
    led_display_bits(0x0);
    cnt_press--;
  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

}



```



## 运行效果图

![image-20221019221640587](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019221640587.png)



![image-20221019221701285](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019221701285.png)

## 总结



## 接口文档与示例

### LED

#### 接口介绍

- void led_init(void);

  初始化LED灯

- void led_display_bits(uint32_t value);

  一下子设置八位LED的状态,从左到右依次对应L7->L0

- void led_display_write_bit(int pos,int value);

  LED位带操作

- void test_led(void);

  放在大循环中用于测试LED的两个函数

#### 一个极简的例子

```c
#include "myAPI.h"
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  led_init();
  while (1)
  {	
      	//0b表示二进制的,最直观,从左到右就对应八个灯,这里是亮最左边的四个灯
		led_display_bits(0b11110000);
      	HAL_Delay(1000);//延时一秒
      	//熄灭所有灯
        led_display_bits(0b00000000);
      	HAL_Delay(1000);
  }
}
```

把以上程序粘贴到工程的main函数中,观察变化,可以很快搞定怎么用.

### 数码管

```c
//数码管
void smg_init(void);   //数码管初始化
void digital_tube_display(int pos,int num); //按照字母表里的数字显示
void digital_tube_display_char(int pos,const char *c); //可显示0-9,a-z的字符,带小数点的.
void digital_tube_display_string(int pos,char *s);//从指定位置开始显示任意值,可显示浮点数,最多显示的数量取决于剩下多少数码管
void test_smg_in_while1(void);//放在大循环中用于测试数码管
/*以下也是关于数码管的部分好用且常用*/
void digital_tube_display_string_IT(void);//在中断中刷新数码管
void play_string_it(int pos,const char *s);/*从指定位开始显示字符串*/
void play_num_it(int start,int end,int num);/*在指定范围内显示整数*/
void play_float_it(int start,int end,float num,int len_after_point);/*在指定空间内显示小数,可设置保留几位小数点*/
```

#### 一个极简的例子

```c
#include "myAPI.h"
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  smg_init();
  while (1)
  {	
      	//0b表示二进制的,最直观,从左到右就对应八个灯,这里是亮最左边的四个灯
		 digital_tube_display_string(0,"Hell012.3");
  }
}
```

把以上程序粘贴到工程的main函数中,观察变化,可以很快搞定怎么用.

#### 在中断中刷新数码管

**您可以切换到`Lab2`分支,将程序下载进入开发板**,

- 按下从右往左数第一个按键会让显示的数字-1,
- 按下从右往左数第二个按键会播放阴乐,
- 按下从右往左数第三个按键会让显示的数字+1.

#### 使用方法简介

- 初始化

![image-20221102145334895](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221102145334895.png)





及其推荐使用在中断中刷新数码管的方法.

- void digital_tube_display_string_IT(void);//在中断中刷新数码管

  需要把这个函数放到中断里

  ![image-20221102145111525](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221102145111525.png)

- void play_string_it(int pos,const char *s);*

  从指定位开始显示字符串

- void play_num_it(int start,int end,int num);

  在指定范围内显示整数

- void play_float_it(int start,int end,float num,int len_after_point);

  在指定空间内显示小数,可设置保留几位小数点

#### 示例

1. 切换到示例的分支:

   ```shell
   git checkout lab2-tubeDisplay
   ```

2. 下载程序到开发板



### 蜂鸣器

目前开发尚未完善.

### 按键

关于按键其实代码要写的并不多,只需要在CubeMX中配置,然后在对应的回调函数中写处理的逻辑即可.