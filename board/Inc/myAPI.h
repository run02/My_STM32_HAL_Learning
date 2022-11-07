#ifndef _myAPI_h_
#define _myAPI_h_
#include "main.h"

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
void buzz_init(void);
void buzz(int value);
void play();//播放阴乐

//adc转换
float calculate_to_temperature(int ad);/*把12位温度模拟量转换位浮点数的温度*/
int measure_the_temperature(void);/*以阻塞的方式测量一次温度,12位精度*/


//按键初始化
void key_exti_gpio_init(void);
/*按键的其它功能使用回调函数实现,里边主要是处理逻辑,不太好写对外提供的服务*/
#endif