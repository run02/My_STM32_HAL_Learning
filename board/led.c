//
// Created by Ryan on 2022/10/12.
//
#include "myAPI.h"

void led_display(){
    unsigned short int x=0b0;
    unsigned int max=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    unsigned int half=GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14;
    static int cnt=0;
    if(cnt==0) {
        static short int i = 0;
        HAL_GPIO_WritePin(GPIOE, max, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8 << i, GPIO_PIN_SET);
        HAL_Delay(1000);
        cnt = ((i++)> 7) ? 1 : 0;
        i=cnt!=0?0:i;
    }else if(cnt==1){
        static short int i = 0;
        HAL_GPIO_WritePin(GPIOE, max, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15 >> i, GPIO_PIN_SET);
        HAL_Delay(1000);
        cnt = ((i++) > 7) ? 2 : 1;
        i=cnt!=1?0:i;
    }else if(cnt==2){
        static short int i = 0;
        x=(i%2==0)?half:(half<<1);
        HAL_GPIO_WritePin(GPIOE, max, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, x, GPIO_PIN_SET);
        HAL_Delay(1000);
        cnt = ((i++) > 5) ? 0 : 2;
        i=cnt!=2?0:i;
    }
}