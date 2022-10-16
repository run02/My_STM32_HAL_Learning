//
// Created by Ryan on 2022/10/12.
//
#include "string.h"
#include "main.h"
#include "myAPI.h"

#define TUBE_OFF 37
#define SEL0 GPIO_PIN_0
#define SEL1 GPIO_PIN_1
#define SEL2 GPIO_PIN_2
#define LED_SEL GPIO_PIN_3
#define L0 GPIO_PIN_8
//#define L7 GPIO_PIN_15

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
        0x77,
        0x7c,
        0x39,
        0x5e,
        0x79,
        0x71,
        0x3d,
        0x76,
        0x0f,
        0x0e,
        0x75,
        0x38,
        0x37,
        0x54,
        0x5c,
        0x73,
        0x67,
        0x31,
        0x49,
        0x78,
        0x3e,
        0x1c,
        0x7e,
        0x64,
        0x6e,
        0x59,
        0x40,  //"-"
        0x00,  //熄灭,
        0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0xf7,0xfc,0xb9,0xde,0xf9,0xf1
};
void smg_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                             |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

    /*Configure GPIO pins : PE8 PE9 PE10 PE11
                             PE12 PE13 PE14 PE15 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : PB0-3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void lock(){
    HAL_GPIO_WritePin(GPIOB,LED_SEL,0);
}
void unlock(){
    HAL_GPIO_WritePin(GPIOB,LED_SEL,1);
}
void light_group_writePin(uint16_t L, GPIO_PinState PinState){
    HAL_GPIO_WritePin(GPIOE,L,PinState);
}

void digital_tube_display(int pos,int num){
    int i=0;
    unlock();
    HAL_GPIO_WritePin(GPIOB,SEL0,pos&0x01);
    HAL_GPIO_WritePin(GPIOB,SEL1,(pos>>1)&0x01);
    HAL_GPIO_WritePin(GPIOB,SEL2,(pos>>2)&0x01);
    for(i=0;i<8;i++)
        light_group_writePin(L0<<i,(code_table[num]>>i&0x01));
    lock();
}

void digital_tube_display_char(int pos,char *c){
    if(strlen(c)>1){
        if (*c >= '0' && *c <= '9' && *(c+1)=='.')
            digital_tube_display(pos, *c - '0'+TUBE_OFF+1);
    }else {
        if (*c >= '0' && *c <= '9')
            digital_tube_display(pos, *c - '0');
        else if (*c >= 'A' && *c <= 'L')
            digital_tube_display(pos, *c - 'A' + 10);
        else
            digital_tube_display(pos, TUBE_OFF);
    }
}

void digital_tube_display_string(int pos,char *s){
    char temp[2];
    while (*s&&pos!=8){
        temp[0]=*s;
        if((*(s+1)=='.')){
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