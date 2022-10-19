//
// Created by Ryan on 2022/10/12.
//
//#include "main.h"
//#include "stm32f1xx_hal_rcc.h"
#include "myAPI.h"
#define u16 uint16_t
#define u32 uint32_t
#define u8 uint8_t
#define uc16 unsigned int
#define PBeep(x) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,(x))
#define CPU_FREQUENCY_MHZ    8	// STM32时钟主频
static int play_flag=0;
void enable_play(void){
    play_flag=1;
}
void disable_play(void){
    play_flag=0;
}

void set_up(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
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
    //             低7  1   2   3   4   5   6   7  高1 高2  高3 高4 高5 不发音
    uc16 tone[] = {247,262,294,330,349,392,440,494,523,587,659,698,784,850,1000};//音频数据表

    //小燕子
    u8 music[]={2,1,2,1,2,3,5,3,
                2,1,2,1,2,3,2,1,6,
                2,1,2,1,2,3,5,3,
                2,3,2,1,2,0,
                2,1,2,1,2,3,5,3,
                2,3,2,1,6,3,2,1,2
    };

    u8 time[] ={6,2,6,2,4,4,4,4,//时间
                6,2,6,2,2,2,2,8,
                6,2,6,2,4,4,4,4,
                6,2,4,4,8,8,
                6,2,6,2,4,4,4,4,
                6,2,4,4,8,2,2,2,2
    };


    u32 yanshi;
    u16 i,e;
    yanshi = 6;//10;  4;  2
    set_up();
    for(i=0;i<46;i++){
        for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++){
            Sound((u32)tone[music[i]]);
        }
    }
}

