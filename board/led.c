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