# 实验6 温度报警

本次实验主要是做一个温度报警装置,显示温度和设置的阈值,按键调整阈值,温度高于阈值的时候蜂鸣器开始鸣响,LED播放流水灯.

实验效果大致如下:

![image-20221108130440064](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108130440064.png)

## 实验目的

1.熟悉热敏电阻的温度特性；

2.掌握STM32单片机片内12位ADC的工作原理；

3.掌握数码管显示、LED控制、无源蜂鸣器控制。

## 实验内容

1. 开发板上数码管左边两位（十进制整数）显示温度的最大阈值，右边三位（十进制）显示当前温度值，精确到小数点后1位。

2. 通过改变Rt器件接触到的温度，可观察到不同的当前温度（可用手接触Rt器件，也可对其吹气等等方式）。

3. 当数码管右边的当前温度值超出数码管左边的温度最大阈值时，LED灯从L0到L7依次循环高亮，同时蜂鸣器“嘟、嘟、嘟”的报警。

4. 可通过RESET键对其复位，回到初始状态。

5. 根据实际情况，可通过按键K1、K2调节温度最大阈值。每按一次K1，阈值加1；每按一次K2，阈值减1。

## 原理

### 读ADC

[STM32 ADC详细篇（基于HAL库） - 东小东 - 博客园 (cnblogs.com)](https://www.cnblogs.com/dongxiaodong/p/14355843.html)

参考这篇文章,可以很快用起来读ADC的操作.

看原理图找到温度连接在PC5,ADC15通道上.

![image-20221108125148375](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108125148375.png)

![image-20221108125012842](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108125012842.png)

### 温度计算

温度测出来的时候是一个12位精度的数字量,要把它转成温度,需要找个什么公式换算一下,

参考这篇文章:

[ 热敏电阻温度计算 公式 程序_yangyang_1024的博客-CSDN博客_热敏电阻计算公式](https://blog.csdn.net/yangyang_1024/article/details/80563522)

![image-20221108124242898](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108124242898.png)





于是得到了算温度的函数,最后有偏差可以±一个常数去抵消一下.

```c
#define R_rt 10000.0
float calculate_to_temperature(int ad){
    float v=((float)ad)/4095*3.3;
    float r=(v*R_rt)/(3.3-v);//10kΩ
    float t=(1/(log(r/R_rt)/3950.0+(1/(273.15+23.0))))-273.15-3.4;
    return  t;
}
```

### 其它模块的原理图



![image-20221108125128135](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108125128135.png)



![image-20221108125028932](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108125028932.png)



![image-20221108125050699](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108125050699.png)

## 实验过程

### 温度设置

![image-20221108130810202](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108130810202.png)

### 引脚图

![image-20221108130830794](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108130830794.png)

### 中断优先级设置

![image-20221108130852643](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108130852643.png)

### 定时器

简单计算一下多少毫秒进入一次中断服务函数,详细讲解可以看这篇文章.

[HAL库教程8：通用定时器的使用_yummy说电子的博客-CSDN博客](https://blog.csdn.net/geek_monkey/article/details/89165205)

![在这里插入图片描述](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//20190409225254887.png)

![image-20221108131033694](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108131033694.png)



其他部分照旧,可以使用CUBEMX来配置,也可以用之前写好的初始化函数.



## 代码

这次代码要用的有点多,但好在都是之前已经封装的差不多了的模块.主函数自己要写的地方一共就那么几行,业务逻辑并不复杂. 但是需要用到之前写的驱动.

### 整体逻辑

```c
#include "main.h"

#include "myAPI.h"

uint8_t rx_buffer[400]={0};
int threshold=23;
float temp;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {

        buzz_it((int)temp,threshold,0,0);
        if((int)temp>threshold)
            lsd_it();
        else
            digital_tube_display_string_IT();
    }
}

int main(void)
{
 
  /* USER CODE BEGIN 2 */
    smg_init();
    buzz_init();
    HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        temp=calculate_to_temperature(measure_the_temperature());
        play_float_it(0,3, temp,1);
        play_num_it(4,7, threshold);
        HAL_Delay(300);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

```



### boardLayout.h

```c
#ifndef _boardLayout_h_
#define _boardLayout_h_


/*LED与数码管*/
#define LED_SEL GPIO_PIN_3 //LED灯开关
#define LED_GROUP GPIOE

#define SEL0 GPIO_PIN_0 //译码器A0
#define SEL1 GPIO_PIN_1 //译码器A1
#define SEL2 GPIO_PIN_2 //译码器A2

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
/*LED与数码管*/

/*按键*/
#define Key_GPIO_Group GPIOC
#define K3 GPIO_PIN_0
#define K2 GPIO_PIN_1
#define K1 GPIO_PIN_2
/*按键*/

/*蜂鸣器*/
#define BUZZ_GROUP GPIOE
#define BUZZ_PIN GPIO_PIN_5

#endif
```

### maAPI.h

```c
#ifndef _myAPI_h_
#define _myAPI_h_
#include "main.h"
#include "boardLayout.h"
//LED灯
void led_init(void);
void led_display_bits(uint32_t value);//LED位带操作
void led_display_write_bit(int pos,int value);//设置指定LED灯的状态
void test_led(void);//放在大循环中用于测试LED的两个函数
#if !defined(IT_Freq)
#define IT_Freq 1 //1ms 进入一次中断
#endif
void lsd_it(void);//在中断中流水灯,1s一次,需要提前知到几ms进一次中断

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
void buzz_init(void);
void buzz(int value);
void play();//播放阴乐
void buzz_it(int value,int threshold,int f,int duty);

//adc转换
float calculate_to_temperature(int ad);/*把12位温度模拟量转换位浮点数的温度*/
int measure_the_temperature(void);/*以阻塞的方式测量一次温度,12位精度*/


//按键初始化
void key_exti_gpio_init(void);
/*按键的其它功能使用回调函数实现,里边主要是处理逻辑,不太好写对外提供的服务*/
#endif
```

跟之前相比,这次的代码有亿点多.

```c
#include "myAPI.h"
#include "math.h"
extern ADC_HandleTypeDef hadc1;

#define R_rt 10000.0
float calculate_to_temperature(int ad){
    float v=((float)ad)/4095*3.3;
    float r=(v*R_rt)/(3.3-v);//10kΩ
    float t=(1/(log(r/R_rt)/3950.0+(1/(273.15+23.0))))-273.15-3.4;
    return  t;
}

int measure_the_temperature(void){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,1000);
   return HAL_ADC_GetValue(&hadc1);
}


```

### led.c

```c
//
// Created by Ryan on 2022/10/12.
//
#include "myAPI.h"

void led_init(void){
    smg_init();//数码管初始化就比LED初始化多了个引脚,懒得改了就用这个了.
}
#define LOCK_GROUP GPIOB //LED灯开关的引脚的组
void lock(){
    HAL_GPIO_WritePin(LOCK_GROUP,LED_SEL,0);
}
void unlock(){
    HAL_GPIO_WritePin(LOCK_GROUP,LED_SEL,1);
}
void led_display_bits(uint32_t value)//LED位带操作(还没写呢)
{
    uint32_t l0=L0;
    value=value&0xff;
    lock();
    while (l0>>=1)
        value<<=1;
    HAL_GPIO_WritePin(LED_GROUP, ALL_LED, 0);
    HAL_GPIO_WritePin(LED_GROUP, value, 1);
    unlock();
}
void led_display_write_bit(int pos,int value)//设置指定LED灯的状态
{   lock();
    HAL_GPIO_WritePin(LED_GROUP, L0 << pos, value&0x01);
    unlock();
}

void lsd_it(void){
    static int i=0,j=0;
    if(++i>=(uint32_t)(0x500u/IT_Freq)){
        j=j>=8?0:++j;
        led_display_bits(0x01u<<j);
        i=0;
    }
}
/*测试LED灯的函数,里边有延时,只能放到while1里*/
void test_led(void){
    static int cnt=0;
    if(cnt==0) {
        static short int i = 0;
        led_display_bits(0);
        led_display_write_bit(i,1);
        HAL_Delay(1000);
        cnt = ((i++)> 7) ? 1 : 0;
        i=cnt!=0?0:i;
    }else if(cnt==1){
        static short int i = 0;
        static uint32_t t=0xf0;
        led_display_bits(t=~t);
        HAL_Delay(1000);
        cnt = ((i++) > 7) ? 0 : 1;
        i=cnt!=1?0:i;
    }

}
```

### smg.c

```c
//
// Created by Ryan on 2022/10/12.
//
#include "main.h"
#include "myAPI.h"
#include "stdio.h"

#define TUBE_OFF 37 //数码段表熄灭的下标

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
        if(*s) {
            if (*s == '.') {
                Tube_String8[pos++][1] = '.';
                s++;
            } else
                Tube_String8[pos++][1] = '\0';
        } else
            break;
    }
}
/*在指定范围内显示整数*/
void play_num_it(int start,int end,int num){
    char fmt[]={'%',end+1-start+'0','d'};
    char n[9]={0};
    sprintf(n,fmt,num);
    n[end+1-start]='\0';
    play_string_it(start,n);
}
/*在指定空间内显示小数,可设置保留几位小数点*/
void play_float_it(int start,int end,float num,int len_after_point){
    char buffer[9];
    char fmt[]={'%',end+1-start+'0','.',len_after_point+'0','f'};
    sprintf(buffer, fmt, num);
    buffer[end+1-start]='\0';
    play_string_it(start,buffer);
}
```

### key.c

```c
#include "myAPI.h"

extern int threshold;//阈值
/*
初始化在CUBEMX里边要点一下生成代码,会在stm32f1xx_it.c中生成对应的初始化代码,在主函数里也要初始化GPIO
感觉这个再写个初始化的代码有点鸡肋,但是还是写了

 */
void key_exti_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pins : PC0 PC1 PC2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}
static inline void k1_call_back(){
    threshold++;
}
static inline void k2_call_back(){
    threshold--;
}
static inline void k3_call_back(){
//    buzz(1);
}

void HAL_GPIO_EXTI_Callback(uint16_t key) {
    if (!HAL_GPIO_ReadPin(Key_GPIO_Group, key))
        return;
    if (key == K3) {
        k3_call_back();
    } else if (key == K2) {
        k2_call_back();
    } else if (key == K1) {
        k1_call_back();
    }
    HAL_Delay(200);/*把系统时钟的优先级设置为比按键高,然后就可以在这里延时了,不然会直接卡死*/
}
```



### buzz.c

```c
//
// Created by Ryan on 2022/10/12.

#include "myAPI.h"
#define u16 uint16_t
#define u32 uint32_t
#define u8 uint8_t
#define uc16 unsigned int

#define PBeep(x) HAL_GPIO_WritePin(BUZZ_GROUP,BUZZ_PIN,(x))
#define CPU_FREQUENCY_MHZ  8   // STM32时钟主频



void buzz_init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BUZZ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZ_GROUP, &GPIO_InitStruct);
}


void buzz(int value){
    PBeep(value);
}
/*
 * 这个函数要放到中断里,value小于threshold(阈值)会发出叫声
 * f和duty如果是0就使用默认的参数
 * 您也修改 #define BUZZ_FREQUENCY 与#define BUZZ_DUTY 来改变默认参数
 * 因为是有源蜂鸣器,这个引脚上有没有PWM方波,
 * 驱动有源蜂鸣器要用脉冲,只好用定时器模拟一个pwm了
 * 缺点是蜂鸣器叫起来比较难听
*/
#define BUZZ_FREQUENCY 10
#define BUZZ_DUTY 5
void buzz_it(int value,int threshold,int f,int duty){
    f=f==0?BUZZ_FREQUENCY:f;
    duty=duty==0?BUZZ_DUTY:duty;
    if (value>threshold){
        static int i=0;
        if(i<BUZZ_DUTY){
            buzz(0);
        }else{
            buzz(1);
        }
        if(i++==BUZZ_FREQUENCY){
            i=0;
        }
    }
}



void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}


void Sound(u16 frq)
{
    u32 time;
    if(frq != 1000)
    {
        time = 500000/((u32)frq);
        PBeep(1);
        delay_us(time);
        PBeep(0);

        delay_us(time);
    }else
        delay_us(1000);
}

void play(void)
{
    extern int buzz_play_flag;
//             低7  1   2   3   4   5   6   7  高1 高2  高3 高4 高5 不发音
    const uc16 tone[] = {247,262,294,330,349,392,440,494,523,587,659,698,784,850,1000};//音频数据表

//小燕子
    const u8 music[]={2,1,2,1,2,3,5,3,
                      2,1,2,1,2,3,2,1,6,
                      2,1,2,1,2,3,5,3,
                      2,3,2,1,2,0,
                      2,1,2,1,2,3,5,3,
                      2,3,2,1,6,3,2,1,2
    };

    const u8 time[] ={6,2,6,2,4,4,4,4,//时间
                      6,2,6,2,2,2,2,8,
                      6,2,6,2,4,4,4,4,
                      6,2,4,4,8,8,
                      6,2,6,2,4,4,4,4,
                      6,2,4,4,8,2,2,2,2
    };
    u32 yanshi;
    u16 i,e;
    yanshi = 6;//10;  4;  2

    if(buzz_play_flag == 0){
        for(i=0;i<46;i++){
            for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++){
                if(buzz_play_flag != 0)
                    return;
                Sound((u32)tone[music[i]]);
            }
        }
    }
    else if(buzz_play_flag == 1){
        for(i=45;i>0;i--){
            for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++){
                if(buzz_play_flag != 1)
                    return;
                Sound((u32)tone[music[i]]);
            }
        }
    }
}
```

### main.c

```c
#include "main.h"

#include "myAPI.h"

ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;


uint8_t rx_buffer[400]={0};
int threshold=23;
float temp;

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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {

        buzz_it((int)temp,threshold,0,0);
        if((int)temp>threshold)
            lsd_it();
        else
            digital_tube_display_string_IT();
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
  /* USER CODE BEGIN 2 */
    smg_init();
    buzz_init();
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_UART_Receive_IT(&huart1,rx_buffer,100);
    HAL_UART_Receive_DMA(&huart1,rx_buffer,100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        temp=calculate_to_temperature(measure_the_temperature());
        play_float_it(0,3, temp,1);
        play_num_it(4,7, threshold);
        HAL_Delay(300);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

```

## 实验结果

![image-20221108130440064](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108130440064.png)





## 参考

[STM32 ADC详细篇（基于HAL库） - 东小东 - 博客园 (cnblogs.com)](https://www.cnblogs.com/dongxiaodong/p/14355843.html)

[ 热敏电阻温度计算 公式 程序_yangyang_1024的博客-CSDN博客_热敏电阻计算公式](https://blog.csdn.net/yangyang_1024/article/details/80563522)

[ntc热敏电阻与温度对照表（5K、10k、15K、50K、100K R/T阻值表） (thermistors.cn)](http://www.thermistors.cn/news/238.html)

[HAL库教程8：通用定时器的使用_yummy说电子的博客-CSDN博客](https://blog.csdn.net/geek_monkey/article/details/89165205)
