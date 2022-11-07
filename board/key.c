#include "myAPI.h"
#define Key_GPIO_Group GPIOC
#define K3 GPIO_PIN_0
#define K2 GPIO_PIN_1
#define K1 GPIO_PIN_2


extern int threshold;//阈值

static inline void k1_call_back(){
    threshold++;
}
static inline void k2_call_back(){
    threshold--;
}
static inline void k3_call_back(){

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
}