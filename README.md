# 项目过时声明
非常感谢大家对我的 STM32 HAL库开发学习仓库的关注和使用。这个仓库记录了我在学习 STM32 开发过程中的笔记和代码示例。我已经将归还需要用到的开发板，未来我将无法添加新的案例或进行更新。

请注意，这些资料和代码示例是在我学习初期整理和编写的，可能在编程实践和代码质量方面不尽完美。但是，这些内容设计简单，适合初学者理解和快速上手，特别是对于那些刚接触 STM32 开发的新手来说，能够提供一个易于理解的学习起点, 能够快速运行出相关功能。

我鼓励大家使用和参考这些资料，同时也欢迎社区成员基于现有内容进行改进和扩展。感谢大家过去的支持，希望我的工作能对你们有所帮助。

祝编程愉快！

# 基于湖大STM32教学开发板 HAL库学习
- [基于湖大STM32教学开发板 HAL库学习](#基于湖大stm32教学开发板-hal库学习)
- [仓库说明](#仓库说明)
  - [0. 简介](#0-简介)
    - [**此仓库的作用:**](#此仓库的作用)
    - [六个实验与分支:](#六个实验与分支)
  - [1. 所使用的开发板](#1-所使用的开发板)
  - [2. 环境配置](#2-环境配置)
    - [如何使用STM32CubeIDE运行示例工程](#如何使用stm32cubeide运行示例工程)
    - [CLion+CubeMX+OpenOcd+Arm-None-Eabi+MinGW+J-Link](#clioncubemxopenocdarm-none-eabimingwj-link)
    - [CubeMX+Keil](#cubemxkeil)
  - [3. 仓库的文件结构](#3-仓库的文件结构)
- [接口文档与驱动源码](#接口文档与驱动源码)
    - [LED](#led)
      - [接口介绍](#接口介绍)
      - [一个极简的例子](#一个极简的例子)
    - [数码管](#数码管)
      - [提供的上层接口](#提供的上层接口)
      - [使用方法简介](#使用方法简介)
      - [示例](#示例)
      - [代码实现接口](#代码实现接口)
    - [蜂鸣器](#蜂鸣器)
      - [提供的上层接口](#提供的上层接口-1)
      - [代码实现接口](#代码实现接口-1)
    - [按键](#按键)
    - [温度转换](#温度转换)
      - [提供的上层接口](#提供的上层接口-2)
      - [代码实现接口](#代码实现接口-2)
    - [串口与DMA](#串口与dma)
    - [IIC读写EEPROM](#iic读写eeprom)
    - [ADC](#adc)
    - [RCC时钟配置](#rcc时钟配置)

# 仓库说明

## 0. 简介

学校专门订购了一批为`STM32`教学量身打造的开发板,板子上集成了`J-Link调试器`(也就是说接一根数据线就能使用板子上自带的`J-Link`调试),自带很多小模块:LED,蜂鸣器,数码管,光敏,按键,等等,非常的紧致精巧.

**美中不足的是给出的示例程序都是固件库**(ST官方已经停止对固件库的维护),开发环境用的是keil.

自己比较喜欢用`HAL库+CLion+CubeMX`这套环境, 正好借此机会封装一些模块. 这样对这套板子也有HAL库的示例程序了,后边的同学如果想用HAL库开发,看这个找个例程使用起来也会舒服的多.

### **此仓库的作用:**

- 基于HAL库+开发板的各部分驱动模块
- 示例程序与[**接口文档**](#接口文档与驱动源码)
- 基于`CubeMX`+`Clion`或`CubeIDE`的实验方法
- 关于六个实验的HAL库示例程序

### 六个实验与分支:

您可以通过以下命令下载此仓库:

```shell
git clone https://github.com/LiRunJi/My_STM32_HAL_Learning.git
```

进入文件夹后,使用以下命令来切换到具体对应的实验:

也可以手动切换分支

![image-20221108154207182](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108154207182.png)

可以通过`Git`(非常推荐)下载示例工程,也可以通过压缩包下载每个分支的示例工程.

![image-20221108173526418](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108173526418.png)

经过[配置](###如何使用STM32CubeIDE运行示例工程 )后,可以轻松运行如下的各个分支:

- LED灯和数码管

  ```shell
  git fetch
  git checkout lab2-smg
  ```

- 按键和电子音乐与数码管显示

  ```shell
  git checkout lab2-smg
  ```

- 输出PWM方波

  ```shell
  git fetch
  git checkout lab3-pwm
  ```

- 串口DMA

  ```shell
  git fetch
  git checkout lab4-uartdma
  ```
  
- IIC总线读写EEPROM

  ```shell
  git fetch
  git checkout lab5-eeprom
  ```

- ADC测温度,设置阈值报警

  ```sh
  git fetch
  git checkout lab6-adcalarm
  ```
  
- *****(实验外的分支)用外部晶振替代内部时钟

  ```sh
  git fetch
  git checkout rcc-config
  ```

- 使用FREERTOS的分支

  ```
  随缘开发...
  有50%的概率会在12月前出现
  ```

  



## 1. 所使用的开发板

![image-20221019204953177](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019204953177.png)

![image-20221019204900926](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019204900926.png)

## 2. 环境配置

这个比较慢,先留个坑

### 如何使用STM32CubeIDE运行示例工程

CubeIDE是ST官方给出的开发环境, 对于装环境而言是最快速的, 基本上下载好了打开个工程就可以运行, 用起来和Eclipse差不多.

1. 导入工程

   ![image-20221108150443542](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108150443542.png)

   ![image-20221108150609634](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108150609634.png)

2. 向工程中添加文件

   ![image-20221108150841910](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108150841910.png)

   ![image-20221108150938020](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108150938020.png)

   ![image-20221108151102894](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151102894.png)

   ![image-20221108151133755](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151133755.png)

   添加完成后会在这里出现:

   ![image-20221108151244012](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151244012.png)

   点击`ADD Floder`

   ![image-20221108151342570](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151342570.png)

   ![image-20221108151437008](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151437008.png)

   ![image-20221108151500423](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151500423.png)

3. 配置下载器配置为J-Link

   ![image-20221108151635660](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151635660.png)

   ![image-20221108151600881](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151600881.png)

4. 配置完成,点击![image-20221108151707470](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151707470.png)下载到开发板并运行,点击![image-20221108151751928](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221108151751928.png)可以打断点调试.您也可以按照上述同样的方法添加自己的c源文件.

   





### CLion+CubeMX+OpenOcd+Arm-None-Eabi+MinGW+J-Link

这套环境用起来最舒服,但配起来相对而言麻烦一些,这里太麻烦了,先欠着,后续随缘补上.

### CubeMX+Keil

特别钟爱Keil也可以用CubeMX+Keil进行开发

## 3. 仓库的文件结构

![image-20221019210529855](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019210529855.png)

箭头标注的以及圈起来的是我们自己可以写代码的区域,其它的都来自ST提供的初始化代码.

- 每个小模块的驱动都写在`board`目录下,


- 头文件在board/Inc目录下的`myAPI.h`头文件

  有直接给出上层调用的接口与注释,一个头文件中包含所有模块可直接调用的上层驱动.每个模块会在接口文档中给出快速使用的例子和简介.

值得注意的是使用该工程文件,每次使用`CubeMX`生成初始化代码后需要把`CMakeList.txt`文件回复为这里的版本,或者在52行和57行附近按下图所示加入` board/Inc` 与` "board/*.*"`,并重新加载`CMake`项目.

![image-20221104213459067](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221104213459067.png)

您也可以按照类似的方法添加您的代码.

# 接口文档与驱动源码

目前的接口文档不太完善,但是能用,这里先挖个坑,后续再填.

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
void buzz_it(int value,int threshold,int f,int duty);/**/

//adc转换
float calculate_to_temperature(int ad);/*把12位温度模拟量转换位浮点数的温度*/
int measure_the_temperature(void);/*以阻塞的方式测量一次温度,12位精度*/


//按键初始化
void key_exti_gpio_init(void);
/*按键的其它功能使用回调函数实现,里边主要是处理逻辑,不太好写对外提供的服务*/
#endif

```



### LED

#### 接口介绍

- `void led_init(void);`

  初始化LED灯

- `void led_display_bits(uint32_t value);`

  一下子设置八位LED的状态,从左到右依次对应`L7`->`L0`

- `void led_display_write_bit(int pos,int value);`

  LED位带操作

- `void test_led(void);`

  放在大循环中用于测试LED的两个函数

#### 一个极简的例子

注意,cubemx生成的主函数中自带的代码千万不要删掉,仅把这里有的加到对应的位置上去即可.

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

#### 提供的上层接口

```c
//数码管
void smg_init(void);   //数码管初始化

void digital_tube_display_string_IT(void);//在中断中刷新数码管

void play_string_it(int pos,const char *s);/*从指定位开始显示字符串*/

void play_num_it(int start,int end,int num);/*在指定范围内显示整数*/

/*在指定空间内显示小数,可设置保留几位小数点*/
void play_float_it(int start,int end,float num,int len_after_point);

void digital_tube_display(int pos,int num); //按照字母表里的数字显示
void digital_tube_display_char(int pos,const char *c); //可显示0-9,a-z的字符,带小数点的.
void digital_tube_display_string(int pos,char *s);//从指定位置开始显示任意值,可显示浮点数,最多显示的数量取决于剩下多少数码管
void test_smg_in_while1(void);//放在大循环中用于测试数码管

```

#### 使用方法简介

- 初始化

![image-20221102145334895](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221102145334895.png)

- `void digital_tube_display_string_IT(void);//在中断中刷新数码管`

  需要把这个函数放到中断里

  ![image-20221102145111525](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221102145111525.png)

- `void play_string_it(int pos,const char *s);`

  从指定位开始显示字符串

- `void play_num_it(int start,int end,int num);`

  在指定范围内显示整数

- `void play_float_it(int start,int end,float num,int len_after_point);`

  在指定空间内显示小数,可设置保留几位小数点

#### 示例

**您可以切换到`Lab2`分支,将程序下载进入开发板**,

1. 切换到示例的分支:

   ```shell
   git fetch;
   git checkout lab2-tubeDisplay
   ```

2. 下载程序到开发板

   - 按下从右往左数第一个按键会让显示的数字-1,
   - 按下从右往左数第二个按键会播放阴乐,
   - 按下从右往左数第三个按键会让显示的数字+1.



#### 代码实现接口

```c
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



### 蜂鸣器

#### 提供的上层接口

```c
void buzz_init(void);/*初始化蜂鸣器的GPIO*/

void buzz(int value);/*给蜂鸣器连着的引脚高/低电平*/

void play();//以在主函数中延时和阻塞的方式播放一整首阴乐

/*
 * 这个函数要放到中断里,value小于threshold(阈值)会发出叫声
 * f和duty如果是0就使用默认的参数
 * 您也修改 #define BUZZ_FREQUENCY 与#define BUZZ_DUTY 来改变默认参数
 * 因为是有源蜂鸣器,这个引脚上有没有PWM方波,
 * 驱动有源蜂鸣器要用脉冲,只好用定时器模拟一个pwm了
 * 缺点是蜂鸣器叫起来比较难听
*/
void buzz_it(int value,int threshold,int f,int duty);
```

#### 代码实现接口

```c
#include "myAPI.h"
#define u16 uint16_t
#define u32 uint32_t
#define u8 uint8_t
#define uc16 unsigned int

#define PBeep(x) HAL_GPIO_WritePin(BUZZ_GROUP,BUZZ_PIN,(x))
#define CPU_FREQUENCY_MHZ  8	// STM32时钟主频

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

### 按键

关于按键其实代码要写的并不多,只需要在`CubeMX`中配置,然后在对应的回调函数中写处理的逻辑即可.

按键回调函数的代码在`key.c`中,一共写了三个按键的回调函数.

**按键要处理的逻辑可以写在这里**

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

### 温度转换

#### 提供的上层接口

```c
float calculate_to_temperature(int ad);/*把12位温度模拟量转换位浮点数的温度*/

int measure_the_temperature(void);/*以阻塞的方式测量一次温度,12位精度*/
```

#### 代码实现接口

```c
#include "myAPI.h"
#include "math.h"
extern ADC_HandleTypeDef hadc1;

#define R_rt 10000.0
#define TemperatureOffset -3.4
float calculate_to_temperature(int ad){
    float v=((float)ad)/4095*3.3;
    float r=(v*R_rt)/(3.3-v);//10kΩ
    float t=(1/(log(r/R_rt)/3950.0+(1/(273.15+23.0))))-273.15+TemperatureOffset;
    return  t;
}

int measure_the_temperature(void){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,1000);
   return HAL_ADC_GetValue(&hadc1);
}

```

### 串口与DMA

[串口与DMA实验](https://github.com/LiRunJi/My_STM32_HAL_Learning/tree/lab4-uartdma)

### IIC读写EEPROM

[EEPROM读写实验](https://github.com/LiRunJi/My_STM32_HAL_Learning/tree/lab5-eeprom)

### ADC

[ADC读温度与报警实验](https://github.com/LiRunJi/My_STM32_HAL_Learning/tree/lab6-adcalarm)

### RCC时钟配置

[RCC时钟配置](https://github.com/LiRunJi/My_STM32_HAL_Learning/tree/rcc-config)
