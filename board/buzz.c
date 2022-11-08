//
// Created by Ryan on 2022/10/12.

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

