#ifndef _boardLayout_h_
#define _boardLayout_h_


/*LED与数码管*/
#define LED_SEL GPIO_PIN_3 //LED灯开关
#define LED_GROUP GPIOE

#define SEL0 GPIO_PIN_0 //译码器A0
#define SEL1 GPIO_PIN_1 //译码器A1
#define SEL2 GPIO_PIN_2 //译码器A2

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
/*LED与数码管*/

/*按键*/
#define Key_GPIO_Group GPIOC
#define K3 GPIO_PIN_0
#define K2 GPIO_PIN_1
#define K1 GPIO_PIN_2
/*按键*/

/*蜂鸣器*/
#define BUZZ_GROUP GPIOE
#define BUZZ_PIN GPIO_PIN_5

#endif