//
// Created by Ryan on 2022/10/12.
//
#include "myAPI.h"
#define L0 GPIO_PIN_8
#define L1 GPIO_PIN_9
#define L2 GPIO_PIN_10
#define L3 GPIO_PIN_11
#define L4 GPIO_PIN_12
#define L5 GPIO_PIN_13
#define L6 GPIO_PIN_14
#define L7 GPIO_PIN_15
#define ALL_LED L0|L1|L2|L3|L4|L5|L6|L7
void led_init(void){
    smg_init();//数码管初始化就比LED初始化多了个引脚,懒得改了就用这个了.
}

void led_display_bits(uint32_t value)//LED位带操作(还没写呢)
{
    uint32_t l0=L0;
    value=value&0xff;
    while (l0>>=1)
        value<<=1;
    HAL_GPIO_WritePin(GPIOE, ALL_LED, 0);
    HAL_GPIO_WritePin(GPIOE, value, 1);
}
void led_display_write_bit(int pos,int value)//设置指定LED灯的状态
{
    HAL_GPIO_WritePin(GPIOE, L0 << pos, value&0x01);
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