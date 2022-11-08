#include "myAPI.h"

extern int threshold;//阈值
/*
初始化在CUBEMX里边要点一下生成代码,会在stm32f1xx_it.c中生成对应的初始化代码,在主函数里也要初始化GPIO
感觉这个再写个初始化的代码有点鸡肋,但是还是写了

 */
void key_exti_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pins : PC0 PC1 PC2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}
static inline void k1_call_back(){
    threshold++;
}
static inline void k2_call_back(){
    threshold--;
}
static inline void k3_call_back(){
//    buzz(1);
}

void HAL_GPIO_EXTI_Callback(uint16_t key) {
    if (!HAL_GPIO_ReadPin(Key_GPIO_Group, key))
        return;
    if (key == K3) {
        k3_call_back();
    } else if (key == K2) {
        k2_call_back();
    } else if (key == K1) {
        k1_call_back();
    }
    HAL_Delay(200);/*把系统时钟的优先级设置为比按键高,然后就可以在这里延时了,不然会直接卡死*/
}