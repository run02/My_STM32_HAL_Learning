//
// Created by Ryan on 2022/10/12.
//
#include "main.h"
#include "myAPI.h"
#include "stdio.h"

#define TUBE_OFF 37 //数码段表熄灭的下标

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
static char Tube_String8[8][2]={0};
void smg_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, ALL_LED, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LED_SEL|SEL0|SEL1|SEL2, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = ALL_LED;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED_SEL|SEL0|SEL1|SEL2;
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
    unlock();
    HAL_GPIO_WritePin(LOCK_GROUP,SEL0|SEL1|SEL2,0);
    HAL_GPIO_WritePin(LOCK_GROUP,pos&0x07,1);
    light_group_writePin(ALL_LED,0);
    light_group_writePin(code_table[num]<<8,1);
    lock();
}
void digital_tube_display_char(int pos,const char *c){
        if (*c >= '0' && *c <= '9' && *(c+1)=='.')
            digital_tube_display(pos, *c - '0'+TUBE_OFF+1);
        else if (*c >= '0' && *c <= '9')
            digital_tube_display(pos, *c - '0');
        else if (*c >= 'A' && *c <= 'Z')
            digital_tube_display(pos, *c - 'A' + 10);
        else if (*c >= 'a' && *c <= 'z')
            digital_tube_display(pos, *c - 'a' + 10);
        else if(*c=='-')
            digital_tube_display(pos,TUBE_OFF-1);
        else
            digital_tube_display(pos, TUBE_OFF);
}
void digital_tube_display_string(int pos,char *s){
    char temp[2];
    while (*s&&pos!=8){
        temp[0]=*s;
        if(*(s+1)=='.'){
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
/*简单粗暴看一下效果的函数*/
void test_smg_in_while1(void){
    digital_tube_display_string(0,"Hell012.3");
}
/*这个函数应该是使用最多的了,需要放在定时器中,进入一次刷新一个数码管,定时器中断最好在几毫秒内,这样显示起来看着舒服*/
void digital_tube_display_string_IT(void){
    static int pos=0;
    char s[2];
    s[0]=Tube_String8[pos][0];
    s[1]=Tube_String8[pos][1];
    digital_tube_display_char(pos++,s);
    pos=pos>7?0:pos;
}
/*从指定位开始显示字符串*/
void play_string_it(int pos,const char *s){
    while(*s&&pos!=8){
        Tube_String8[pos][0]=*(s++);
        if(*s) {
            if (*s == '.') {
                Tube_String8[pos++][1] = '.';
                s++;
            } else
                Tube_String8[pos++][1] = '\0';
        } else
            break;
    }
}
/*在指定范围内显示整数*/
void play_num_it(int start,int end,int num){
    char fmt[]={'%',end+1-start+'0','d'};
    char n[9]={0};
    sprintf(n,fmt,num);
    n[end+1-start]='\0';
    play_string_it(start,n);
}
/*在指定空间内显示小数,可设置保留几位小数点*/
void play_float_it(int start,int end,float num,int len_after_point){
    char buffer[9];
    char fmt[]={'%',end+1-start+'0','.',len_after_point+'0','f'};
    sprintf(buffer, fmt, num);
    buffer[end+1-start]='\0';
    play_string_it(start,buffer);
}
