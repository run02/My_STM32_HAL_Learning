//
// Created by Ryan on 2022/10/12.
//
#include "main.h"
#include "myAPI.h"
#define TUBE_OFF 21
#define SEL0 GPIO_PIN_0
#define SEL1 GPIO_PIN_1
#define SEL2 GPIO_PIN_2
#define LED_SEL GPIO_PIN_3
#define L0 GPIO_PIN_8
#define L7 GPIO_PIN_15

short int code_table[]={
0xc0,//0
0xf9,//1
0xa4,//2
0xb0,//3
0x99,//4
0x92,//5
0x82,//6
0xf8,//7
0x80,//8
0x90,//9
0x88,//A
0x83,//B
0xc6,//C
0xa1,//D
0x86,//E
0x8e, //F
0x8c, //P
0xc1,//U
0x91,//Y
0x7c,//L
0x00,//全亮
0xff  //熄灭
};

void lock(){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
}
void unlock(){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
}
void light_group_writePin(uint16_t L, GPIO_PinState PinState){
    HAL_GPIO_WritePin(GPIOE,L,PinState);
}

void digital_tube_display(int pos,int num){

    uint8_t sel0=(pos>>2)&0x01;
    uint8_t sel1=(pos>>1)&0x01;
    uint8_t sel2=pos&0x01;
    int i=0;
    unlock();
    HAL_GPIO_WritePin(GPIOB,SEL0,sel0);
    HAL_GPIO_WritePin(GPIOB,SEL1,sel1);
    HAL_GPIO_WritePin(GPIOB,SEL2,sel2);
    for(i=0;i<8;i++)
        light_group_writePin(L0<<i,code_table[num]>>i);


    lock();
}