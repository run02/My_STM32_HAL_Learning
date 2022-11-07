#include "myAPI.h"
#include "math.h"
extern ADC_HandleTypeDef hadc1;

float calculate_to_temperature(int ad){
    float v=((float)ad)/4095*3.3;
    float r=(v*100000.0)/(3.3-v);//10kΩ
    float t=(1/(log(r/100000.0)/3950.0+(1/(273.15+25.0))))-273.15-3.4;
    return  t;
}

int measure_the_temperature(void){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,1000);
   return HAL_ADC_GetValue(&hadc1);
}

