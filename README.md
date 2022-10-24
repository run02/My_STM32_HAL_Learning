# 湖大STM32开发板HAL库上层驱动

最近学校开了STM32的课,教学使用的芯片是湖南大学的基于STM32F103VBT6的板子,板子上集成了Jlink调试(也就是说接一根数据线就能使用板子上自带的Jlink调试),自带很多小模块:LED,蜂鸣器,数码管,光敏,按键,等等.

美中不足的是给出的示例程序都是固件库(ST官方已经停止对固件库的维护),开发环境用的是keil.

自己比较喜欢用HAL库+CLion+CubeMX这套环境,  反正也要写实验报告, 正好借此机会封装一些模块. 这样对这套板子也有HAL库的示例程序了,后边的同学如果想用HAL库开发,看这个找个例程使用起来也会舒服的多. 

## 仓库的文件结构

![image-20221019210529855](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019210529855.png)

箭头标注的以及圈起来的是我们自己可以写代码的区域,其它的都来自ST提供的初始化代码.

每个小模块的驱动都写在board目录下,

头文件在board/Inc目录下,就是那个myAPI头文件,驱动封装好了直接给出上层调用的接口,加起来也不会有多少所以就放到一个头文件里了.

**每个模块会在下文中给出快速使用的例子和简介.**

## 开发板

![image-20221019204953177](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019204953177.png)

![image-20221019204900926](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221019204900926.png)

## 环境配置

这个比较慢,先留个坑

### STM32CubeIDE

CubeIDE是ST官方给出的开发环境, 对于装环境而言是最快速的, 基本上下载好了打开个工程就可以运行, 用起来和Eclipse差不多.

### CLion+CubeMX+OpenOcd+Arm-None-Eabi+MinGW+J-Link

这套环境用起来最舒服,但配起来相对而言麻烦一些,这里太麻烦了,先欠着,后续随缘补上.

### CubeMX+Keil

特别钟爱Keil也可以用CubeMX+Keil进行开发

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
extern char Tube_String8[8][2];
void smg_init(void);   //数码管初始化
void digital_tube_display(int pos,int num); //按照字母表里的数字显示
void digital_tube_display_char(int pos,char *c); //可显示0-9,a-z的字符,带小数点的.
void digital_tube_display_string(int pos,char *s);//从指定位置开始显示任意值,可显示浮点数,最多显示的数量取决于剩下多少数码管
void digital_tube_display_string_IT(void);//在中断中刷新数码管
void test_smg_in_while1(void);//放在大循环中用于测试数码管
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

`extern char Tube_String8[8][2]`

>  这个变量用来存每一个数码管显示什么,因为可能有显示小数点的需求,所以每一个长度是两位,这样就放得类似于`"1."`这样的内容了.
>
> 板子上一共有8个数码管,所以长度是8.
>
> 支持显示1-9,a-z,带小数点的1-9.



- 在第0位显示数字1:

  ```c
  Tube_String8[0][0]=1+'0';
  Tube_String8[0][1]='\0';
  ```

- 在第零位和第一位显示`1.2`:

  ```c
  Tube_String8[0][0]=1+'0';
  Tube_String8[0][1]='.';
  Tube_String8[1][0]=2+'0';
  Tube_String8[1][1]='\0';
  ```

- 在第二位显示字幕H

  ```c
  Tube_String8[2][0]='H';
  Tube_String8[2][1]='\0';
  ```

`Lab2`中使用到数码管的关键部分:

```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
        Tube_String8[0][0]=cnt_press/100%10+'0';
        Tube_String8[0][1]=0;
        Tube_String8[1][0]=cnt_press/10%10+'0';
        Tube_String8[0][1]=0;
        Tube_String8[2][0]=cnt_press%10+'0';
        Tube_String8[0][1]=0;
        digital_tube_display_string_IT();
    }
}
/* USER CODE END 4 */
```

### 蜂鸣器

目前开发尚未完善.

### 按键

关于按键其实代码要写的并不多,只需要在CubeMX中配置,然后在对应的回调函数中写处理的逻辑即可.