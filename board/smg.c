//
// Created by Ryan on 2022/10/12.
//
#include "main.h"
#include "myAPI.h"

#define TUBE_OFF 37 //数码段表熄灭的下标
#define SEL0 GPIO_PIN_0 //译码器A0
#define SEL1 GPIO_PIN_1 //译码器A1
#define SEL2 GPIO_PIN_2 //译码器A2
#define LED_SEL GPIO_PIN_3 //LED灯开关
#define L0 GPIO_PIN_8  //LED0 (数码段的A)
#define L1 GPIO_PIN_9
#define L2 GPIO_PIN_10
#define L3 GPIO_PIN_11
#define L4 GPIO_PIN_12
#define L5 GPIO_PIN_13
#define L6 GPIO_PIN_14
#define L7 GPIO_PIN_15
#define LOCK_GROUP GPIOB //LED灯开关的引脚的组
#define LIGHT_GROUP GPIOE //LED灯的引脚的组
#define ALL_LED L0|L1|L2|L3|L4|L5|L6|L7

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
//    uint32_t L=L0;
//    uint32_t P=code_table[num];
//    HAL_GPIO_WritePin(LOCK_GROUP,SEL0,pos&0x01);
//    HAL_GPIO_WritePin(LOCK_GROUP,SEL1,(pos>>1)&0x01);
//    HAL_GPIO_WritePin(LOCK_GROUP,SEL2,(pos>>2)&0x01);
//    unlock();
//    while (L>>=1)
//        P<<=1;
//    for(int i=0;i<8;i++)
//        light_group_writePin(L0<<i,code_table[num]&0x01);
//    lock();
    unlock();
    HAL_GPIO_WritePin(LOCK_GROUP,SEL0|SEL1|SEL2,0);
    HAL_GPIO_WritePin(LOCK_GROUP,pos&0x07,1);
    light_group_writePin(ALL_LED,0);
    light_group_writePin(code_table[num]<<8,1);
    lock();
}

void digital_tube_display_char(int pos,char *c){
        if (*c >= '0' && *c <= '9' && *(c+1)=='.')
            digital_tube_display(pos, *c - '0'+TUBE_OFF+1);
        else if (*c >= '0' && *c <= '9')
            digital_tube_display(pos, *c - '0');
        else if (*c >= 'A' && *c <= 'Z')
            digital_tube_display(pos, *c - 'A' + 10);
        else if (*c >= 'a' && *c <= 'z')
            digital_tube_display(pos, *c - 'a' + 10);
        else
            digital_tube_display(pos, TUBE_OFF);
}
/*这个函数应该是使用最多的了,放在定时器*/
void digital_tube_display_string_IT(void){
    static int pos=0;
    char *s=Tube_String8[pos];
    digital_tube_display_char(pos++,s);
    pos=pos>7?0:pos;
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

void test_smg_in_while1(void){
    digital_tube_display_string(0,"Hell012.3");
}
